#include "hypermail.h"
#include "getname.h"
#include "setup.h"

extern char *set_domainaddr;

static int blankstring(char *str)
{
    register char *cp;
    for (cp = str; *cp; cp++) {
	if (*cp != ' ' && *cp != '\t' && *cp != '\r' && *cp != '\n')
	    return (0);
    }
    return (1);
}

char *spamify(char *input)
{
  /* we should replace the @-letter in the email
     address */
  int newlen=strlen(input)+4;
  char *atptr=strchr(input, '@');
  if(atptr) {
    char *newbuf = malloc(newlen);
    int index=atptr-input;
    /* copy the part before the @ */
    memcpy(newbuf, input, index);
    /* append _at_ */
    memcpy(newbuf+index, "_at_", 4);
    /* append the part after the @ */
    strcpy(newbuf+index+4, input+index+1);
    /* correct the pointer and free the old */
    free(input);
    return newbuf;
  }
  /* weird email, bail out */
  return input;
}

/*
** Grabs the name and email address from a From: header.
** This could get tricky; I've tried to keep it simple.
** Should be able to handle all the addresses below:
**
**   From: user                   [no @]
**   From: kent (Kent Landfield)  [no @ - with comment]
**   From: <user@node.domain>     [no text name, use email as text name]
**   From: Kent Landfield <kent>  [text name but no @]
**   From: (kent)                 [comment - no email address]
**   From: "" <kent>              [email address but null comment]
**   From:                        [blank From: line]
**   From: uu.net!kent            [uucp addresses - no comment]
**   From: uu.net!kent (kent)     [uucp addresses - with comment]
**   From: "(Joe Bloggs)" <joe@anorg.com> 
**   From: "Roy T. Fielding" <fielding@kiwi.ics.uci.edu>
**   From: kent@localhost
**   From: kent@uu.net (Kent Landfield)
**   From: (George Burgyan) <gburgyan@cybercon.com>
**   From: <gburgyan@cybercon.com> (George Burgyan) 
**   From:              Kent B. Landfield <kent@landfield.com>
**   From:      IN%"fekete+reply@c2.net" 26-JAN-1997 13:28:55.36
**   From:      IN%"vicric@panix.com"  "Vicki Richman" 13-AUG-1996 10:54:33.38
**   From:      US2RMC::"lwv26@cas.org" "Larry W. Virden, x2487" 22-OCT-1994 09:44:21.44
**   From:          Mail Delivery Subsystem <postmaster@igc.apc.org>
**   From:          Self <ehasbrouck>
**   From:         adam@eden.apana.org.au (Adam Frey)
**   From:        faqserv@penguin-lust.mit.edu
**   From:    nc0548@freebsd.netcom.com (Mark Hittinger)
**   From: "- Pam Greene, one of the *.answers moderators" <pgreene@MIT.EDU>
**   From: "Felan shena Thoron'edras" <felan@netcom.com>
**   From: David Muir Sharnoff <muir@idiom.com>
**   From: A.J.Doherty@reading.ac.uk (Andy Doherty)
**   From: Jordan Hubbard                        <jkh@vector.eikon.e-technik.tu-muenchen.de>
**   From: ZXPAN%SLACVM.BITNET@MITVMA.MIT.EDU
**   From: afs!piz!alf@uu5.psi.com (Alf the Poet)
**   From: answers@cp.tn.tudelft.nl ("Moderator *.answers")
**   From: mdw%merengue@merengue.oit.unc.edu (Matt Welsh)
**   From: bgoffe@whale.st.usm.edu (William L. Goffe)
**
** This is an interesting new one (1998-11-26):
** From: <name.hidden@era.ericsson.se>›Name.Hidden@era.ericsson.seœ
*/

