/* 
** Copyright (C) 1994, 1995 Enterprise Integration Technologies Corp.
**         VeriFone Inc./Hewlett-Packard. All Rights Reserved.
** Kevin Hughes, kev@kevcom.com 3/11/94
** Kent Landfield, kent@landfield.com 4/6/97
** 
** This program and library is free software; you can redistribute it and/or 
** modify it under the terms of the GNU (Library) General Public License 
** as published by the Free Software Foundation; either version 2 
** of the License, or any later version. 
** 
** This program is distributed in the hope that it will be useful, 
** but WITHOUT ANY WARRANTY; without even the implied warranty of 
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the 
** GNU (Library) General Public License for more details. 
** 
** You should have received a copy of the GNU (Library) General Public License
** along with this program; if not, write to the Free Software 
** Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA 
*/

/*
** All the nasty string functions live here.
*/

#include "hypermail.h"
#include "setup.h"
#include "parse.h"

/*
** Push byte onto a buffer realloc the buffer if needed.
**
** Returns the (new) buffer pointer.
*/
char *PushByte(struct Push *push, char byte)
{				/* byte to append */
#define PUSH_DEFAULT 32		/* default strings are this big */
    if (!push)
	return NULL;		/* this is a sever error */
    if (!push->string) {
	push->string = (char *)malloc(PUSH_DEFAULT);
	if (!push->string)
	    return NULL;	/* error again */
	push->alloc = PUSH_DEFAULT;
	push->len = 0;
#ifdef DEBUG_PUSH
	fprintf(stderr, "PUSH: INIT at index 0 alloc %d\n", PUSH_DEFAULT);
#endif
    }
    else if ((push->len + 2) >= push->alloc) {
	char *newptr;
	newptr = (char *)realloc(push->string, push->alloc * 2);	/* double the size */
	if (!newptr) {
	    return push->string;	/* live on the old one! */
	}
	push->alloc *= 2;	/* enlarge the alloc size */
	push->string = newptr;	/* use the new buffer */
#ifdef DEBUG_PUSH
	fprintf(stderr,
		"PUSH: REALLOC at index %d to alloc %d\n", push->len,
		push->alloc);
#endif
    }
#ifdef DEBUG_PUSH
    fprintf(stderr, "PUSH: WRITE '%c' at index %d\n", byte, push->len);
#endif
    push->string[push->len++] = byte;
    push->string[push->len] = 0;	/* zero terminate */

    return push->string;	/* current buffer */
}

/*
** Push a string onto a buffer, and realloc the buffer if needed.
**
** Returns the (new) buffer pointer.
*/
char *PushString(struct Push *push, char *append)
{				/* string to append */
    char *string = NULL;

    while (*append) {		/* continue until zero termination */
	string = PushByte(push, *append);
	append++;		/* get next character */
    }

    return string;		/* this is the new buffer */
}

/*
** Push a limited string onto a buffer, and realloc the buffer if needed.
**
** Returns the (new) buffer pointer.
*/
char *PushNString(struct Push *push, char *append,	/* string to append */
		  int size)
{				/* maximum number of bytes to copy */
    char *string = NULL;

    while (*append && size--) {	/* continue until zero termination */
	string = PushByte(push, *append);
	append++;		/* get next character */
    }

    return string;		/* this is the new buffer */
}


/*
** Malloc() out a string, give it to whoever asked for it.
*/

char *strsav(char *s)
{
    char *p;

    if (NULL == s)
	s = "";			/* Daniel's kludge to survive better */

    p = (char *)emalloc(strlen(s) + 1);
    strcpy(p, s);
    return p;
}

char *strreplace(char *present, char *new)
{
    char *retval;
    int len;
    if (new == NULL) {
	free(present);
	return NULL;
    }
    len = strlen(new) + 1;
    if (present == NULL)
	retval = (char *)malloc(len);
    else
	retval = (char *)realloc(present, len);
    return strcpy(retval, new);	/* CCC is safe, buffer allocated for it
					   by length of string 'new' */
}

/*
** strcpymax() - copies a string, but max N bytes. It guarantees the
** destination string to be zero terminated. That is, if the destination
** buffer is X bytes, set N to X.
*/

void strcpymax(char *dest, const char *src, int n)
{
    int i;
    if (n) {
	n--;			/* decrease one to allow for the termination byte */
	for (i = 0; *src && (i < n); i++)
	    *dest++ = *src++;
    }
    *dest = 0;
}

