/* 
** Copyright (C) 1994, 1995 Enterprise Integration Technologies Corp.
**         VeriFone Inc./Hewlett-Packard. All Rights Reserved.
** Kevin Hughes, kev@kevcom.com 3/11/94
** Jay Weber, weber@eit.com
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
** mail.c 
** This tiny CGI program constructs an email form (GET) and
** sends the contents (POST).
*/

#include "mail.h"
#include "trio.h"

extern int strcasecmp(const char *, const char *);
extern FILE *popen(const char *, const char *);
extern int pclose (FILE *);

/*
** Malloc() out a string, give it to whoever asked for it.
*/

char *strsav(char *s)
{
    char *p;

    if (NULL == s)
        s = "";                 /* Daniel's kludge to survive better */

    p = (char *)malloc((size_t)(strlen(s) + 1));
    strcpy(p, s);
    return p;
}

static void progerr(const char *s)
{
    printf("<html>\n");
    printf("<head>\n");
    printf("<title>Email Gateway Error</title>\n");
    printf("</head>\n");
    printf("<body bgcolor=\"#ffffff\" text=\"#000000\">\n");
    printf("<h1 class=\"center\">Email Gateway Error</h1>\n");
    printf("This gateway program encountered an error:\n<p>\n");
    printf("<code>%s</code>\n", s);
    printf("</body>\n</html>\n");

    exit(0);
}

char *lookupnumaddr(char *numaddress)
{
    unsigned long addr;
    struct hostent *he;

    if (((addr = inet_addr(numaddress)) == -1) ||
	((he = gethostbyaddr((char *)&addr, sizeof(addr), AF_INET)) ==
	 NULL)) return numaddress;

    return he->h_name;
}

void printform(char *to, char *from, char *subject,
	       char *body, char *replyto, cgi_info *ci)
{
    printf("<html>\n");
    printf("<head>\n");
    printf("<title>Email Gateway</title>\n");
    printf("</head>\n");
    printf("<body bgcolor=\"#ffffff\" text=\"#000000\">\n");

#if DEBUGGING
    dump_cgi_info(ci);
    printf("<br>inet_addr %s\n",
	   addr_failure == 0 ? "Succeeded" : "Failed");
    printf("<br>gethostbyaddr %s\n",
	   gethostbyaddr_failure == 0 ? "Succeeded" : "Failed");
    printf("<br>hostname %s\n", hostn);
#endif

    printf("<h1 class=\"center\">Email Gateway</h1>\n");
    printf
	("<p align=\"center\"><em>Send an email message by filling in the form below.<br>Make sure your user name is specified correctly on the From: line.</em></p>\n");

    printf("<div class=\"center\">\n");
    printf("<form method=\"post\" action=\"%s\">\n", CGINAME);
    printf("<pre>\n");
    printf("<strong>     To:</strong> ");
    printf("<input size=\"45\" name=\"to\" value=\"%s\">\n", to);
    printf("<strong>   From:</strong> ");
    printf("<input size=\"45\" name=\"from\" value=\"%s\">\n", from);
    printf("<strong>Subject:</strong> ");
    printf("<input size=\"45\" name=\"subject\" value=\"%s\">\n", subject);
    if (replyto != NULL && replyto[0] != '\0') {
	printf("<strong>In reply to:</strong> ");
	printf("<input size=\"45\" name=\"replyto\" value=\"%s\">\n", replyto);
    }
    printf("\n");

    printf("<textarea name=\"body\"");
    printf("rows=\"%d\" cols=\"%d\">%s</textarea>\n", ROWS, COLS, body);
    printf("</pre>\n");

    printf("<input type=\"submit\" value=\"    Send Email    \">     ");
    printf("<input type=\"reset\" value=\"    Reset Form    \">");
    printf("</div>\n");
    printf("</form>\n");
    printf("</body>\n</html>\n");
}

