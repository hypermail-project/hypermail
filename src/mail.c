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
    printf("<HTML>\n");
    printf("<HEAD>\n");
    printf("<TITLE>Email Gateway Error</TITLE>\n");
    printf("</HEAD>\n");
    printf("<BODY BGCOLOR=\"#ffffff\" TEXT=\"#000000\">\n");
    printf("<H1 ALIGN=CENTER>Email Gateway Error</H1>\n");
    printf("This gateway program encountered an error:\n<P>\n");
    printf("<CODE>%s</CODE>\n", s);
    printf("</BODY>\n</HTML>\n");

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
    printf("<HTML>\n");
    printf("<HEAD>\n");
    printf("<TITLE>Email Gateway</TITLE>\n");
    printf("</HEAD>\n");
    printf("<BODY BGCOLOR=\"#ffffff\" TEXT=\"#000000\">\n");

#if DEBUGGING
    dump_cgi_info(ci);
    printf("<BR>inet_addr %s\n",
	   addr_failure == 0 ? "Succeeded" : "Failed");
    printf("<BR>gethostbyaddr %s\n",
	   gethostbyaddr_failure == 0 ? "Succeeded" : "Failed");
    printf("<BR>hostname %s\n", hostn);
#endif

    printf("<H1 ALIGN=CENTER>Email Gateway</H1>\n");
    printf
	("<P ALIGN=CENTER><EM>Send an email message by filling in the form below.<BR>Make sure your user name is specified correctly on the From: line.</EM></P>\n");

    printf("<DIV ALIGN=CENTER>\n");
    printf("<FORM METHOD=POST ACTION=\"%s\">\n", CGINAME);
    printf("<PRE>\n");
    printf("<STRONG>     To:</STRONG> ");
    printf("<INPUT SIZE=45 NAME=\"to\" VALUE=\"%s\">\n", to);
    printf("<STRONG>   From:</STRONG> ");
    printf("<INPUT SIZE=45 NAME=\"from\" VALUE=\"%s\">\n", from);
    printf("<STRONG>Subject:</STRONG> ");
    printf("<INPUT SIZE=45 NAME=\"subject\" VALUE=\"%s\">\n", subject);
    if (replyto != NULL && replyto[0] != '\0') {
	printf("<STRONG>In reply to:</STRONG> ");
	printf("<INPUT SIZE=45 NAME=\"replyto\" VALUE=\"%s\">\n", replyto);
    }
    printf("\n");

    printf("<TEXTAREA NAME=\"body\"");
    printf("rows=%d cols=%d>%s</TEXTAREA>\n", ROWS, COLS, body);
    printf("</PRE>\n");

    printf("<INPUT TYPE=submit VALUE=\"    Send Email    \">     ");
    printf("<INPUT TYPE=reset value=\"    Reset Form    \">");
    printf("</DIV>\n");
    printf("</FORM>\n");
    printf("</BODY>\n</HTML>\n");
}

void cgi_main(cgi_info *ci)
{
    form_entry *parms, *p;
    char *from, *to, *subject, *replyto, *body, *host;
    char tmpstr[MAXLEN];
    FILE *f;

    /* Only correct remote adresses are valid! */
    if (ci == 0 || ci->remote_addr == 0)
        return;

    print_mimeheader("text/html");

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

    strcpy(tmpstr, lookupnumaddr(ci->remote_addr));
#ifdef DEBUGGING
    strcpy(hostn, tmpstr);
#endif
    if (strchr(tmpstr, '.'))
	host = strsav(tmpstr);
    else
	host = strsav(ci->remote_addr);

    sprintf(tmpstr, "%s@%s", (ci->remote_user && *(ci->remote_user)) ?
	    ci->remote_user : "", host);

    if (from == NULL || from[0] == '\0')
	from = strsav(tmpstr);

    switch (mcode(ci)) {

    case MCODE_HEAD:
	return;

    case MCODE_GET:
	printform(to, from, subject, body, replyto, ci);
	break;

    case MCODE_POST:
	if (from == NULL || from[0] == '\0' || from[0] == '@' ||
	    from[strlen(from) - 1] == '@' || !strchr(from, '@'))
	    progerr("Invalid <STRONG>From:</STRONG> address.");

	if (to == NULL || to[0] == '\0' || to[0] == '@' ||
	    to[strlen(to) - 1] == '@' || !strchr(to, '@'))
	    progerr("Invalid <STRONG>To:</STRONG> address.");

	if (subject == NULL || subject[0] == '\0')
	    progerr("The subject field is blank.");

	if (body == NULL || body[0] == '\0')
	    progerr("No message has been written.");

	sprintf(tmpstr, "%s -t", SENDMAIL);

	if ((f = popen(tmpstr, "w")) != NULL) {
	    fprintf(f, "From: %s\nTo: %s\n", from, to);

	    if (replyto != NULL && replyto[0] != '\0')
		fprintf(f, "In-Reply-To: <%s>\n", replyto);

	    fprintf(f, "Subject: %s\nX-Sender: %s\n\n%s", subject, host,
		    body);
	    pclose(f);

	    printf("<HTML>\n");
	    printf("<HEAD>\n");
	    printf("<TITLE>Email Gateway Response</TITLE>\n");
	    printf("</HEAD>\n");
	    printf("<BODY BGCOLOR=\"#ffffff\" TEXT=\"#000000\">\n");
	    printf("<H1 ALIGN=CENTER>Your message has been sent.</H1>\n");

	    printf("The following message has been sent to ");
	    printf("<STRONG>%s</STRONG>:\n<P>\n<HR NOSHADE>\n\n", to);

	    printf("<BLOCKQUOTE>\n");
	    printf("<BR>From: %s\n", from);
	    printf("<BR>To: %s\n", to);

	    if (replyto != NULL && replyto[0] != '\0')
		printf("<BR>In-Reply-To: &lt;%s&gt;\n", replyto);

	    printf("<BR>Subject: %s\n", subject);
	    printf("<BR>X-Sender: %s\n", host);
	    printf("<P>\n%s\n", body);

	    printf("</BLOCKQUOTE>\n");
	    printf("<P>\n<HR NOSHADE>\n");
	    printf("</BODY>\n</HTML>\n");
	}
	else
	    progerr("The message was not sent - sendmail error.");
	break;

    default:
	sprintf(tmpstr, "Unrecognized method used: \"%s\".",
		ci->request_method);
	progerr(tmpstr);
    }
    free_form_entries(parms);
}