/*
** strcasestr() - case insensitive strstr()
*/

char *strcasestr(char *haystack, char *needle)
{
    int nlen = strlen(needle);
    int hlen = strlen(haystack);

    int i;
    int max;

    max = hlen - nlen;

    for (i = 0; i <= max; i++) {
	if (!strncasecmp(haystack, needle, nlen))
	    return haystack;
	haystack++;
    }
    return NULL;
}


/*
** Strips the timezone information from long date strings, so more correct
** comparisons can be made between dates when looking for article replies.
** Y2K ok.
*/

char *stripzone(char *date)
{
    int num;
    static char tmpdate[DATESTRLEN];

    if (!strcmp(date, NODATE))
	return (date);

    if (!strchr(date, ':'))
	return (date);

    strcpymax(tmpdate, date, DATESTRLEN);
    num = strlen(tmpdate);
    while (tmpdate[num] != ':')
	num--;

    num += 3;
    while (tmpdate[num])
	tmpdate[num++] = '\0';

    return (tmpdate);
}

/*
** How many times does the character c appear in string s?
*/

int numstrchr(char *s, char c)
{
    int i;

    for (i = 0; *s != '\0'; s++) {
	if (*s == c)
	    i++;
    }
    return i;
}


/*
** Grabs whatever happens to be between the outermost double quotes in a line.
** This is for grabbing the values in comments.
*/

char *getvalue(char *line)
{
    int i;
    int len;
    char *c, *d;
    struct Push buff;

    INIT_PUSH(buff);

    c = strchr(line, '\"');
    d = strrchr(line, '\"');
    if (c == NULL)
	return "";
    for (c++, i = 0, len = MAXLINE - 1; *c && c != d && i < len; c++)
	PushByte(&buff, *c);

    RETURN_PUSH(buff);
}


/* 
** Get rid of Re:'s in a subject and strips spaces at the end
** of subjects. Make the subject index much less cluttered.
**
** Returns an ALLOCATED string.
*/

char *unre(char *subject)
{
    int ws = 0;
    char *c, *s;

    struct Push buff;
    INIT_PUSH(buff);

    s = subject;

    while (findre(s, &c)) {
	s = c;			/* get position after next re-string */
    }

    while (*s && isspace(*s))
	s++;

    c = s;			/* the first non-space position after the last re: */

    while (*c) {
	if (c && isspace(*c))
	    ws++;
	else {
	    if (ws) {
		PushByte(&buff, ' ');
		ws = 0;
	    }
	    PushByte(&buff, *c);
	}
	c++;
    }
    if (!PUSH_STRLEN(buff)) {
	PushByte(&buff, '\0');
    }

    RETURN_PUSH(buff);
}

/*
** Only gets rid of one re: in a subject, so messages the subject is a reply to
** can be guessed.
*/

char *oneunre(char *subject)
{
    char *c;
    struct Push buff;

    INIT_PUSH(buff);

    if (isre(subject, &c)) {
	if (*c && isspace(*c))
	    c++;
	subject = c;
	PushString(&buff, subject);
	RETURN_PUSH(buff);
    }
    return NULL;
}

/*
** Is a line in an article body part of a quoted passage?
*/

int isquote(char *line)
{
    char *lp;

    if (!line)
	return (0);

    if (*line == '>')
	return 1;

    lp = line;

#ifdef RECOGNIZE_SUPERCITE_QUOTES
    /*
       ** If there is a ":" in the first column, 
       ** it means the text is quoted. 
     */
    if (*lp == ':') {
	char *cp;
	/* 
	   ** Check to make sure that smileys are not
	   ** intrepreted as Supercite Quotes.
	 */
	cp = lp + 1;
	if (*cp && *cp != '-' && *cp != ']' &&
	    *cp != '>' && *cp != '(' && *cp != ')' && *cp != '^')
	    return 1;
    }
#endif

    while (*lp && (*lp == ' ' || *lp == '\t'))
	lp++;

    if (!(*lp))
	return 0;

#ifdef RECOGNIZE_SUPERCITE_QUOTES
    /*
       ** recognize citations in the form "  Jane>" 
     */

    while (*lp &&
	   ((*lp >= 'A' && *lp <= 'Z') ||
	    (*lp >= 'a' && *lp <= 'z'))) lp++;
#endif

    if (*lp == '>')
	return 1;

    return 0;
}