void cgi_main(cgi_info *ci)
{
    form_entry *parms, *p;
    char *from, *to, *subject, *replyto, *body, *host;
    char *tmpstr;
    char tmpstr2[256];
    FILE *f;

    /* Only correct remote adresses are valid! */
    if (ci == 0 || ci->remote_addr == 0)
        return;

    print_mimeheader("text/html");

#ifndef I_LIKE_SPAMMERS
    /* This program has been set to be useless by default because it
     * is probably easy for spammers to use as an open relay. It also
     * has problems with enabling malicious use of JavaScript and
     * CRLF Injection. Don't enable code below the #else unless you
     * are sure that you trust the people who have access to the web
     * page it produces.
     */
    printf("<html>\n");
    printf("<head>\n");
    printf("<title>Email Gateway</title>\n");
    printf("</head>\n");
    printf("<body bgcolor=\"#ffffff\" text=\"#000000\">\n");
    printf("This page has been disabled due to potential abuse by spammers.\n");
    printf("</body>\n</html>\n");
    return;
#else
    from = NULL;
    to = NULL;
    subject = NULL;
    body = NULL;
    replyto = NULL;

    parms = get_form_entries(ci);

    if (parms) {
	for (p = parms; p; p = p->next) {
	    if (!strcasecmp(p->name, "from"))
		from = p->val;
	    else if (!strcasecmp(p->name, "to"))
		to = p->val;
	    else if (!strcasecmp(p->name, "subject"))
		subject = p->val;
	    else if (!strcasecmp(p->name, "body"))
		body = p->val;
	    else if (!strcasecmp(p->name, "replyto"))
		replyto = p->val;
	}
    }

    tmpstr = lookupnumaddr(ci->remote_addr);
#ifdef DEBUGGING
    strcpy(hostn, tmpstr);
#endif
    if (strchr(tmpstr, '.'))
	host = strsav(tmpstr);
    else
	host = strsav(ci->remote_addr);

    trio_snprintf(tmpstr2, sizeof(tmpstr2), "%s@%s",
		  (ci->remote_user && *(ci->remote_user)) ?
		  ci->remote_user : "", host);

    if (from == NULL || from[0] == '\0')
	from = strsav(tmpstr2);

    switch (mcode(ci)) {

    case MCODE_HEAD:
	return;

    case MCODE_GET:
	printform(to, from, subject, body, replyto, ci);
	break;

    case MCODE_POST:
	if (from == NULL || from[0] == '\0' || from[0] == '@' ||
	    from[strlen(from) - 1] == '@' || !strchr(from, '@'))
	    progerr("Invalid <strong>From:</strong> address.");

	if (to == NULL || to[0] == '\0' || to[0] == '@' ||
	    to[strlen(to) - 1] == '@' || !strchr(to, '@'))
	    progerr("Invalid <strong>To:</strong> address.");

	if (subject == NULL || subject[0] == '\0')
	    progerr("The subject field is blank.");

	if (body == NULL || body[0] == '\0')
	    progerr("No message has been written.");

	trio_snprintf(tmpstr2, sizeof(tmpstr2), "%s -t", SENDMAIL);

	if ((f = popen(tmpstr2, "w")) != NULL) {
	    fprintf(f, "From: %s\nTo: %s\n", from, to);

	    if (replyto != NULL && replyto[0] != '\0')
		fprintf(f, "In-Reply-To: <%s>\n", replyto);

	    fprintf(f, "Subject: %s\nX-Sender: %s\n\n%s", subject, host,
		    body);
	    pclose(f);

	    printf("<html>\n");
	    printf("<head>\n");
	    printf("<title>Email Gateway Response</title>\n");
	    printf("</head>\n");
	    printf("<body bgcolor=\"#ffffff\" text=\"#000000\">\n");
	    printf("<h1 class=\"center\">Your message has been sent.</h1>\n");

	    printf("The following message has been sent to ");
	    printf("<strong>%s</strong>:\n<p>\n<hr>\n\n", to);

	    printf("<blockquote>\n");
	    printf("<br>From: %s\n", from);
	    printf("<br>To: %s\n", to);

	    if (replyto != NULL && replyto[0] != '\0')
		printf("<br>In-Reply-To: &lt;%s&gt;\n", replyto);

	    printf("<br>Subject: %s\n", subject);
	    printf("<br>X-Sender: %s\n", host);
	    printf("<p>\n%s\n", body);

	    printf("</blockquote>\n");
	    printf("<p>\n<hr>\n");
	    printf("</body>\n</html>\n");
	}
	else
	    progerr("The message was not sent - sendmail error.");
	break;

    default:
	trio_snprintf(tmpstr2, sizeof(tmpstr2),
		      "Unrecognized method used: \"%s\".", ci->request_method);
	progerr(tmpstr2);
    }
    free_form_entries(parms);
#endif
}