void getname(char *line, char **namep, char **emailp)
{
    int i;
    int len;
    char *c;
    int comment_fnd;

    char email[MAILSTRLEN];
    char name[NAMESTRLEN];

    len = MAILSTRLEN - 1;
    comment_fnd = 0;

    /*
     * Zero out data storage.
     */
    memset(email, 0, MAILSTRLEN);
    memset(name, 0, NAMESTRLEN);

    *namep = NULL;
    *emailp = NULL;

    /* EMail Processing First:
       ** First, is there an '@' sign we can use as an anchor ?
     */
    if ((c = hm_strchr(line, '@')) == NULL) {
	/* 
	   ** No '@' sign here so ...
	 */
	if (strchr(line, '(')) {	/* From: bob (The Big Guy) */
	    c = strchr(line, ':') + 1;
	    while (*c == ' ' || *c == '\t')
		c++;
	    for (i = 0; *c && *c != '(' && *c != ' ' &&
		 *c != '\t' && *c != '\n' && i < len; c++)
		email[i++] = *c;
	    email[i] = '\0';
	}
	else if ((c = strchr(line, '<'))) {	/* From: <kent> */
	    c++;
	    for (i = 0; *c && *c != '>' && *c != ' ' &&
		 *c != '\t' && *c != '\n' && i < len; c++)
		email[i++] = *c;
	    email[i] = '\0';
	}
	else {
	    /* 
	     *    - check to see if the From: line is blank, (taken care of)
	     *    - check if From: uu.net!kent formatted line
	     *    - check if "From: kent" formatted line
	     */
	    c = strchr(line, ':') + 1;
	    while (*c == ' ' || *c == '\t')
		c++;
	    for (i = 0; *c && *c != ' ' && *c != '\t' &&
		 *c != '\n' && *c != ',' && i < len; c++)
		email[i++] = *c;
	    email[i] = '\0';

	}

	if (email[0] == '\0')	/* Was it a junk From line ? */
	    strcpymax(email, NOEMAIL, MAILSTRLEN);

	else if (use_domainaddr) {
	    /*
	     * check if site domainizes addresses 
	     * but don't modify uucp addresses
	     */
	    if ((c = strchr(email, '!')) == NULL) {
		strcat(email, "@");
		strcat(email, set_domainaddr);
	    }
	}
    }
    else {
	while (*c != ' ' && *c != '\t' && *c != '<' && *c != '"' &&
	       *c != ':') c--;
	c++;
	for (i = 0; *c && *c != '>' && *c != ' ' && *c != '\t' &&
	     *c != '"' && *c != '\n' && *c != ']' && *c != ',' &&
	     i < len; c++)
	    email[i++] = *c;
	email[i] = '\0';
    }

    /*
     * NAME Processing - Boy are there a bunch of funky formats here.
     *                   No promises... I'll do my best. Let me know
     *                   what I missed...
     */

    if (strchr(line, '<')) {
	c = strchr(line, ':') + 1;
	while (*c == ' ' || *c == '\t')
	    c++;

	/* if a comment then just look for the end point */

	if (*c == '\"') {
	    int rmparen = 0;

	    ++c;
	    if (*c == '(') {
		++c;
		rmparen = 1;
	    }
	    for (i = 0, len = NAMESTRLEN - 1;
		 *c && *c != '\"' && *c != '\n' && i < len; c++)
		name[i++] = *c;

	    if (rmparen && name[(i - 1)] == ')')
		--i;		/* get rid of "(name-comment)" parens */

	    comment_fnd = 1;
	}
	else if (hm_strchr(line, '(')) {
	    c = hm_strchr(line, '(') + 1;
	    if (*c == '"')	/* is there a comment in the comment ? */
		c++;
	}
	else if (*c == '<') {	/* Comment may be on the end */
	    /* From: <bill@celestial.com> Bill Campbell */
	    c = strchr(line, '>') + 1;
	    for (i = 0, len = NAMESTRLEN - 1; *c && *c != '\n' && i < len;
		 c++)
		name[i++] = *c;

	    comment_fnd = 1;
	}
    }
    else if (strchr(line, '(')) {
	c = strchr(line, '(');
	c++;
	if (*c == '"')		/* is there a comment in the comment ? */
	    c++;
	while (*c == ' ' || *c == '\t')
	    c++;
    }
    else if (strchr(line, '[')) {
	c = strchr(line, ':') + 1;
	while (*c == ' ' || *c == '\t')
	    c++;

	for (i = 0, len = NAMESTRLEN - 1;
	     *c && *c != '\"' && *c != '[' && *c != '\n' && i < len; c++)
	    name[i++] = *c;

	name[--i] = '\0';
	comment_fnd = 1;
    }
    else {
	/*
	 * Is there an email address available 
	 * that we can use for the name ?
	 */
	if (!strcmp(email, NOEMAIL))	/* No */
	    strcpymax(name, NONAME, NAMESTRLEN);
	else {
	    c = email + strlen(email) - 1;
	    while (isspace(*c))
		*c-- = '\0';
	    strcpymax(name, email, NAMESTRLEN);	/* Yes */
	}
	*namep = strsav(name);
	*emailp = strsav(email);
	return;
    }

    if (!comment_fnd) {
	int in_ascii = TRUE, esclen = 0;
	for (i = 0, len = NAMESTRLEN - 1;
	     *c && *c != '<' && *c != '\"' && *c != ')' && *c != '(' &&
	     *c != '\n' && i < len; c++)
	{
		if (set_iso2022jp) {
			iso2022_state(c, &in_ascii, &esclen);
			if (esclen) {
				for (; esclen; esclen--, c++) name[i++] = *c;
				for (; in_ascii == FALSE && i < len;
				     c++, iso2022_state(c, &in_ascii, &esclen)) {
					name[i++] = *c;
				}
				c--;
			} else {
				name[i++] = *c;
			}
		} else {
			name[i++] = *c;
		}
	}
    }

    if (name[i-1] == ' ' && *c == '<' || *c == '(')
	name[--i] = '\0';
    else
	name[i] = '\0';

    /*
     * Is the name string blank ? If so then 
     * force it to get filled with something.
     */
    if (blankstring(name))
	name[0] = '\0';

    /* Bailing and taking the easy way out... */

    if (name[0] == '\0') {
	if (email[0] == '\0')
	    strcpymax(name, NONAME, NAMESTRLEN);
	else
	    strcpymax(name, email, NAMESTRLEN);
    }

    /* 
     * need to strip spaces off the end of 
     * the email and name strings 
     */

    c = email + (strlen(email) - 1);
    while (c > email && isspace(*c))
	*c-- = '\0';

    *namep = strsav(name);
    *emailp = strsav(email);
}