/*
** Converts <, >, and & to &lt;, &gt; and &amp;.
** It was ugly. Now its better. And probably faster.
**
** Returns an ALLOCATED string!
*/

char *convchars(char *line)
{
    struct Push buff;

    INIT_PUSH(buff);		/* init macro */

    /* avoid strlen() for speed */

    for (; *line; line++) {
	switch (*line) {
	case '<':
	    PushString(&buff, "&lt;");
	    break;
	case '>':
	    PushString(&buff, "&gt;");
	    break;
	case '&':
	    PushString(&buff, "&amp;");
	    break;
	case '\"':
	    PushString(&buff, "&quot;");
	    break;
	default:
	    PushByte(&buff, *line);
	}
    }
    RETURN_PUSH(buff);
}

/*
** Just the opposite of convchars().
** Completely rewritten 17th Nov 1998 by Daniel.
*/

char *unconvchars(char *line)
{
    struct Push buff;
    INIT_PUSH(buff);

    for (; *line; line++) {
	if (*line == '&') {
	    if (!strncmp("lt;", line + 1, 3)) {
		PushByte(&buff, '<');
		line += 3;
	    }
	    else if (!strncmp("gt;", line + 1, 3)) {
		PushByte(&buff, '>');
		line += 3;
	    }
	    else if (!strncmp("amp;", line + 1, 4)) {
		PushByte(&buff, '&');
		line += 4;
	    }
	    else if (!strncmp("quot;", line + 1, 5)) {
		PushByte(&buff, '\"');
		line += 5;
	    }
	    else
		PushByte(&buff, *line);
	}
	else
	    PushByte(&buff, *line);
    }
    RETURN_PUSH(buff);
}

/*
 * translatechars() is just a ripoff of convertchars() 
 * with a different argument list.
 */

static void translatechars(char *start, char *end, struct Push *buff)
{
    char *p;

    for (p = start; p <= end; p++) {

	switch (*p) {

	case '<':
	    PushString(buff, "&lt;");
	    break;
	case '>':
	    PushString(buff, "&gt;");
	    break;
	case '&':
	    PushString(buff, "&amp;");
	    break;
	case '\"':
	    PushString(buff, "&quot;");
	    break;
	default:
	    PushByte(buff, *p);
	}
    }
}

/* Given a string, replaces all instances of "oldpiece" with "newpiece".
**
** Modified this routine to eliminate recursion and to avoid infinite
** expansion of string when newpiece contains oldpiece.  --Byron Darrah
**
** 1998-11-17 (Daniel) modified to deal with any length strings and dynamic
** buffers.
*/

char *replace(char *string, char *oldpiece, char *newpiece)
{
    int str_index, newstr_index, oldpiece_index, end,
	new_len, old_len, cpy_len;
    char *c;

    struct Push buff;

    INIT_PUSH(buff);

    if ((c = (char *)strstr(string, oldpiece)) == NULL) {
	/* push the whole thing */
	PushString(&buff, string);
	RETURN_PUSH(buff);
    }

    new_len = strlen(newpiece);
    old_len = strlen(oldpiece);
    end = strlen(string) - old_len;
    oldpiece_index = c - string;

    newstr_index = 0;
    str_index = 0;
    while (str_index <= end && c != NULL) {
	/* Copy characters from the left of matched pattern occurence */
	cpy_len = oldpiece_index - str_index;
	PushNString(&buff, string + str_index, cpy_len);

	newstr_index += cpy_len;
	str_index += cpy_len;

	/* Copy replacement characters instead of matched pattern */

	PushString(&buff, newpiece);

	newstr_index += new_len;
	str_index += old_len;

	/* Check for another pattern match */
	if ((c = (char *)strstr(string + str_index, oldpiece)) != NULL)
	    oldpiece_index = c - string;
    }
    /* Copy remaining characters from the right of last matched pattern */
    PushString(&buff, string + str_index);

    RETURN_PUSH(buff);
}

char *replacechar(char *string, char old, char *new)
{
    struct Push buff;

    INIT_PUSH(buff);

    for (; *string; string++) {
	if (*string == old) {
	    PushString(&buff, new);
	}
	else
	    PushByte(&buff, *string);
    }

    RETURN_PUSH(buff);
}

/*
** Generates the mail command to use from the default mail command,
** the current recipient's email address, the current ID of the
** message, and the current subject.
**
** Returns an ALLOCATED string!
*/

