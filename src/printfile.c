#include "hypermail.h"
#include "setup.h"
#include "print.h"
#include "printfile.h"

/*
** printfile - print html header/footer file and fill in values 
**             substituting for magic cookies. Return allocated
**             memory with the header/footer filled in.
**
**      Substitution cookies supported
**
**              %% - '%' character
**              %~ - storage directory
**              %a - Other Archives URL
**              %b - About Archive URL
**              %B - Body line specified
**              %c - Charset META TAG - Not valid on index pages
**              %e - email addr of message author - Not valid on index pages
**              %f - file name of the HTML document
**              %g - date and time archive generated
**              %h - HMURL
**              %i - Message-id - Not valid on index pages
**              %l - archive label
**              %m - Mailto address
**              %p - PROGNAME
**              %s - Subject of message or Index Title
**              %v - VERSION
**              %u - Expanded version link (HMURL,PROGNAME,VERSION)
**              %S - Subject META TAG - Not valid on index pages
**              %A - Author META TAG - Not valid on index pages
**              \n - newline character
**              \t - tab character
**
*/

int printfile(FILE *fp, char *format, char *label, char *subject,
	      char *dir, char *name, char *email, char *message_id,
	      char *charset, char *filename)
{
    register char *cp;
    register char *aptr;
    char c;
    char *ptr;

    aptr = format;

    while ((c = *aptr++)) {
	if (c == '\\') {
	    switch (*aptr++) {
	    case 'n':		/* Add the \n character */
		putc('\n', fp);
		continue;
	    case 't':		/* Add the \t character */
		putc('\t', fp);
		continue;
	    default:
		break;
	    }			/* end switch */
	}
	else if (c == '%') {
            char nextone = *aptr++;
	    switch (nextone) {
	    case '%':		/* Add the % character */
		putc('%', fp);
		continue;
	    case '~':		/* %~ - storage directory */
		for (cp = dir; *cp; cp++)
		    putc(*cp, fp);
		continue;
	    case 'A':		/* %e - email address of message author */
		if (email && name) {
		    fprintf(fp,
			    "<META NAME=\"Author\" CONTENT=\"%s (%s)\">",
			    name, email);
		}
		continue;
	    case 'a':		/* %a - Other Archives URL */
		if (set_archives) {
		    for (cp = set_archives; *cp; cp++)
			putc(*cp, fp);
		}
		continue;
	    case 'B':		/* %B - BODY specified */
		if (set_htmlbody) {
		    for (cp = set_htmlbody; *cp; cp++)
			putc(*cp, fp);
		}
		continue;
	    case 'b':		/* %b - About this archive URL */
		if (set_about) {
		    for (cp = set_about; *cp; cp++)
			putc(*cp, fp);
		}
		continue;
	    case 'c':
		if (charset && *charset) {
		    /* only output this if we have a charset */
		    fprintf(fp, "<META HTTP-EQUIV=\"Content-Type\""
			    " CONTENT=\"text/html; charset=%s\">\n",
			    charset);
		}
		continue;

	    case 'e':		/* %e - email address of message author */
		if (email) {
		    for (cp = email; *cp; cp++)
			putc(*cp, fp);
		}
		continue;
	    case 'f':		/* %f - file name */
		if (filename) {
		    for (cp = filename; *cp; cp++)
			putc(*cp, fp);
		}
		continue;
	    case 'g':		/* %g - date and time archive generated */
		for (cp = getlocaltime(); *cp; cp++)
		    putc(*cp, fp);
		continue;
	    case 'h':		/* %h - Hypermail Resource Center */
		for (cp = HMURL; *cp; cp++)
		    putc(*cp, fp);
		continue;
	    case 'i':		/* %m - Message-ID of message */
		if (message_id) {
		    for (cp = message_id; *cp; cp++)
			putc(*cp, fp);
		}
		continue;
	    case 'l':		/* %l - Archive label  */
		for (cp = label; *cp; cp++)
		    putc(*cp, fp);
		continue;
	    case 'm':		/* %m - mailto */
		for (cp = set_mailto; *cp; cp++)
		    putc(*cp, fp);
		continue;
	    case 'p':		/* %p - PROGNAME */
		for (cp = PROGNAME; *cp; cp++)
		    putc(*cp, fp);
		continue;
	    case 's':		/* %s - Subject of message or Index Title */
		for (ptr = cp = convchars(subject); *cp; cp++)
		    putc(*cp, fp);
		free(ptr);
		continue;
	    case 'S':		/* %s - Subject of message or Index Title */
		fprintf(fp, "<META NAME=\"Subject\" CONTENT=\"%s\">",
			cp = convchars(subject));
		free(cp);
		continue;
	    case 'v':		/* %v - VERSION */
		for (cp = VERSION; *cp; cp++)
		    putc(*cp, fp);
		continue;
	    case 'u':		/* %u - Expanded Version link */
		fprintf(fp, "<A HREF=\"%s\">%s %s</A>",
			HMURL, PROGNAME, VERSION);
		continue;
	    default:
		putc('%', fp);
		putc(nextone, fp);
		continue;
	    }			/* end switch */
	}
	putc(c, fp);
    }				/* end while */

    fflush(fp);
    return (0);
}