#if 0
/* Retaining for now. It does not process address on the From: line correctly
** and there is a array bounds read problem under certain contitions. 
*/
/****************************************************************************
 *
 * Getting name and email part from a From: -line
 *
 * Daniel Stenberg rewrote the whole routine Nov 24 1998.
 * Rewrote it again spring 1999.
 *
 * "This works even better"
 *
 ***************************************************************************/

#include "setup.h"

void getname(char *buffer, char **namep, char **emailp)
{
    char *ptr;
    char *see;

    char *startofemail;
    char *endofemail;

    char *startofcomment = NULL;
    char *endofcomment;

    int emaillen = 0;
    int commentlen = 0;

    struct Push namebuf;
    struct Push emailbuf;

    /* start with NULLs */
    *namep = *emailp = NULL;

    /* first, init both string buffers */
    INIT_PUSH(namebuf);
    INIT_PUSH(emailbuf);

    ptr = buffer;

    if (!strncasecmp("From: ", ptr, 6)) {
	ptr += 6;

	while (ptr && *ptr && isspace(*ptr))
	    ptr++;
#ifdef DEBUG_GETFROM
	printf("** IN: %s", ptr);
#endif

	/* <one machine-usable reference> */
	/* (comment) */

	endofemail = NULL;
	startofemail = strchr(ptr, '<');
	if (startofemail) {
	    endofemail = strchr(++startofemail, '>');
	    emaillen = endofemail - startofemail;
#ifdef DEBUG_GETFROM
	    printf("FOUND <>\n");
#endif
	}
	endofcomment = NULL;

	if (!endofemail) {
	    startofcomment = strchr(ptr, '(');
	    if (startofcomment) {
		endofcomment = strchr(++startofcomment, ')');
		commentlen = endofcomment - startofcomment;
#ifdef DEBUG_GETFROM
		printf("FOUND ()\n");
#endif
	    }
	}

    /******** We found neither an email field nor comment ******/
	if (!endofemail && !endofcomment) {
	    char at = 0;
	    /* We make an attempt to get an email address from the string! */
	    startofemail = ptr;
	    see = ptr;

	    while (*see) {
		if ('@' == *see) {
		    at = 1;
		}
		else if (isspace(*see)) {
		    if (at) {
			break;
		    }
		    if ('\n' == *see)
			break;
		    at = 0;
		    startofemail = see + 1;
		}
		see++;
	    }
	    if (at) {
		endofemail = see;
		emaillen = endofemail - startofemail;
#ifdef DEBUG_GETFROM
		printf("USE word with @!\n");
#endif
	    }
	    else {
		/* No, then the whole line is the email address! */
		endofemail = see;
		emaillen = endofemail - startofemail;
#ifdef DEBUG_GETFROM
		printf("USE last word without space!\n");
#endif
	    }

	}

    /******** We got an email field but no comment ******/
	if (!endofcomment && endofemail) {
	    if (startofemail > ptr) {
		/* we set the text before the email address to be a comment */
		startofcomment = ptr;
		endofcomment = startofemail;
#ifdef DEBUG_GETFROM
		printf("USE comment BEFORE email\n");
#endif
	    }
	    else {
		/* we set the text after the email address to be a comment */
		startofcomment = endofemail + 1;
		endofcomment = strchr(endofemail, '\n');
#ifdef DEBUG_GETFROM
		printf("USE comment AFTER email\n");
#endif
	    }
	    commentlen = endofcomment - startofcomment - 1;
	}

    /******** We got a comment field but no email ******/
	if (endofcomment && !endofemail) {
	    if (startofcomment > ptr) {
		/* we set the text before the comment to be the email */
		startofemail = ptr;
		endofemail = startofcomment;
#ifdef DEBUG_GETFROM
		printf("USE email BEFORE comment\n");
#endif
	    }
	    else {
		/* we set the text after the comment to be the email */
		startofemail = endofcomment + 1;
		endofemail = strchr(endofcomment, '\n');
#ifdef DEBUG_GETFROM
		printf("USE email AFTER comment\n");
#endif
	    }
	    emaillen = endofemail - startofemail - 1;
	}

	if (endofemail) {
	    /* this may be an email address */
#ifdef DEBUG_GETFROM
	    printf("EMAIL: %.*s\n", emaillen, startofemail);
#endif
	    PushNString(&emailbuf, startofemail, emaillen);

	    /* If there's no @-letter, we may add a domain */
	    if (use_domainaddr && !strchr(PUSH_STRING(emailbuf), '@')) {
		/* don't modify uucp addresses */
		if (strchr(PUSH_STRING(emailbuf), '!')) {
		    PushByte(&emailbuf, '@');
		    PushString(&emailbuf, set_domainaddr);
		}
	    }
	}
	if (endofcomment) {
	    /* this may be an email address */
#ifdef DEBUG_GETFROM
	    printf("COMMENT: %.*s\n", commentlen, startofcomment);
#endif

	    /* pass all initiating white spaces or quotes */
	    while (*startofcomment &&
		   (isspace(*startofcomment) || *startofcomment == '\"')) {
		startofcomment++;
		commentlen--;
	    }

	    /* don't count the trailing white spaces or quotes */
	    while (commentlen &&
		   (isspace(startofcomment[commentlen - 1]) ||
		    startofcomment[commentlen - 1] == '\"')) {
		commentlen--;
	    }
	    PushNString(&namebuf, startofcomment, commentlen);
	}

	if (!endofemail && !endofcomment) {
#ifdef DEBUG_GETFROM
	    printf("-- FAILED: %s", ptr);
#endif
	}
    }
    *namep = PUSH_STRING(namebuf);
    *emailp = PUSH_STRING(emailbuf);
}

#endif