char *makemailcommand(char *mailcommand, char *email, char *id, char *subject)
{
    int hasre;
    char *cp;
    char *tmpsubject;
    char *convsubj;

    char *newcmd = NULL;
    char *newcmd2;

    if (subject && isre(subject, NULL))
	hasre = 1;
    else
	hasre = 0;

    convsubj = convchars(subject);

    /* remade to deal with any-length strings */
    /* tmpsubject = maprintf("%s%s", (hasre) ? "" : "Re: ", (convsubj) ? convsubj : ""); */
    tmpsubject = maprintf("%s%s", (convsubj && !hasre) ? "Re: " : "", (convsubj) ? convsubj : "");


	if ((cp = strrchr(email, ' ')) != NULL)
	    *cp = '\0';

	newcmd = replace(mailcommand, "$TO", email);
	newcmd2 = replace(newcmd, "$ID", id);
	free(newcmd);

	newcmd = replace(newcmd2, "$SUBJECT", (tmpsubject) ? tmpsubject : "");
	free(newcmd2);

	newcmd2 = replacechar(newcmd, '%', "%25");
	free(newcmd);

	newcmd = replacechar(newcmd2, ' ', "%20");
	free(newcmd2);

	newcmd2 = replacechar(newcmd, '+', "%2B");
	free(newcmd);

	newcmd = newcmd2;	/* this is the new string */

	free(tmpsubject);

    free(convsubj);
    return newcmd;
}

/*
** RFC 1738
** Thus, only alphanumerics, the special characters "$-_.+!*'(),", and
** reserved characters used for their reserved purposes may be used
** unencoded within a URL.
**
**
**        //<user>:<password>@<host>:<port>/<url-path>
** 
** Some or all of the parts "<user>:<password>@", ":<password>",
** ":<port>", and "/<url-path>" may be excluded.  The scheme specific
** data start with a double slash "//" to indicate that it complies with
** the common Internet scheme syntax.
**
*/

char *parseemail(char *input,	/* string to parse */
		 char *mid,	/* message ID */
		 char *msubject)
{				/* message subject */
    char mailbuff[256];
    char mailaddr[MAILSTRLEN];
    char tempbuff[MAXLINE];
    char *ptr;
    char *lastpos = input;
    struct Push buff;
    
    char *at;

    if(set_spamprotect)
      at="_at_";
    else
      at="@";

    INIT_PUSH(buff);

    while (*input) {
	if ((ptr = strchr(input, '@'))) {
	    /* found a @ */
	    char *email = ptr - 1;
	    char content[2];
	    int backoff = ptr - input;	/* max */

#define VALID_IN_EMAIL_USERNAME   "a-zA-Z0-9_.%-"
#define VALID_IN_EMAIL_DOMAINNAME "a-zA-Z0-9.-"

	    /* check left side */
	    while (backoff) {
		if (sscanf
		    (email, "%1[" VALID_IN_EMAIL_USERNAME "]", content)) {
		    email--;
		    backoff--;
		}
		else
		    break;
	    }
	    if (email != ptr - 1) {	/* bigger chance this is an address */
		email++;
		if (sscanf
		    (ptr + 1, "%255[" VALID_IN_EMAIL_DOMAINNAME "]",
		     mailbuff)) {

		    /* a valid mail right-end */
		    if (lastpos < email) {
			PushNString(&buff, lastpos, email - lastpos);
		    }

                    msnprintf(mailaddr, sizeof(mailaddr),"%.*s%s%s", 
                            ptr-email, email, at, mailbuff);

		    if (valid_root_domain(mailaddr)) {
			char *mailcmd = makemailcommand(set_bodymailcommand,
							mailaddr, mid,
							msubject);
			msnprintf(tempbuff, sizeof(tempbuff),
				  "<a href=\"%s\">%.*s%s%s</a>", mailcmd,
				  ptr - email, email, at, mailbuff);

			free(mailcmd);

			PushString(&buff, tempbuff);

			input = ptr + strlen(mailbuff) + 1;
			lastpos = input;
			continue;
		    }
		    else {	/* bad address */
			PushString(&buff, mailaddr);
			input = ptr + strlen(mailbuff) + 1;
			lastpos = input;
			continue;
		    }
		}
	    }
	    /* no address, continue from here */
	    input = ptr + 1;
	    continue;
	}
	else
	    input = strchr(input, '\0');
    }
    if (lastpos < input) {
	PushNString(&buff, lastpos, input - lastpos);
    }
    RETURN_PUSH(buff);
}