/*
** Prints the standard page header 
*/

void print_main_header(FILE *fp, bool index_header, char *label, char *name,
		       char *email, char *subject, char *charset,
		       char *filename)
{
    char *title;
    char *rp;

    fprintf(fp,
	    "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0 Transitional//EN\"\n"
	    "                      "
	    "\"http://www.w3.org/TR/REC-html40/loose.dtd\">\n");
    fprintf(fp, "<HTML>\n");
    fprintf(fp, "<HEAD>\n");

    if (charset && *charset) {
	/* charset info "as early as possible within the HEAD of the document"
	 */
	fprintf(fp, "<META HTTP-EQUIV=\"Content-Type\""
		" CONTENT=\"text/html; charset=%s\">\n", charset);
    }

    /* 
     * Assure the title is not longer than 64 characters as is
     * recommended by the HTML specs. Also, strip off any trailing
     * whitespace in TITLE so weblint is happy. 
     */

    title = maprintf("%s: %s", label, rp = convchars(subject));
    free(rp);

    if (strlen(title) > TITLESTRLEN) {
	rp = title + (TITLESTRLEN - 1);
	*rp-- = '\0';
    }
    else
	rp = title + (strlen(title) - 1);

    while (isspace(*rp))
	*rp-- = '\0';

    fprintf(fp, "<TITLE>%s</TITLE>\n", title);
    free(title);

    if (name && email)
	fprintf(fp, "<META NAME=\"Author\" CONTENT=\"%s (%s)\">\n",name,email);
    fprintf(fp, "<META NAME=\"Subject\" CONTENT=\"%s\">\n", rp =
	    convchars(subject));
    free(rp);
    if (use_mailto)
	fprintf(fp, "<LINK REV=\"made\" HREF=\"mailto:%s\">\n", set_mailto);
    /* print the css url according to the type of header */
    if (index_header && set_icss_url && *set_icss_url)
        fprintf(fp, "<LINK REL=\"Stylesheet\" HREF=\"%s\">\n", set_icss_url);
    else if (!index_header && set_mcss_url && *set_mcss_url)
      fprintf(fp, "<LINK REL=\"Stylesheet\" HREF=\"%s\">\n", set_mcss_url);

    fprintf(fp, "</HEAD>\n");

    if (set_htmlbody)
	fprintf(fp, "%s\n", set_htmlbody);
    else
	fprintf(fp, "<BODY BGCOLOR=\"#FFFFFF\" TEXT=\"#000000\">\n");
}

/*
** Prints the header for an index page.
*/

void print_msg_header(FILE *fp, char *label, char *subject,
		      char *dir, char *name, char *email, char *msgid,
		      char *charset, char *filename)
{
    char *ptr;
    if (mhtmlheaderfile)
	printfile(fp, mhtmlheaderfile, set_label, subject, set_dir, name, 
		  email, msgid, charset, filename);
    else {
	print_main_header(fp, FALSE, set_label, name, email, subject,
			  charset, filename);
#if 0 /* JK modified this */       
	fprintf(fp, "<H1 ALIGN=CENTER>%s</H1>\n",
		ptr = convchars(subject));
	free(ptr);
#endif
	fprintf(fp, "<H1>%s</H1>\n",
		ptr = convchars(subject));
	free(ptr);
#if 0 /* JK: and removed this as it looked a bit strange */
	if (!set_usetable)
	    fprintf(fp, "<HR NOSHADE>\n<P>\n");
#endif
    }
}

/*
** Prints the header for the by_date index page.
*/

void print_index_header(FILE *fp, char *label, char *dir, char *subject,
			char *filename)
{
    if (ihtmlheaderfile)
	printfile(fp, ihtmlheaderfile, label, subject, dir, NULL, NULL,
		  NULL, NULL, filename);
    else {
	print_main_header(fp, TRUE, label, NULL, NULL, subject, NULL, NULL);
	fprintf(fp, "<H1 ALIGN=CENTER>%s<BR>%s</H1>\n", label, subject);
#if 0 /*@@ JK: removed it */	
	if (!set_usetable)
	    fprintf(fp, "<HR NOSHADE>\n");
#endif
    }
}

/*
** Prints the HTML page footer.
*/

void printfooter(FILE *fp, char *htmlfooter, char *label, char *dir,
		 char *subject, char *filename)
{
    printcomment(fp, "trailer", "footer");

    if (htmlfooter)
	printfile(fp, htmlfooter, label, subject,
		  dir, NULL, NULL, NULL, NULL, filename);
    else {
	if (set_showhr && !set_usetable)
	    fprintf(fp, "<HR NOSHADE>\n");
	fprintf(fp, "<P>\n<SMALL>\n<EM>\n");
	fprintf(fp, "%s ", lang[MSG_ARCHIVE_GENERATED_BY]);
	fprintf(fp, "<A HREF=\"%s\">%s %s</A> \n", HMURL, PROGNAME, VERSION);
	fprintf(fp, ": <EM>%s</EM>\n", getlocaltime());
	fprintf(fp, "</EM>\n</SMALL>\n");
	fprintf(fp, "</BODY>\n</HTML>\n");
    }
}
