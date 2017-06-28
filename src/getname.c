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
    if (set_antispamdomain) {
        return spamify_replacedomain(input, set_antispamdomain);
    }
    else {
        return spamify_small(input);
    }
}

char *spamify_small(char *input)
{
    int insertlen = strlen(set_antispam_at);
    /* we should replace the @-letter in the email address */
    int newlen = strlen(input) + insertlen;

    char *atptr = strchr(input, '@');

    if (atptr) {
        char *newbuf = malloc(newlen);
        int index = atptr - input;
        /* copy the part before the @ */
        memcpy(newbuf, input, index);
        memcpy(newbuf + index, set_antispam_at, insertlen);

        /* append the part after the @ */
        strcpy(newbuf + index + insertlen, input + index + 1);

        /* correct the pointer and free the old */
        free(input);
        return newbuf;
    }
    /* weird email, bail out */
    return input;
}

char *spamify_replacedomain(char *input, char *antispamdomain)
{
    /* replace everything after the @-letter in the email address */
    int newlen = strlen(input) + strlen(set_antispam_at);
    int domainlen = strlen(antispamdomain);

    char *atptr = strchr(input, '@');

    if (domainlen > 0) {
        newlen = newlen + domainlen;
    }

    if (atptr) {
        char *newbuf = malloc(newlen);
        int index    = atptr - input;
        /* copy the part before the @ */
        memcpy(newbuf, input, index);
        /* append _at_ */
        memcpy(newbuf + index, set_antispam_at, strlen(set_antispam_at));
        if (domainlen > 0) {
            /* append the new domain */
            strcpy(newbuf + index + strlen(set_antispam_at), antispamdomain);
        }
        else {
            /* append the part after the @ */
            strcpy(newbuf + index + strlen(set_antispam_at), input + index + 1);
        }
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

/* AUDIT biege: this code is really tricky and may lead to BOFs in email[] and/or name[] */
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
    ** Zero out data storage.
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
        if (strchr(line, '(')) {    /* From: bob (The Big Guy) */
            c = strchr(line, ':') + 1;
            while (*c == ' ' || *c == '\t')
                c++;
            for (i = 0; *c && *c != '(' && *c != ' ' && *c != '\t' && *c != '\n' && i < len; c++)
                email[i++] = *c;
            email[i] = '\0';
        }
        else if ((c = strchr(line, '<'))) {    /* From: <kent> */
            c++;
            for (i = 0; *c && *c != '>' && *c != ' ' && *c != '\t' && *c != '\n' && i < len; c++)
                email[i++] = *c;
            email[i] = '\0';
        }
        else {
            /* 
            ** - check to see if the From: line is blank, (taken care of)
            ** - check if From: uu.net!kent formatted line
            ** - check if "From: kent" formatted line
            */
            c = strchr(line, ':') + 1;
            while (*c == ' ' || *c == '\t')
                c++;

            for (i = 0; *c && *c != ' ' && *c != '\t' && *c != '\n' && *c != ',' && i < len; c++)
                email[i++] = *c;
            email[i] = '\0';
        }

        if (email[0] == '\0')    /* Was it a junk From line ? */
            strcpymax(email, NOEMAIL, MAILSTRLEN);

        else if (use_domainaddr) {
            /*
             * check if site domainizes addresses 
             * but don't modify uucp addresses
             */
            if ((c = strchr(email, '!')) == NULL) {
                if (strlen(email) < len) { 
                    strcat(email, "@");
                    strncat(email, set_domainaddr, len-strlen(email));
                }
            }
        }
    }
    else {
        while (*c != ' ' && *c != '\t' && *c != '<' && *c != '"' && *c != ':')
            c--;
        c++;
        for (i = 0; *c && *c != '>' && *c != ' ' && *c != '\t' && *c != '"' && *c != '\n' && *c != ']' && *c != ',' && i < len; c++)
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
            for (i = 0, len = NAMESTRLEN - 1; *c && *c != '\"' && *c != '\n' && i < len; c++)
                name[i++] = *c;

            if (rmparen && name[(i - 1)] == ')')
                --i;        /* get rid of "(name-comment)" parens */

            comment_fnd = 1;
        }
        else if (hm_strchr(line, '(')) {
            c = hm_strchr(line, '(') + 1;
            if (*c == '"')    /* is there a comment in the comment ? */
                c++;
        }
        else if (*c == '<') {    /* Comment may be on the end */
            /* From: <bill@celestial.com> Bill Campbell */
            char *c2 = strchr(line, '>');
            if (c2 != NULL) {
                c = c2 + 1;
                for (i = 0, len = NAMESTRLEN - 1; *c && *c != '\n' && i < len; c++)
                    name[i++] = *c;

                comment_fnd = 1;
            }
        }
    }
    else if (strchr(line, '(')) {
        c = strchr(line, '(');
        c++;
        if (*c == '"')        /* is there a comment in the comment ? */
            c++;
        while (*c == ' ' || *c == '\t')
            c++;
    }
    else if (strchr(line, '[')) {
        c = strchr(line, ':') + 1;
        while (*c == ' ' || *c == '\t')
            c++;

        for (i = 0, len = NAMESTRLEN - 1; *c && *c != '\"' && *c != '[' && *c != '\n' && i < len; c++)
            name[i++] = *c;

        name[--i] = '\0';
        comment_fnd = 1;
    }
    else {
        /*
         * Is there an email address available 
         * that we can use for the name ?
         */
        if (!strcmp(email, NOEMAIL))    /* No */
            strcpymax(name, NONAME, NAMESTRLEN);
        else {
            c = email + strlen(email) - 1;
            while (isspace(*c))
                *c-- = '\0';
            strcpymax(name, email, NAMESTRLEN);    /* Yes */
        }
        *namep = strsav(name);
        *emailp = strsav(email);
        return;
    }

    if (!comment_fnd) {
        int in_ascii = TRUE, esclen = 0;
        for (i = 0, len = NAMESTRLEN - 1; *c && *c != '<' && *c != '\"' && *c != ')' && *c != '(' && *c != '\n' && i < len; c++) {
            if (set_iso2022jp) {
                iso2022_state(c, &in_ascii, &esclen);
                if (esclen) {
                    for (; esclen; esclen--, c++)
                        name[i++] = *c;
                    for (; in_ascii == FALSE && i < len; c++, iso2022_state(c, &in_ascii, &esclen)) {
                        name[i++] = *c;
                    }
                    c--;
                }
                else {
                    name[i++] = *c;
                }
            }
            else {
                name[i++] = *c;
            }
        }
    }

    if (i > 0 && name[i - 1] == ' ' && (*c == '<' || *c == '('))
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