/*
** Returns the allocated and converted string.
**
** This function is run on each and every body line, so it pays to make it
** run quickly.
**/

static char *url[] = {
    "http://",
    "https://",
    "news:",
    "ftp://",
    "file://",
    "gopher://",
    "nntp://",
    "wais://",
    "telnet://",
    "prospero://",
/* "mailto:", *//* Can't have mailto: as it will be converted twice */
    NULL
};


char *parseurl(char *input)
{
    struct Push buff;		/* output buffer */
    char urlbuff[256];
    char tempbuff[MAXLINE];
    char *inputp;
    char *match[sizeof(url) / sizeof(char **)];
    int first;

    if (!input || !*input)
	return NULL;

    if (!strstr(input, "://")) {
	/*
	 * All our protocol prefixes have this "//:" substring in them. Most
	 * of the lines we process don't have any URL. Let's not spend any
	 * time looking for URLs in lines that we can prove cheaply don't
	 * have any; it will be a big win for average input if we follow
	 * this trivial heuristic. 
	 */
	return convchars(input);
    }
    INIT_PUSH(buff);

    /*
     * Iterate on possible URLs in the input string. There might 
     * be more than one! 
     */

    inputp = input;
    first = TRUE;

    while (1) {
	char *leftmost = NULL;
	char **up;
	char *thisprotocol = NULL;
	char *p;

	/* Loop on protocol prefixes, searching for the leftmost. */

	for (up = url; *up; up++) {
	    int i;

	    if (first) {
		/* 
		 * we haven't looked for this one yet, so do so and remember
		 * any match we find 
		 */
		match[up - url] = p = strcasestr(inputp, *up);
	    }
	    else if (match[i = up - url]) {
		/*
                 * we looked for it before, and we found it before, so if the
		 * match is to the right of the input we have processed so
		 * far, we can simply reuse it; else, search again. 
		 */
		if (match[i] <= inputp)
		    match[i] = p = strcasestr(inputp, *up);
		else
		    p = match[i];
	    }
	    else {
		/*
                 * we looked for it before, and we didn't find it before, so
		 * we aren't bloody likely to find it this time; don't waste
		 * time looking again 
                 */
		continue;
	    }

	    if (p) {
		/*
                 * Found a protocol prefix. We want the leftmost such, 
                 * so note the match and keep looking for other protocols. 
                 */
		if (!leftmost || p < leftmost) {
		    leftmost = p;
		    thisprotocol = *up;
		}
	    }
	}

	first = FALSE;

	if (leftmost) { /* we found at least one protocol prefix */
	    int accepted = FALSE;

	    /* 
	     * all the charaters between the position where we started
             * looking for a protocol prefix and the protocol prefix
             * need to be checked for character translations 
	     */

	    translatechars(inputp, leftmost-1, &buff);
	    inputp = leftmost + strlen(thisprotocol);

	    if (sscanf(inputp, "%255[^] )>\"\'\n[\t\\]", urlbuff)) {
	        char *r;
	
		/* 
		 * A valid url: up to 255 characters in a run containing legal
		 * URL characters. But let's nibble off any punctuation other
		 * than slashes at the end, because they are not likely part
		 * of the URL. E.g. a trailing comma. 
		 */

		for(r = strchr(urlbuff, '\0') - 1; 
		    *r != '/' && ispunct(*r) && r > urlbuff;
		    r--);
		if(r++ > urlbuff) {

		    /* 
		     * there should be something left in the URL after we chew
		     * away the trailing punctuation if we are going to call it
		     * valid 
		     */

		    accepted = TRUE;
		    if(*r) {
		        *r = '\0';
		    }
		}
	    }
	    if(accepted) {
	        msnprintf(tempbuff, sizeof(tempbuff),
			  "<a href=\"%s%s\">%s%s</a>",
			  thisprotocol, urlbuff, thisprotocol, urlbuff);
		PushString(&buff, tempbuff); /* append the tag buffer */
		inputp += strlen(urlbuff);
	    } else {
	        PushString(&buff, thisprotocol);
	    }
	} else {
	    /*
	     * no prospects found; translate the characters in the rest of
	     * the string and return 
	     */
	    translatechars(inputp, strchr(inputp, '\0') - 1, &buff);
	    break;
	}
    }
    RETURN_PUSH(buff);
}
