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

#include "hypermail.h"
#include "setup.h"
#include "struct.h"
#include "printfile.h"
#include "print.h"
#include "parse.h"
#include "txt2html.h"
#include "finelink.h"

#include "threadprint.h"

#ifdef CHANGE_12DEC2000_BC
#include "mprintf.h"
#include "proto.h"
#endif

static char *indextypename[NO_INDEX];
static char *indextypefilename[NO_INDEX];

#ifdef GDBM

#include "gdbm.h"

/*
** store a single message summary to an already open-for-write GDBM index
**/

int togdbm(void *gp, int num, char *name, char *email,
	   char *date, char *msgid, char *subject, char *inreply,
	   char *fromdate, char *charset, char *isodate,
	   char *isofromdate)
{
  datum key;
  datum content;
  char *buf;
  char *dp;
  int rval;

  key.dsize = sizeof(num); /* the key is the message number */
  key.dptr = (char *) &num;

  /* malloc() a string long enough for our data */

  if(!(buf = (char *) malloc(
			     (name ? strlen(name) : 0) + 
			     (email ? strlen(email) : 0) + 
			     (date ? strlen(date) : 0) +
			     (msgid ? strlen(msgid) : 0) + 
			     (subject ? strlen(subject) : 0) + 
			     (inreply ? strlen(inreply) : 0) +
			     (fromdate ? strlen(fromdate) : 0) +
			     (charset ? strlen(charset) : 0) +
			     (isodate ? strlen(isodate) : 0) +
			     (isofromdate ? strlen(isofromdate) : 0) +
			     10))) {
    return -1;
  }
  strcpy(dp = buf, fromdate ? fromdate : "");
  dp += strlen(dp) + 1;
  strcpy(dp, date ? date : "");
  dp += strlen(dp) + 1;
  strcpy(dp, name ? name : "");
  dp += strlen(dp) + 1;
  strcpy(dp, email ? email : "");
  dp += strlen(dp) + 1;
  strcpy(dp, subject ? subject : "");
  dp += strlen(dp) + 1;
  strcpy(dp, msgid ? msgid : "");
  dp += strlen(dp) + 1;
  strcpy(dp, inreply ? inreply : "");
  dp += strlen(dp) + 1;
  strcpy(dp, charset ? charset : "");
  dp += strlen(dp) + 1;
  strcpy(dp, isofromdate ? isofromdate : "");
  dp += strlen(dp) + 1;
  strcpy(dp, isodate ? isodate : "");
  dp += strlen(dp) + 1;
  content.dsize = dp - buf;
  content.dptr = buf; /* the value is in this string */
  rval = gdbm_store((GDBM_FILE) gp, key, content, GDBM_REPLACE);
  free(buf);
  return !rval;

} /* end togdbm() */

#endif


/* Uses threadlist to find the next message after
 * msgnum in the thread containing msgnum.
 * Returns NULL if there are no more messages in 
 * the thread.
 */
struct emailinfo *nextinthread(int msgnum)
{
    struct reply *rp = threadlist;

    for (rp = threadlist; (rp != NULL) && (rp->msgnum != msgnum);
	 rp = rp->next) {
	;
    }

    if (rp == NULL) {		/* msgnum not found in threadlist */
	return NULL;
    }

    rp = rp->next;

    if ((rp == NULL) || (rp->frommsgnum == -1)) {	
        /*end of thread - no next msg */
	return NULL;
    }
    return rp->data;
}


/*
** Output a menu line with hyperlinks for table display
**
** All parameters are inputs only.  Parameters:
**   fp       : HTML output file pointer
**   idx      : Type of page this menu is for.
**   archives : "" or else the URL of more hypermail archives.
**   currentid: "" or else the id of the "current" message.
**   cursub   : "" or else the subject of the "current" message.
**   pos      : Called at the top or bottom of the page.
*/

void fprint_menu(FILE *fp, mindex_t idx, char *archives,
		 char *currentid, char *cursub, int pos)
{
    char *ptr;
    fprintf(fp,
	    "<div align=\"center\">\n<table border=\"2\" width=\"100%%\">\n<tr>\n");

    if (set_mailcommand) {
	if (set_hmail) {
	    ptr = makemailcommand(set_newmsg_command, set_hmail,
				  currentid, cursub);
            if (strcmp(ptr, "NONE")!=0)
	         fprintf(fp, "<th><a href=\"%s\">%s</a></th>\n",
			 ptr ? ptr : "", lang[MSG_NEW_MESSAGE]);
	    if (ptr)
		free(ptr);

	    if ((currentid != NULL && currentid[0] != '\0') ||
		(cursub != NULL && cursub[0] != '\0')) {

		ptr = makemailcommand(set_replymsg_command, set_hmail,
				      currentid, cursub);
		if (strcmp(ptr, "NONE")!=0)
		    fprintf(fp, "<th><a href=\"%s\">%s</a></th>\n",
			    ptr ? ptr : "", lang[MSG_REPLY]);
		if (ptr)
		    free(ptr);
	    }
	}
    }

    if (set_about && *set_about)
	fprintf(fp, "<th><a href=\"%s\">%s</a></th>\n", set_about,
		lang[MSG_ABOUT_THIS_LIST]);

    if (idx != NO_INDEX && !set_reverse) {
	if (pos == PAGE_TOP)
	    fprintf(fp, "<th><a href=\"#end\">%s</a></th>\n",
		    lang[MSG_END_OF_MESSAGES]);
	else
	    fprintf(fp, "<th><a name=\"end\" href=\"#\">%s</a></th>\n",
		    lang[MSG_START_OF_MESSAGES]);
    }

    if (idx != DATE_INDEX && show_index[DATE_INDEX])
	fprintf(fp, "<th><a href=\"%s\">%s</a></th>\n", datename,
		lang[MSG_DATE_VIEW]);

    if (idx != THREAD_INDEX && show_index[THREAD_INDEX])
	fprintf(fp, "<th><a href=\"%s\">%s</a></th>\n", thrdname,
		lang[MSG_THREAD_VIEW]);

    if (idx != SUBJECT_INDEX && show_index[SUBJECT_INDEX])
	fprintf(fp, "<th><a href=\"%s\">%s</a></th>\n", subjname,
		lang[MSG_SUBJECT_VIEW]);

    if (idx != AUTHOR_INDEX && show_index[AUTHOR_INDEX])
	fprintf(fp, "<th><a href=\"%s\">%s</a></th>\n", authname,
		lang[MSG_AUTHOR_VIEW]);

#ifdef CHANGE_12DEC2000_BC
    if (set_attachmentsindex) {
	if (idx != ATTACHMENT_INDEX) {
	    fprintf(fp, "<th><a href=\"%s\">%s</a></th>\n", attname,
		lang[MSG_ATTACHMENT_VIEW]);
	}
    }
#endif

    if (archives && *archives)
	fprintf(fp, "<th><a href=\"%s\">%s</a></th>\n", archives,
		lang[MSG_OTHER_MAIL_ARCHIVES]);

    fprintf(fp, "</tr>\n</table>\n</div>\n");
}

/*----------------------------------------------------------------------------*/

void fprint_summary(FILE *fp, int pos, long first_d, long last_d, int num)
{
    fprintf(fp, "<div align=\"center\">\n");
    fprintf(fp, "<table>\n");

    if (pos == PAGE_TOP) {
        fprintf(fp, "<tr>\n<th colspan=\"4\">%d %s</th>\n</tr>\n",
	    num, lang[MSG_MESSAGES]);
        fprintf(fp, "<tr>\n  <th>%s:</th><td><em>%s</em></td>\n",
	        lang[MSG_STARTING], getdatestr(first_d));
        fprintf(fp, "  <th>%s:</th><td><em>%s</em></td>\n</tr>\n",
	        lang[MSG_ENDING], getdatestr(last_d));
    }
    else { /* bottom of page */
       fprintf(fp,"<tr><th><a name=\"end\">%s: </a></th><td><em>%s</em></td>\n",
                lang[MSG_LAST_MESSAGE_DATE], getdatestr(last_d));
        fprintf(fp, "<th>%s: </th><td><em>%s</em></td>\n",
                lang[MSG_ARCHIVED_ON], getlocaltime());
    }
    fprintf(fp, "</table>\n</div>\n");
}

/*----------------------------------------------------------------------------*/

void print_index_header_links(FILE *fp, mindex_t called_from, 
			      long startdatenum, long enddatenum,
			      int amountmsgs)
{
    /* 
     * Print out the links for
     * 
     *      About this archive 
     *      Other mail archives 
     *      Most recent messages 
     *      Messages sorted by: [ date ][ subject ][ author ] 
     *
     * as appropriate.
     */
    char *ptr;

    if (set_mailcommand && set_hmail) {
	ptr = makemailcommand("mailto:$TO", set_hmail, "", "");
	fprintf(fp, "<strong><a href=\"%s\">%s</A></strong>\n",
		ptr ? ptr : "", lang[MSG_NEW_MESSAGE]);
	if (ptr)
	    free(ptr);
    }

    if (!set_reverse && (called_from != AUTHOR_INDEX && called_from != SUBJECT_INDEX))
	fprintf(fp, "<strong><a href=\"#end\">%s</a></strong><br>\n",
		lang[MSG_MOST_RECENT_MESSAGES]);

    fprintf(fp, "<strong>%d %s %s:</strong> \n", amountmsgs,
	    lang[MSG_ARTICLES], lang[MSG_SORTED_BY]);

    if (called_from != AUTHOR_INDEX && show_index[AUTHOR_INDEX])
	fprintf(fp, "<a href=\"%s\">[ %s ]</a>\n", authname,
		lang[MSG_AUTHOR]);

    if (called_from != DATE_INDEX && show_index[DATE_INDEX])
	fprintf(fp, "<a href=\"%s\">[ %s ]</a>\n", datename,
		lang[MSG_DATE]);

    if (called_from != THREAD_INDEX && show_index[THREAD_INDEX])
	fprintf(fp, "<a href=\"%s\">[ %s ]</a>\n", thrdname,
		lang[MSG_THREAD]);

    if (called_from != SUBJECT_INDEX && show_index[SUBJECT_INDEX])
	fprintf(fp, "<a href=\"%s\">[ %s ]</a>\n", subjname,
		lang[MSG_SUBJECT]);

#ifdef CHANGE_12DEC2000_BC
    if (set_attachmentsindex) {
	if (called != FROM_ATTACHMENT) {
	    fprintf(fp, "<a href=\"%s\">[ %s ]</a>\n", attname,
		lang[MSG_ATTACHMENT]);
	}
    }
#endif

    if (set_about && *set_about)
	fprintf(fp, "<br><strong><a href=\"%s\">%s</a></strong>\n",
		set_about, lang[MSG_ABOUT_THIS_ARCHIVE]);

    if (set_archives && *set_archives)
	fprintf(fp, "<br><strong><a href=\"%s\">%s</a></strong>\n",
		set_archives, lang[MSG_OTHER_MAIL_ARCHIVES]);

    if (set_custom_archives && *set_custom_archives)
        fprintf(fp,"<br><strong>%s:</strong> %s\n", 
		lang[MSG_OTHER_MAIL_ARCHIVES], set_custom_archives);

    /* JK: moved it here as it looks better and changed a bit the text */
    if (set_mailcommand && set_hmail) {
	ptr = makemailcommand(set_newmsg_command, set_hmail, "", "");
	fprintf(fp, "<br><strong>%s:</strong> <a href=\"%s\">[ %s ]</a>\n",
		lang[MSG_MAIL_ACTIONS], ptr ? ptr : "", 
		lang[MSG_MA_NEW_MESSAGE]);
	if (ptr)
	    free(ptr);
    }


#if 0 /* JK removed this */
    fprintf(fp, "<p>\n");
#endif
    /* JK: added a <p> */
    if (set_showhr)
        fprintf(fp, "<hr noshade><p>\n");

    /*
     * Printout the Dates for the Starting and Ending messages 
     * in the archive, along with a count of the messages.
     */

    fprintf(fp, "<strong>%s:</strong> <em>%s</em><br>\n",
            lang[MSG_STARTING], getdatestr(startdatenum));

    fprintf(fp, "<strong>%s:</strong> <em>%s</em><br>\n",
            lang[MSG_ENDING], getdatestr(enddatenum));
}

void print_index_footer_links(FILE *fp, mindex_t called_from,
			      long enddatenum, int amountmsgs)
{
    /* 
     * Print out the links for
     * 
     *      Messages sorted by: [ date ][ subject ][ author ] 
     *      Other mail archives 
     *
     * as appropriate.
     */
     char *ptr;

     fprintf(fp, "<a name=\"end\"><strong>%s:</strong></a> <em>%s</em><br>\n",
		lang[MSG_LAST_MESSAGE_DATE], getdatestr(lastdatenum));

     fprintf(fp, "<strong>%s:</strong> <em>%s</em><p>\n",
             lang[MSG_ARCHIVED_ON], getlocaltime());

     if (set_showhr)
         fprintf(fp, "<hr noshade>\n");

    fprintf(fp, "\n");
    fprintf(fp, "<strong>%d %s %s:</strong> \n", amountmsgs,
	    lang[MSG_ARTICLES], lang[MSG_SORTED_BY]);

    if (called_from != AUTHOR_INDEX && show_index[AUTHOR_INDEX])
	fprintf(fp, "<a href=\"%s\">[ %s ]</a>\n", authname,
		lang[MSG_AUTHOR]);

    if (called_from != DATE_INDEX && show_index[DATE_INDEX])
	fprintf(fp, "<a href=\"%s\">[ %s ]</a>\n", datename,
		lang[MSG_DATE]);

    if (called_from != THREAD_INDEX && show_index[THREAD_INDEX])
	fprintf(fp, "<a href=\"%s\">[ %s ]</a>\n", thrdname,
		lang[MSG_THREAD]);

    if (called_from != SUBJECT_INDEX && show_index[SUBJECT_INDEX])
	fprintf(fp, "<a href=\"%s\">[ %s ]</a>\n", subjname,
		lang[MSG_SUBJECT]);

#ifdef CHANGE_12DEC2000_BC
    if (set_attachmentsindex) {
	if (called != FROM_ATTACHMENT) {
	    fprintf(fp, "<a href=\"%s\">[ %s ]</a>\n", attname,
		lang[MSG_ATTACHMENT]);
	}
    }
#endif

    if (set_about && *set_about)
	fprintf(fp, "<br><strong><a href=\"%s\">%s</a></strong>\n",
		set_about, lang[MSG_ABOUT_THIS_ARCHIVE]);

    if (set_archives && *set_archives)
	fprintf(fp, "<br><strong><a href=\"%s\">%s</a></strong>\n",
		set_archives, lang[MSG_OTHER_MAIL_ARCHIVES]);

    if (set_custom_archives && *set_custom_archives)
        fprintf(fp,"<br><strong>%s:</strong> %s\n", 
		lang[MSG_OTHER_MAIL_ARCHIVES], set_custom_archives);

    /* JK: added it here as it looks better and changed a bit the text */
    if (set_mailcommand && set_hmail) {
	ptr = makemailcommand(set_newmsg_command, set_hmail, "", "");
	fprintf(fp, "<br><strong>%s:</strong> <a href=\"%s\">[ %s ]</a>\n",
		lang[MSG_MAIL_ACTIONS], ptr ? ptr : "", 
		lang[MSG_MA_NEW_MESSAGE]);
	if (ptr)
	    free(ptr);
    }
    fprintf(fp, "\n\n");
}

/*
** Prints a comment in the file.
*/

void printcomment(FILE *fp, char *label, char *value)
{
    if (value && *value)
	/* we only save the non-NULL ones */
	fprintf(fp, "<!-- %s=\"%s\" -->\n", label, value);
}

/*
** Prints a line of HTML. Used to filter off unwanted tags from the output.
*/

void printhtml(FILE *fp, char *line)
{
    /* the banned tags ignore all of them that begin exactly like "<tag" or
       "</tag" (case insensitive). Any other letters within the tags destroy
       this filter capability. */

    char *banned_tags[] = { "html>" };	/* ending >-letter must be included */
    unsigned int i;
    int lindex;

    while (*line) {
	if ('<' != *line)
	    fputc(*line++, fp);
	else {
	    if ('/' == line[1])
		lindex = 2;
	    else
		lindex = 1;
	    for (i = 0; i < sizeof(banned_tags) / sizeof(banned_tags[0]);
		 i++) {
		if (!strncasecmp
		    (&line[lindex], banned_tags[i], strlen(banned_tags[i]))) {
		    line += strlen(banned_tags[i]) + lindex;
		    break;
		}
	    }
	    if (sizeof(banned_tags) / sizeof(banned_tags[0]) == i) {
		fputc(*line++, fp);
		if (2 == lindex)
		    fputc(*line++, fp);
	    }
	}
    }

}

/*
** Pretty-prints the dates in the index files.
*/

void printdates(FILE *fp, struct header *hp, int year, int month)
{
  char *subj;
  const char *startline;
  const char *break_str;
  const char *endline;
  const char *subj_tag;
  const char *subj_end_tag;
  char date_str[DATESTRLEN+11];
  char line_count_str[50];

  if (hp != NULL) {
    struct emailinfo *em=hp->data;
    printdates(fp, hp->left, year, month);
    if ((year == -1 || year_of_datenum(em->date) == year)
	&& (month == -1 || month_of_datenum(em->date) == month)) {
      subj = convchars(em->subject);

      line_count_str[0] = 0;
      if(set_indextable) {
	startline = "<tr><td>";
	break_str = "</td><td>";
	strcpy(date_str, getdatestr(hp->data->date));
	endline = "</td></tr>";
	subj_tag = "";
	subj_end_tag = "";
      }
      else {
	startline = "<li>";
	break_str = "&nbsp;";
	sprintf(date_str, "<em>(%s)</em>", getdatestr(hp->data->date));
	endline = "";
	subj_tag = "<strong>";
	subj_end_tag = "</strong>";
      }
      fprintf(fp,"%s<a href=\"%.4d.%s\">%s%s%s</a>%s"
	      "<a name=\"%d\"><em>%s</em></a>%s%s%s%s\n",
	      startline, em->msgnum, set_htmlsuffix,
	      subj_tag, subj, subj_end_tag, break_str, em->msgnum, em->name,
	      break_str, date_str, line_count_str, endline);
      free(subj);
    }
    printdates(fp, hp->right, year, month);
  }
}

/*
** Pretty-prints the files with attachments in the index files.
*/

#ifdef CHANGE_12DEC2000_BC
void printattachments(FILE *fp, struct header *hp)
{
/* XXX - Next 2 lines copied from parse.c - yuck! */
#define DIR_PREFIXER "att-"
#define PATH_SEPARATOR '/'
    char *subj;
    char* attdir;

    if (hp != NULL) {
	struct emailinfo *em = hp->data;
	printattachments(fp, hp->left);
	subj = convchars(em->subject);

	/* See if there's a directory corresponding to this message */
	attdir = maprintf("%s%c" DIR_PREFIXER "%04d",
	    set_dir, PATH_SEPARATOR, em->msgnum);
	if (isdir(attdir)) {

	    if (set_indextable) {
		fprintf(fp, "<tr><td><a href=\"%.4d.%s\">%s</a></td>"
		    "<td><em>%s</em></a></td>"
		    "<td>%s</td></tr>\n",
		    em->msgnum, set_htmlsuffix, subj,
		    em->name, getdatestr(em->date));
	    }
	    else {
		fprintf(fp,
		    "<li><a href=\"%.4d.%s\"><strong>%s</strong></a>&nbsp;"
		    "<a name=\"%d\"><em>%s</em></a>&nbsp;<em>(%s)</em>\n",
		    em->msgnum, set_htmlsuffix, subj, em->msgnum, em->name,
		    getdatestr(em->date));
	    }
	}
	free(attdir);
	free(subj);
	printattachments(fp, hp->right);
    }
}
#endif

int showheader(char *header)
{
    return (inlist(set_show_headers, header) || inlist(set_show_headers, "*"));
}

/*
 * ConvURLsWithHrefs handles lines with URLs that are already written as
 * href's, to avoid having ConvURLsString add a second href to those URLs.
*/

static char *ConvURLsWithHrefs(const char *line, char *mailid,
			       char *mailsubject, char *c)
{
    char *p;
    struct Push retbuf;
    char *tmpline5;
    char *tmpline6;
    char *tmpline1 = (char *)emalloc(strlen(line) + 1);
    strncpy(tmpline1, line, c - line);
    tmpline1[c - line] = 0;
    INIT_PUSH(retbuf);
    p = strcasestr(c, "</A>");
    if (!p) return NULL;
    	/* complete href found; run ConvURLsString on text outside it*/
    tmpline5 = ConvURLsString(tmpline1, mailid, mailsubject);
    if (!tmpline5) return NULL;
    PushString(&retbuf, tmpline5);
    free(tmpline5);
    p += 4;
    tmpline6 = ConvURLsString(p, mailid, mailsubject);
    if (!tmpline6) {
	free(PUSH_STRING(retbuf));
	return NULL;
    }
    strncpy(tmpline1, c, p - c);
    tmpline1[p - c] = 0;
    PushString(&retbuf, tmpline1);
    PushString(&retbuf, tmpline6);
    free(tmpline1);
    free(tmpline6);
    RETURN_PUSH(retbuf);
}

/*
 * ConvMsgid converts a msgid into a link to the message it refers to if that
 * message is in the archive, and call ConvURLsString on the rest of the line.
*/

static char *ConvMsgid(char *line, char *inreply,
		       char *mailid, char *mailsubject)
{
    char *tmpline4;
    int subjmatch;
    char filename[MAXFILELEN];
    char *tmpline1;
    char *c;
    struct Push buff;
    struct emailinfo *ep = hashreplylookup(-1, inreply, &subjmatch);

    if (ep == NULL || subjmatch)
	return NULL;		/* not a known msgid - maybe an email addr? */
    c = strstr(line, inreply);
    if (!c)
	return NULL;		/* should never happen */
    INIT_PUSH(buff);
    tmpline1 = (char *)emalloc(c - line + 1);
    strncpy(tmpline1, line, c - line);
    tmpline1[c - line] = 0;
    tmpline4 = ConvURLsString(tmpline1, mailid, mailsubject);
    free(tmpline1);
    PushString(&buff, tmpline4);
    free(tmpline4);
    sprintf(filename, "<A HREF=\"%.4d.%s\">", ep->msgnum, set_htmlsuffix);
    PushString(&buff, filename);
    PushString(&buff, inreply);
    PushString(&buff, "</A>");
    tmpline4 = ConvURLsString(c + strlen(inreply), mailid, mailsubject);
    PushString(&buff, tmpline4);
    free(tmpline4);
    RETURN_PUSH(buff);
}


/*
** Converts URLs, email addresses, and message IDs in a line to links,
** mail commands, and articles respectively. Write everything to the
** file pointed to by 'fp'.
*/

void ConvURLs(FILE *fp, char *line, char *mailid, char *mailsubject)
{
    char *parsed = ConvURLsString(line, mailid, mailsubject);
    if (parsed) {
	/* write it to the file! */
	fwrite(parsed, strlen(parsed), 1, fp);
	free(parsed);
    }
}

char *ConvURLsString(char *line, char *mailid, char *mailsubject)
{
    char *parsed;
    char *newparse;
    char *c;

    if ((c = strcasestr(line, "<A HREF=\"")) != NULL
	&& !strcasestr(c + 9, "mailto")) {
	parsed = ConvURLsWithHrefs(line, mailid, mailsubject, c);
	if (parsed)
	    return parsed;
    }

    if (set_linkquotes) {
	char *inreply = getreply(line);
	if (inreply) {
	    parsed = ConvMsgid(line, inreply, mailid, mailsubject);
	    free(inreply);
	    if (parsed)
	        return parsed;
	}
    }

    parsed = parseurl(line);

    if (use_mailcommand) {
	/* Exclude headers that are not mail type headers */

	if (parsed && *parsed) {
	    newparse = parseemail(parsed,	/* source */
				  mailid,	/* mail's Message-Id: */
				  mailsubject);	/* mail's Subject: */
	    free(parsed);
	    parsed = newparse;
	}
    }

    return parsed;
}


/*
** The heuristics for displaying an otherwise ordinary line (a non-quote,
** non-sig, non-inhtml, non-blank line) if 'showhtml' is 1 (which really means
** we're *not* doing <pre> or txt2html to format the text) have changed so
** that indented text shows up correctly.  First, leading spaces become HTML
** non-breaking spaces (&nbsp;).  In order for this to work, those lines
** must have been preceded by a <br> or <p>.  We accomplish this by checking
** ahead when we print each line... if we determine that the next line
** should be broken before, we do so.
*/

void printbody(FILE *fp, struct body *bp, char *id, char *subject,
	       int msgnum, char *dir, char *inreply, int maybe_reply)
{
    int insig, inblank;
#ifdef REMOVED_19991103
    int inhtml;
#endif
    char inheader = FALSE;	/* we always start in a mail header */
    int pre = FALSE;

    int inquote;
    int quote_num;
    int quoted_percent = (set_quote_hide_threshold <= 100
			  ? compute_quoted_percent(bp) : 100);
    bool replace_quoted = (quoted_percent > set_quote_hide_threshold);

    if(set_showprogress && replace_quoted)
      printf("\nMessage %d quoted text (%d %%) replaced by links\n",
	     msgnum,quoted_percent);

    /* JK: added an extra <p> here */
    if (set_showhr)
	fprintf(fp, "<hr noshade><p>\n");

    printcomment(fp, "body", "start");

    if (!set_showhtml) {
	fprintf(fp, "<pre>\n");
	pre = TRUE;
    }

#if DEBUG_PRINTBODY
    else
	printf("SHOWHTML is ON\n");
#endif
    if (set_showhtml == 2)
      init_txt2html();
    inquote = 0;
    quote_num = 0;

    inblank = 1;
    insig = 0;

#ifdef REMOVED_19991103
    inhtml = 0;
#endif

    while (bp != NULL) {
	if (bp->html) {
	    /* already in HTML, don't touch */
	    if (pre) {
		fprintf(fp, "</pre>\n");
		pre = FALSE;
	    }
	    printhtml(fp, bp->line);
	    inheader = FALSE;	/* this can't be a header if already in HTML */
	    bp = bp->next;
	    continue;
	}

	if (bp->header) {
	    char head[128];
	    if (!inheader) {
              /* @@ JK: I'm not sure why, but I had a !set_showhtml here */
                if(!set_showhtml && !pre && set_showheaders) {
		    fprintf(fp, "<pre>\n");
		    pre = TRUE;
		}
		inheader = TRUE;
	    }
	    if (sscanf(bp->line, "%127[^:]", head) &&
		set_show_headers && !showheader(head)) {
		/* the show header keyword has been used, then we skip all those
		   that aren't mentioned! */
		if (isalnum(*head) || !set_showheaders) {
		    /* this check is only to make sure that the last line among 
		       the headers (the "\n" one) won't be filtered off */
		    bp = bp->next;
		    continue;
		}
	    }
	}
	else {
	    if (inheader) {
                insig=0;
		if (set_showhtml) {
		  if (pre) {
			fprintf(fp, "</pre>\n");
			pre = FALSE;
		  }
		  fprintf(fp, "<p>\n");
		}
		inheader = FALSE;
	    }
	}

#ifdef REMOVED_990310
	if (bp->attached && bp->header) {
            /* only show <br> if not in <pre> since 
             * in <pre> mode, the existing newlines are enough! 
             */
	    fprintf(fp, "%s%s", bp->line, pre ? "" : "<br>\n");	
	    bp = bp->next;
	    continue;
	}
#endif

	if (((bp->line)[0] != '\n') && (bp->header && !set_showheaders)) {
	    bp = bp->next;
	    continue;
	}

	if (set_showhtml == 2 && !inheader) {
	    txt2html(fp, bp, id, subject, msgnum, inreply, dir,
		     replace_quoted, maybe_reply);
	    bp = bp->next;
	    continue;
	}

        if (bp->header && set_showheaders && !pre) {
            fprintf(fp, "<pre>\n");
            pre = TRUE;
	}
 
	if ((bp->line)[0] == '\n' && inblank) {
	    bp = bp->next;
	    continue;
	}
	else
	    inblank = 0;

#ifdef REMOVED_19991103
	if (!strncasecmp(bp->line, "<html>", 6)) {
	    inhtml = 1;
	    if (!set_showhtml && pre) {
		fprintf(fp, "</pre>\n");
		pre = FALSE;
	    }
	}
	else if (!strncasecmp(bp->line, "</html>", 7)) {
	    inhtml = 0;
	    if (set_showhtml)
		fprintf(fp, "%s", bp->line);
	    else {
		char *ptr;
		fprintf(fp, "%s<br>", ptr = convchars(bp->line));
		free(ptr);
	    }
	    if (!set_showhtml && !pre) {
		fprintf(fp, "<pre>\n");
		pre = TRUE;
	    }
	    bp = bp->next;
	    continue;
	}

	if (inhtml) {
	    if (set_showhtml)
		fprintf(fp, "%s", bp->line);
	    else {
		char *ptr;
		fprintf(fp, "%s<br>", ptr = convchars(bp->line));
		free(ptr);
	    }
	    bp = bp->next;
	    continue;
	}
#endif
	if (set_showhtml) {
	    if (is_sig_start(bp->line)) {
		insig = 1;
		if (!pre) {
		    fprintf(fp, "<pre>\n");
		    pre = TRUE;
		}
	    }

	    if (!inheader && (bp->line)[0] == '\n')
		fprintf(fp, "<p>");
	    else {
		if (insig) {
		    ConvURLs(fp, bp->line, id, subject);
		}
		else if (isquote(bp->line)) {
		    if(set_linkquotes) {
			if(handle_quoted_text(fp, msgnum, bp, bp->line, inquote,
					      quote_num, inreply, dir,
					      replace_quoted, maybe_reply,
					      id, subject)) {
			    ++quote_num;
			    inquote = 1;
			}
		    }
		    else {
		        fprintf(fp, "%s", (set_iquotes) ? "<em>" : "");

			ConvURLs(fp, bp->line, id, subject);

			fprintf(fp, "%s<br>\n", (set_iquotes) ? "</em>" : "");
		    }
		}
		else if ((bp->line)[0] != '\0') {
		    char *sp;
                    sp = print_leading_whitespace(fp, bp->line);

		    /* JK: avoid converting Message-Id: headers */
		    if (bp->header && bp->parsedheader 
			&& !strncasecmp(bp->line, "Message-Id:", 11) 
			&& use_mailcommand) {
		        /* we desactivate it just during this conversion */
		        use_mailcommand = 0;
			ConvURLs(fp, sp, id, subject);
			use_mailcommand = 1;
		    }
		    else
		        ConvURLs(fp, sp, id, subject);

		    /*
		     * Determine whether we should break.
		     * We could check for leading spaces
		     * or quote lines, but in general,
		     * non-alphanumeric lines should be
		     * broken before.
		     */

		    if ((set_showbr && !bp->header) ||
			((bp->next != NULL) &&
			 !isalnum(bp->next->line[0]))) fprintf(fp, "<br>");
		    if (!bp->header) {
			fprintf(fp, "\n");
		    }
		}

	    }
	}
	else if ((bp->line)[0] != '\0') {
	  /* JK: avoid converting Message-Id: headers */
	  if (bp->header && bp->parsedheader 
	      && !strncasecmp(bp->line, "Message-Id:", 11) 
	      && use_mailcommand) {
	      /* we desactivate it just during this conversion */
	      use_mailcommand = 0;
	      ConvURLs(fp, bp->line, id, subject);
	      use_mailcommand = 1;
	  }
	  else
	      ConvURLs(fp, bp->line, id, subject);
	}
	if (!isquote(bp->line))
	  inquote = 0;
	bp = bp->next;
    }

    if (pre)
	fprintf(fp, "</pre>\n");
    else if (set_showhtml == 2)
        end_txt2html(fp);

    fprintf(fp, "<p>");
    printcomment(fp, "body", "end");
    if (set_showhr)
	fprintf(fp, "<hr noshade>\n");
}

char *
print_leading_whitespace(FILE *fp, char *sp)
{
    while (*sp && (*sp == ' ' || *sp == '\t')) {
        if (*sp == '\t')
	    fprintf(fp, "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;");
	else
	    fprintf(fp, "&nbsp;");
	sp++;
    }
    return sp;
}

/*
** Printing...the other main part of this program!
** This writes out the articles, beginning with the number startnum.
*/

void writearticles(int startnum, int maxnum)
{
    int num, skip, subjmatch, newfile;
    int is_reply = 0;
    int maybe_reply = 0;
    struct emailinfo *old_reply_to = NULL;
    struct emailinfo *email;
    struct emailinfo *email2;
    struct emailinfo *email_next_in_thread;

    char filename[MAXFILELEN];
    struct body *bp, *status;
    struct reply *rp;
    FILE *fp;
    char *ptr;

#ifdef GDBM

    /* A gdbm hack for avoiding opening all the message files to
     * get the header comments; see parse.c for details thereof. */

    char indexname[MAXFILELEN];
    static GDBM_FILE gp;

    if(set_usegdbm) {
      sprintf(indexname, (set_dir[strlen(set_dir)-1] == '/') 
	      ? "%s%s" : "%s/%s",
	      set_dir, GDBM_INDEX_NAME);

      /* open the database, creating it if necessary */
	    
      if(!(gp = gdbm_open(indexname, 0, GDBM_WRCREAT, 0664, 0))) {

	/* couldn't open; unlink it rather than risk running
	 * with an inconsistent version; it will be recreated if
	 * necessary */

	unlink(indexname);
      }
    }
#endif

    num = startnum;

    if (set_showprogress)
	printf("%s \"%s\"...    ", lang[MSG_WRITING_ARTICLES], set_dir);

    while ((bp = hashnumlookup(num, &email))) {

	sprintf(filename, "%s%s%.4d.%s",
		set_dir, (set_dir[strlen(set_dir) - 1] == '/') ? "" : "/",
		num, set_htmlsuffix);

	/*
	 * Determine to overwrite files or not
	 */

	if (isfile(filename))
	    newfile = 0;
	else
	    newfile = 1;

	is_reply = 0;
	set_new_reply_to(-1, -1);

	skip = 0;
	if (!newfile && !set_overwrite) {
	    skip = 1;		/* is this really necessary with continue ??? */
	    num++;
	    continue;
	}
	else {
	    if ((fp = fopen(filename, "w")) == NULL) {
		sprintf(errmsg, "%s \"%s\".", lang[MSG_COULD_NOT_WRITE],
			filename);
		progerr(errmsg);
	    }
	}

	/*
	 * Create the comment fields necessary for incremental updating
	 */

	print_msg_header(fp, set_label,
			 email->subject, set_dir,
			 email->name, email->emailaddr, email->msgid,
			 email->charset, filename);

	printcomment(fp, "received", email->fromdatestr);
	printcomment(fp, "isoreceived", secs_to_iso(email->fromdate));
	printcomment(fp, "sent", email->datestr);
	printcomment(fp, "isosent", secs_to_iso(email->date));
	printcomment(fp, "name", email->name);
	printcomment(fp, "email", email->emailaddr);
	printcomment(fp, "subject", ptr = convchars(email->subject));
	if (ptr)
	    free(ptr);
	printcomment(fp, "id", email->msgid);
	printcomment(fp, "charset", email->charset);
	printcomment(fp, "inreplyto", ptr = convchars(email->inreplyto));
#ifdef GDBM
	if(gp) {
	  togdbm((void *) gp, num, email->name, email->emailaddr, 
		 email->datestr, email->msgid, email->subject, 
		 email->inreplyto, email->fromdatestr, email->charset, 
		 secs_to_iso(email->date), secs_to_iso(email->fromdate));
	}
#endif
	if (ptr)
	    free(ptr);

	if (set_usetable) {
	    fprint_menu(fp, NO_INDEX, "",
			email->msgid, email->subject, PAGE_TOP);
	    fprintf(fp, "<p>\n");
	}

	/*
	 * Print the message's author info and date.
	 */

#if 0 /* JK: removed it because it's redundant with the H1 header */
	fprintf(fp, "<strong>Subject: </strong>%s<br>\n", email->subject);
#endif
	if (!strcmp(email->name, email->emailaddr)) {
	    if (use_mailcommand) {
		ptr = makemailcommand(set_mailcommand,
				      email->emailaddr,
				      email->msgid, email->subject);
		fprintf(fp, "<strong>From:</strong> <a href=\"%s\">", ptr ? ptr : "");
		if (ptr)
		    free(ptr);
		fprintf(fp, "<em>%s</em></a><br>\n", email->name);
	    }
	    else
		fprintf(fp, "<em>%s</em><br>\n", email->name);
	}
	else {
	    if (use_mailcommand && strcmp(email->emailaddr,"(no email)")!=0) {
		ptr = makemailcommand(set_mailcommand,
				      email->emailaddr,
				      email->msgid, email->subject);
		fprintf(fp, "<strong>From:</strong> %s (<a href=\"%s\">",
			email->name, ptr ? ptr : "");
		if (ptr)
		    free(ptr);
		fprintf(fp, "<em>%s</em></a>)<br>\n", email->emailaddr);
	    }
	    else
	    {
		fprintf(fp,
			"<strong>From:</strong> %s (<em>%s</em>)<br>\n",
			email->name, 
			(strcmp(email->emailaddr,"(no email)")!=0) ? email->emailaddr : "no email");
	    }
	}
	fprintf(fp, "<strong>Date:</strong> %s\n<p>\n",
		getdatestr(email->date));

	/*
	 * This is here because it looks better here. The table looks
	 * better before the Author info. This stuff should be in 
	 * printfile() so it could be laid out as the user wants...
	 */

	/*
	 * Should we print message links ?
	 */

	if (set_show_msg_links) {
	    printcomment(fp, "next", "start");
	    fprintf(fp, "<ul>\n");

	    /*
	     * Is there a next message?
	     */

	    status = hashnumlookup(num + 1, &email2);
	    if (status) {
		fprintf(fp, "<li><strong>%s:</strong> ",
			lang[MSG_NEXT_MESSAGE]);

		ptr = convchars(email2->subject);
		fprintf(fp, "<a href=\"%.4d.%s\">%s: \"%s\"</a>\n",
			num + 1, set_htmlsuffix, email2->name,
			ptr ? ptr : "");
		if (ptr)
		    free(ptr);
	    }

	    /*
	     * Is there a previous message?
	     */

	    status = hashnumlookup(num - 1, &email2);
	    if (status) {
		fprintf(fp, "<li><strong>%s:</strong> ",
			lang[MSG_PREVIOUS_MESSAGE]);
		fprintf(fp, "<a href=\"%.4d.%s\">%s: \"%s\"</a>\n",
			num - 1, set_htmlsuffix, email2->name,
			ptr = convchars(email2->subject));
		if (ptr)
		    free(ptr);
	    }

	    /*
	     * Is this message a reply to another?
	     */

	    maybe_reply = 0;
	    if (email->inreplyto[0]) {
		email2 =
		    hashreplylookup(email->msgnum, email->inreplyto,
				    &subjmatch);
		if (email2) {
		    is_reply = 1;
		    if (subjmatch)
			fprintf(fp, "<li><strong>%s:</strong>",
				lang[MSG_MAYBE_IN_REPLY_TO]);
		    else
			fprintf(fp, "<li><strong>%s:</strong>",
				lang[MSG_IN_REPLY_TO]);
		    fprintf(fp, " <a href=\"%.4d.%s\">", email2->msgnum,
			    set_htmlsuffix);
		    fprintf(fp, "%s: \"%s\"</a>\n",
			    email2->name,
			    ptr = convchars(email2->subject));
		    if (ptr)
			free(ptr);
		}
	    }

	    /*
	     * Is there a message next in the thread?
	     */

	    printcomment(fp, "nextthread", "start");

	    email_next_in_thread = nextinthread(email->msgnum);
	    if (email_next_in_thread) {
		fprintf(fp, "<li><strong>%s:</strong> ",
			lang[MSG_NEXT_IN_THREAD]);
		fprintf(fp, "<a href=\"%.4d.%s\">",
			email_next_in_thread->msgnum, set_htmlsuffix);
		fprintf(fp, "%s: \"%s\"</a>\n",
			email_next_in_thread->name,
			ptr = convchars(email_next_in_thread->subject));
		if (ptr)
		    free(ptr);
	    }

	    /*
	     * Does this message have replies? If so, print them all!
	     */

	    if (set_showreplies) {
		for (rp = replylist; rp != NULL; rp = rp->next) {
                    if (rp->frommsgnum == num
                        && hashnumlookup(rp->msgnum, &email2)) {
			if (rp->maybereply)
			    fprintf(fp, "<li><strong>%s:</strong>",
				    lang[MSG_MAYBE_REPLY]);
			else
			    fprintf(fp, "<li><strong>%s:</strong>",
				    lang[MSG_REPLY]);
			fprintf(fp, " <a href=\"%.4d.%s\">",
                                rp->msgnum, set_htmlsuffix);
                        fprintf(fp, "%s: \"%s\"</a>\n", email2->name,
                                ptr = convchars(email2->subject));
			free(ptr);
		    }
		}
		printcomment(fp, "reply", "end");
	    }
	}

	if (!set_usetable) {
            fprintf(fp,"<li><strong>%s %s:</strong> \n",
		      lang[MSG_MESSAGES], lang[MSG_SORTED_BY]);
	    if (show_index[DATE_INDEX])
	        fprintf(fp, "<a href=\"%s#%d\">[ %s ]</a>\n",
			datename, num, lang[MSG_DATE]);
	    if (show_index[THREAD_INDEX])
	        fprintf(fp, "<a href=\"%s#%d\">[ %s ]</a>\n",
			thrdname, num, lang[MSG_THREAD]);
	    if (show_index[SUBJECT_INDEX])
	        fprintf(fp, "<a href=\"%s#%d\">[ %s ]</a>\n",
			subjname, num, lang[MSG_SUBJECT]);
	    if (show_index[AUTHOR_INDEX])
	        fprintf(fp, "<a href=\"%s#%d\">[ %s ]</a>\n",
			authname, num, lang[MSG_AUTHOR]);
#ifdef CHANGE_12DEC2000_BC
	    if (set_attachmentsindex) {
		fprintf(fp, "<a href=\"%s\">[ %s ]</a>\n", attname,
		    lang[MSG_ATTACHMENT]);
	    }
#endif

	    if (set_custom_archives && *set_custom_archives)
	        fprintf(fp,"<li><strong>%s:</strong> %s\n", 
			lang[MSG_OTHER_MAIL_ARCHIVES],
			set_custom_archives);

	    /* JK: moved it here as it looks better */
	    if (set_mailcommand && set_hmail) {
		fprintf(fp, "<li><strong>%s:</strong>", 
			lang[MSG_MAIL_ACTIONS]);
		if ((email->msgid && email->msgid[0]) ||
		    (email->subject && email->subject[0])) {
		    ptr = makemailcommand(set_replymsg_command, set_hmail,
					  email->msgid, email->subject);
		    fprintf(fp,
			    " <a href=\"%s\">[ %s ]</a>",
			    ptr ? ptr : "", lang[MSG_MA_REPLY]);
		    if (ptr)
			free(ptr);
		}
	        ptr = makemailcommand(set_newmsg_command, set_hmail,
				      email->msgid, email->subject);
		fprintf(fp, " <a href=\"%s\">[ %s ]</a>\n",
			ptr ? ptr : "", lang[MSG_MA_NEW_MESSAGE]);
		if (ptr)
		    free(ptr);

	    }
	}

	if (set_show_msg_links || !set_usetable)
	    fprintf(fp, "</ul>\n");

	if (set_linkquotes || set_showhtml == 2)
	  /* should be changed to unconditional after tested for a while?
	     - pcm@rahul.net 1999-09-09 */
	    find_quote_prefix(email->bodylist, is_reply);

	/*
	 * Finally...print the body!
	 */

	printbody(fp, email->bodylist, email->msgid, email->subject,
		  num, set_dir, email->inreplyto, maybe_reply);

	/*
	 * Should we print out the message links ?
	 */

	if (set_show_msg_links) {
	  /* JK: removed a <p>\n here */
	    fprintf(fp, "<ul>\n");

	    printcomment(fp, "next", "start");

	    status = hashnumlookup(num + 1, &email2);

	    if (status != NULL) {
		fprintf(fp, "<li><strong>%s:</strong> ",
			lang[MSG_NEXT_MESSAGE]);
		fprintf(fp, "<a href=\"%.4d.%s\">%s: \"%s\"</a>\n",
			num + 1, set_htmlsuffix, email2->name, ptr =
			convchars(email2->subject));
		free(ptr);
	    }

	    status = hashnumlookup(num - 1, &email2);
	    if (set_linkquotes && old_reply_to
		&& num - 1 == (get_new_reply_to() == -1 ? old_reply_to->msgnum
			       : get_new_reply_to()))
	        status = 0;
	    if (status) {
		fprintf(fp, "<li><strong>%s:</strong> ",
			lang[MSG_PREVIOUS_MESSAGE]);
		fprintf(fp, "<a href=\"%.4d.%s\">%s: \"%s\"</a>\n",
			num - 1, set_htmlsuffix, email2->name,
			ptr = convchars(email2->subject));
		free(ptr);
	    }

	    if (*email->inreplyto) {
		email2 =
		    hashreplylookup(email->msgnum, email->inreplyto,
				    &subjmatch);
		if (email2) {
		    if (subjmatch)
			fprintf(fp, "<li><strong>%s:</strong>",
				lang[MSG_MAYBE_IN_REPLY_TO]);
		    else
			fprintf(fp, "<li><strong>%s:</strong>",
				lang[MSG_IN_REPLY_TO]);
		    fprintf(fp, " <a href=\"%.4d.%s\">",
			    email2->msgnum, set_htmlsuffix);
		    fprintf(fp, "%s: \"%s\"</a>\n", email2->name,
			    ptr = convchars(email2->subject));
		    free(ptr);
		}
	    }

	    printcomment(fp, "nextthread", "start");

	    /* email_next_in_thread = nextinthread(email->msgnum); redundant */
	    if (email_next_in_thread) {
		fprintf(fp, "<li><strong>%s:</strong> ",
			lang[MSG_NEXT_IN_THREAD]);
		fprintf(fp, "<a href=\"%.4d.%s\">",
			email_next_in_thread->msgnum, set_htmlsuffix);
		fprintf(fp, "%s: \"%s\"</a>\n",
			email_next_in_thread->name,
			ptr = convchars(email_next_in_thread->subject));
		if (ptr)
		    free(ptr);
	    }

	    if (set_showreplies) {
		/* FIXME:
		   This should be cleaned up to use threadprint.c functions */

		for (rp = replylist; rp != NULL; rp = rp->next) {
                    if (rp->frommsgnum == num
                        && hashnumlookup(rp->msgnum, &email2)) {
			if (rp->maybereply)
			    fprintf(fp, "<li><strong>%s:</strong>",
				    lang[MSG_MAYBE_REPLY]);
			else
			    fprintf(fp, "<li><strong>%s:</strong>",
				    lang[MSG_REPLY]);
			fprintf(fp, " <a href=\"%.4d.%s\">",
                                rp->msgnum, set_htmlsuffix);
                        fprintf(fp, "%s: \"%s\"</a>\n", email2->name,
                                ptr = convchars(email2->subject));
			free(ptr);
		    }
		}
		printcomment(fp, "reply", "end");
	    }
	    if (set_usetable)
	        fprintf(fp, "</ul>\n");
	}

	if (set_usetable)
	    fprint_menu(fp, NO_INDEX, "",
			email->msgid, email->subject, PAGE_BOTTOM);
	else {
	      if (!set_show_msg_links)
		  fprintf(fp, "<ul>\n");
	      fprintf(fp,"<li><strong>%s %s:</strong> \n",
		      lang[MSG_MESSAGES], lang[MSG_SORTED_BY]);
	      if (show_index[DATE_INDEX])
		  fprintf(fp,"<a href=\"%s#%d\">[ %s ]</a>\n",
			  datename, num, lang[MSG_DATE]);
	      if (show_index[THREAD_INDEX])
		  fprintf(fp,"<a href=\"%s#%d\">[ %s ]</a>\n",
			  thrdname, num, lang[MSG_THREAD]);
	      if (show_index[SUBJECT_INDEX])
		  fprintf(fp,"<a href=\"%s#%d\">[ %s ]</a>\n",
			  subjname, num, lang[MSG_SUBJECT]);
	      if (show_index[AUTHOR_INDEX])
		  fprintf(fp,"<a href=\"%s#%d\">[ %s ]</a>\n",
			  authname, num, lang[MSG_AUTHOR]);
#ifdef CHANGE_12DEC2000_BC
	    if (set_attachmentsindex) {
		fprintf(fp, "<a href=\"%s\">[ %s ]</a>\n", attname,
		    lang[MSG_ATTACHMENT]);
	    }
#endif
	      if (set_custom_archives && *set_custom_archives)
		  fprintf(fp,"<li><strong>%s:</strong> %s\n", 
			  lang[MSG_OTHER_MAIL_ARCHIVES],
			  set_custom_archives);
	      /* JK: added it here, so that we have symmetry */
	      if (set_mailcommand && set_hmail) {

		  fprintf(fp, "<li><strong>%s:</strong>", 
			  lang[MSG_MAIL_ACTIONS]);
		  if ((email->msgid && email->msgid[0]) ||
		      (email->subject && email->subject[0])) {
		      ptr = makemailcommand(set_replymsg_command, set_hmail,
					    email->msgid, email->subject);
		      fprintf(fp,
			      " <a href=\"%s\">[ %s ]</a>",
			      ptr ? ptr : "", lang[MSG_MA_REPLY]);
		      if (ptr)
			  free(ptr);
		  }
	          ptr = makemailcommand(set_newmsg_command, set_hmail,
					email->msgid, email->subject);
		  fprintf(fp, " <a href=\"%s\">[ %s ]</a>\n",
			  ptr ? ptr : "", lang[MSG_MA_NEW_MESSAGE]);
		  if (ptr)
		     free(ptr);
	      }
	      fprintf(fp, "</ul>\n");
	}

	printfooter(fp, mhtmlfooterfile, set_label, set_dir,
		    email->subject, filename);

	fclose(fp);

	if (get_new_reply_to() != -1) {
	  	/* will only be true if set_linkquotes is */
	    replace_maybe_replies(filename, set_dir, get_new_reply_to());
	    for (rp = replylist; rp != NULL; rp = rp->next) {
	        /* get rid of old guesses for where this links */
	        if (rp->msgnum == num) {
	            rp->frommsgnum = get_new_reply_to();
		    rp->maybereply = 0;
		    break;
		}
	    }
	    if (!rp) {
	        struct emailinfo *e3;
		if(hashnumlookup(num, &e3)) {
		    replylist = addreply(replylist, get_new_reply_to(), e3, 0);
		}
	    }
	}

	if (newfile && chmod(filename, set_filemode) == -1) {
	    sprintf(errmsg, "%s \"%s\": %o.",
		    lang[MSG_CANNOT_CHMOD], filename, set_filemode);
	    progerr(errmsg);
	}

	if (maxnum && !(num % 5) && set_showprogress) {
	    printf("\b\b\b\b%03.0f%c",
		   ((float)num / (float)maxnum) * 100, '%');
	    fflush(stdout);
	}

	num++;
    }

#ifdef GDBM
    if(gp) {
      gdbm_close(gp);
    }
#endif

    if (set_showprogress)
	printf("\b\b\b\b    \n");
} /* end writearticles() */

/*
** Write the date index...
*/

void writedates(int amountmsgs)
{
    int newfile;
    char filename[MAXFILELEN];
    FILE *fp;

    sprintf(filename, "%s%s%s", set_dir,
	    (set_dir[strlen(set_dir) - 1] == '/') ? "" : "/", datename);

    if (isfile(filename))
	newfile = 0;
    else
	newfile = 1;

    if ((fp = fopen(filename, "w")) == NULL) {
	sprintf(errmsg, "%s \"%s\".", lang[MSG_COULD_NOT_WRITE], filename);
	progerr(errmsg);
    }

    if (set_showprogress)
	printf("%s \"%s\"...", lang[MSG_WRITING_DATE_INDEX], filename);

    /*
     * Print out the index file header 
     */
    print_index_header(fp, set_label, set_dir, lang[MSG_BY_DATE], datename);

    /* 
     * Print out archive information links at the top of the index
     */
    if (!set_usetable) 
        print_index_header_links(fp, DATE_INDEX, firstdatenum, lastdatenum,
				 amountmsgs);				 
    else {
	fprint_menu(fp, DATE_INDEX, set_archives, "", "", PAGE_TOP);
	fprint_summary(fp, PAGE_TOP, firstdatenum, lastdatenum, amountmsgs);
	/* JK: added an extra <p> here */
	if (set_showhr)
	    fprintf(fp, "<hr noshade><p>\n"); 
    }

    /*
     * Print out the actual message index lists. Here's the beef.
     */

    if (set_indextable) {
	fprintf(fp,
		"<div align=\"center\">\n<table width=\"80%%\">\n<tr><td><u><strong>%s</strong></u></td><td><u><strong>%s</strong></u></td><td><u><strong>%s</strong></u></td></tr>\n",
		lang[MSG_CSUBJECT], lang[MSG_CAUTHOR], lang[MSG_CDATE]);
    	printdates(fp, datelist, -1, -1);
	fprintf(fp, "</table>\n</div>\n<p>\n");
    }
    else {
	fprintf(fp, "<ul>\n");
	printdates(fp, datelist, -1, -1);
	fprintf(fp, "</ul>\n<p>\n");
    }

    if (!set_usetable) {
	/* 
	 * Print out archive information links at the bottom of the index
	 */
	print_index_footer_links(fp, DATE_INDEX, lastdatenum, amountmsgs);
    }
    else {
	if (set_showhr)
	    fprintf(fp, "<hr noshade>\n");
	fprint_summary(fp, PAGE_BOTTOM, firstdatenum, lastdatenum, amountmsgs);
	fprint_menu(fp, DATE_INDEX, set_archives, "", "", PAGE_BOTTOM);
    }

    /* 
     * Print the index page footer.
     */
    printfooter(fp, ihtmlfooterfile, set_label, set_dir, lang[MSG_BY_DATE],
		datename);

    fclose(fp);

    if (newfile && chmod(filename, set_filemode) == -1) {
	sprintf(errmsg, "%s \"%s\": %o.",
		lang[MSG_CANNOT_CHMOD], filename, set_filemode);
	progerr(errmsg);
    }

    if (set_showprogress)
	putchar('\n');
}

/*
** Write the attachments index...
*/
#ifdef CHANGE_12DEC2000_BC
void writeattachments(int amountmsgs)
{
    int newfile;
    char filename[MAXFILELEN];
    FILE *fp;

    sprintf(filename, "%s%s%s", set_dir,
	    (set_dir[strlen(set_dir) - 1] == '/') ? "" : "/", attname);

    if (isfile(filename))
	newfile = 0;
    else
	newfile = 1;

    if ((fp = fopen(filename, "w")) == NULL) {
	sprintf(errmsg, "%s \"%s\".", lang[MSG_COULD_NOT_WRITE], filename);
	progerr(errmsg);
    }

    if (set_showprogress)
	printf("%s \"%s\"...", lang[MSG_WRITING_ATTACHMENT_INDEX], filename);

    /*
     * Print out the index file header 
     */
    print_index_header(fp, set_label, set_dir, lang[MSG_BY_ATTACHMENT], attname);

    /* 
     * Print out archive information links at the top of the index
     */
    if (!set_usetable) 
	print_index_header_links(fp, FROM_ATTACHMENT, amountmsgs);
    else {
	fprint_menu(fp, ATTACHMENT_INDEX, set_archives, "", "", PAGE_TOP);
	fprint_summary(fp, PAGE_TOP, firstdatenum, lastdatenum, amountmsgs);
	/* JK: added an extra <p> here */
	if (set_showhr)
	    fprintf(fp, "<hr noshade><p>\n"); 
    }

    /*
     * Print out the actual message index lists. Here's the beef.
     */

    if (set_indextable) {
	fprintf(fp,
		"<div align=\"center\">\n<table width=\"80%%\">\n<tr><td><u><strong>%s</strong></u></td><td><u><strong>%s</strong></u></td><td><u><strong>%s</strong></u></td></tr>\n",
		lang[MSG_CSUBJECT], lang[MSG_CAUTHOR], lang[MSG_CDATE]);
	printattachments(fp, datelist);
	fprintf(fp, "</table>\n</div>\n<p>\n");
    }
    else {
	fprintf(fp, "<ul>\n");
	printattachments(fp, datelist);
	fprintf(fp, "</ul>\n<p>\n");
    }

    if (!set_usetable) {
	/* 
	 * Print out archive information links at the bottom of the index
	 */
	print_index_footer_links(fp, FROM_DATE, amountmsgs);
    }
    else {
	if (set_showhr)
	    fprintf(fp, "<hr noshade>\n");
	fprint_summary(fp, PAGE_BOTTOM, firstdatenum, lastdatenum, amountmsgs);
	fprint_menu(fp, DATE_INDEX, set_archives, "", "", PAGE_BOTTOM);
    }

    /* 
     * Print the index page footer.
     */
    printfooter(fp, ihtmlfooterfile, set_label, set_dir, lang[MSG_BY_DATE],
		attname);

    fclose(fp);

    if (newfile && chmod(filename, set_filemode) == -1) {
	sprintf(errmsg, "%s \"%s\": %o.",
		lang[MSG_CANNOT_CHMOD], filename, set_filemode);
	progerr(errmsg);
    }

    if (set_showprogress)
	putchar('\n');
}
#endif

/*
** Write the thread index...
*/

void writethreads(int amountmsgs)
{
    int newfile;
    char filename[MAXFILELEN];
    FILE *fp;

    struct printed *pp;

    while (printedlist) {	/* cleanup needed ?? */
	pp = printedlist;
	printedlist = printedlist->next;
	free(pp);
    }

    printedlist = NULL;

    sprintf(filename, "%s%s%s",
	    set_dir, (set_dir[strlen(set_dir) - 1] == '/') ? "" : "/", 
            thrdname);

    if (isfile(filename))
	newfile = 0;
    else
	newfile = 1;

    if ((fp = fopen(filename, "w")) == NULL) {
	sprintf(errmsg, "%s \"%s\".", lang[MSG_COULD_NOT_WRITE], filename);
	progerr(errmsg);
    }

    if (set_showprogress)
	printf("%s \"%s\"...", lang[MSG_WRITING_THREAD_INDEX], filename);

    print_index_header(fp, set_label, set_dir, lang[MSG_BY_THREAD], thrdname);

    if (!set_usetable) {
	/* 
	 * Print out the index page links 
	 */
	print_index_header_links(fp, THREAD_INDEX, firstdatenum, lastdatenum,
				 amountmsgs);
    }
    else {
	fprint_menu(fp, THREAD_INDEX, set_archives, "", "", PAGE_TOP);
	fprint_summary(fp, PAGE_TOP, firstdatenum, lastdatenum, amountmsgs);
	/* JK: added an extra <p> here */
	if (set_showhr)
	    fprintf(fp, "<hr noshade><p>\n");
    }

    if (set_indextable) {
	fprintf(fp,
		"<div align=\"center\">\n<table>\n<tr><td><u><strong>%s</strong></u></td><td><u><strong>%s</strong></u></td><td><u><strong> %s</strong></u></td></tr>\n",
		lang[MSG_CSUBJECT], lang[MSG_CAUTHOR], lang[MSG_CDATE]);
	print_all_threads(fp, -1, -1);
	fprintf(fp, "</table>\n</div>\n<p>\n");
    }
    else {
	fprintf(fp, "<ul>\n");
	print_all_threads(fp, -1, -1);
	fprintf(fp, "</ul>\n<p>\n");
    }

    if (!set_usetable) {
	/* 
	 * Print out archive information links at the bottom of the index
	 */
        print_index_footer_links(fp, THREAD_INDEX, lastdatenum, amountmsgs);
    }
    else {
	if (set_showhr)
	    fprintf(fp, "<hr noshade>\n");
	fprint_summary(fp, PAGE_BOTTOM, firstdatenum, lastdatenum, amountmsgs);
	fprint_menu(fp, THREAD_INDEX, set_archives, "", "", PAGE_BOTTOM);
    }

    printfooter(fp, ihtmlfooterfile, set_label, set_dir,
		lang[MSG_BY_THREAD], thrdname);

    fclose(fp);

    if (newfile && chmod(filename, set_filemode) == -1) {
	sprintf(errmsg, "%s \"%s\": %o.",
		lang[MSG_CANNOT_CHMOD], filename, set_filemode);
	progerr(errmsg);
    }

    if (set_showprogress)
	putchar('\n');
}

/*
** Print the subject index pointers alphabetically.
*/

void printsubjects(FILE *fp, struct header *hp, char **oldsubject,
		   int year, int month)
{
  char *subj;
  const char *startline;
  const char *break_str;
  const char *endline;
  char date_str[DATESTRLEN+11];
  char line_count_str[50];

  if (hp != NULL) {
    printsubjects(fp, hp->left, oldsubject, year, month);
    if ((year == -1 || year_of_datenum(hp->data->date) == year)
	&& (month == -1 || month_of_datenum(hp->data->date) == month)) {
      subj = convchars(hp->data->unre_subject);

	if (strcasecmp(hp->data->unre_subject, *oldsubject)) {
	    if (set_indextable) {
		fprintf(fp,
			"<tr><td colspan=\"3\"><strong>%s</strong></td></tr>\n",
			subj);
	    }
	    else {
		fprintf(fp, "<li><strong>%s</strong>\n", subj);
	    }
	}
	line_count_str[0] = 0;
	if(set_indextable) {
	    startline = "<tr><td>&nbsp;</td><td>";
	    break_str = "</td><td>";
	    strcpy(date_str, getdatestr(hp->data->date));
	    endline = "</td></tr>";
	}
	else {
	    startline = "<ul><li>";
	    break_str = "";
	    sprintf(date_str, "<em>(%s)</em>", getdatestr(hp->data->date));
	    endline = "</ul>";
	}
	fprintf(fp,"%s<a href=\"%.4d.%s\">%s</a>%s <a name=\"%d\">%s</a>%s%s\n",
		startline, hp->data->msgnum,
		set_htmlsuffix, hp->data->name, break_str,
		hp->data->msgnum,
		date_str, line_count_str, endline);
	*oldsubject = hp->data->unre_subject;
      
	free(subj);
    }
    printsubjects(fp, hp->right, oldsubject, year, month);
  }
}

/*
** Prints the subject index.
*/

void writesubjects(int amountmsgs)
{
    int newfile;
    char filename[MAXFILELEN];
    FILE *fp;

    sprintf(filename, "%s%s%s",
	    set_dir, (set_dir[strlen(set_dir) - 1] == '/') ? "" : "/", 
            subjname);

    if (isfile(filename))
	newfile = 0;
    else
	newfile = 1;

    if ((fp = fopen(filename, "w")) == NULL) {
	sprintf(errmsg, "%s \"%s\".", lang[MSG_COULD_NOT_WRITE], filename);
	progerr(errmsg);
    }

    if (set_showprogress)
	printf("%s \"%s\"...", lang[MSG_WRITING_SUBJECT_INDEX], filename);

    print_index_header(fp,set_label,set_dir,lang[MSG_BY_SUBJECT],subjname);

    /* 
     * Print out the index page links 
     */
    if (!set_usetable) 
	print_index_header_links(fp, SUBJECT_INDEX, firstdatenum, lastdatenum,
				 amountmsgs);
    else {
	fprint_menu(fp, SUBJECT_INDEX, set_archives, "", "", PAGE_TOP);
	fprint_summary(fp, PAGE_TOP, firstdatenum, lastdatenum, amountmsgs);
	/* JK: added this <p> here */
	if (set_showhr)
	    fprintf(fp, "<hr noshade><p>\n");
    }

    if (set_indextable) {
	fprintf(fp,
		"<div align=\"center\">\n<table>\n<tr><td><u><strong>%s</strong></u></td><td><u><strong>%s</strong></u></td><td><u><strong> %s</strong></u></td></tr>\n",
		lang[MSG_CSUBJECT], lang[MSG_CAUTHOR], lang[MSG_CDATE]);
    }
    else {
	fprintf(fp, "<ul>\n");
    }
    {
	char *oldsubject = "";	/* dummy to start with */
	printsubjects(fp, subjectlist, &oldsubject, -1, -1);
    }
    if (set_indextable) {
	fprintf(fp, "</table>\n</div>\n<p>\n");
    }
    else {
	fprintf(fp, "</ul>\n<p>\n");
    }

    /* 
     * Print out archive information links at the bottom of the index
     */

    if (!set_usetable) 
	print_index_footer_links(fp, SUBJECT_INDEX, lastdatenum, amountmsgs);
    else {
	if (set_showhr)
	    fprintf(fp, "<hr noshade>\n");
	fprint_summary(fp, PAGE_BOTTOM, firstdatenum, lastdatenum, amountmsgs);
	fprint_menu(fp, SUBJECT_INDEX, set_archives, "", "", PAGE_BOTTOM);
    }

    printfooter(fp, ihtmlfooterfile, set_label, set_dir,
		lang[MSG_BY_SUBJECT], subjname);

    fclose(fp);

    if (newfile && chmod(filename, set_filemode) == -1) {
	sprintf(errmsg, "%s \"%s\": %o.",
		lang[MSG_CANNOT_CHMOD], filename, set_filemode);
	progerr(errmsg);
    }

    if (set_showprogress)
	putchar('\n');
}

/*
** Prints the author index links sorted alphabetically.
*/

void printauthors(FILE *fp, struct header *hp, char **oldname,
		  int year, int month)
{
  char *subj;
  const char *startline;
  const char *break_str;
  const char *endline;
  char date_str[DATESTRLEN+11];
  char line_count_str[50];

  if (hp != NULL) {
    printauthors(fp, hp->left, oldname, year, month);
    if ((year == -1 || year_of_datenum(hp->data->date) == year)
	&& (month == -1 || month_of_datenum(hp->data->date) == month)) {

      subj = convchars(hp->data->subject);

      if (strcasecmp(hp->data->name, *oldname)) {

	if(set_indextable)
		fprintf(fp,
			"<tr><td colspan=\"3\"><strong>%s</strong></td></tr>",
			hp->data->name);
	else
		fprintf(fp, "<li><strong>%s</strong>\n", hp->data->name);
      }
      line_count_str[0] = 0;
      if(set_indextable) {
	startline = "<tr><td>&nbsp;</td><td>";
	break_str = "</td><td>";
	strcpy(date_str, getdatestr(hp->data->date));
	endline = "</td></tr>";
      }
      else {
	startline = "<ul><li>";
	break_str = "&nbsp;";
	sprintf(date_str, "<em>(%s)</em>", getdatestr(hp->data->date));
	endline = "</ul>";
      }
      fprintf(fp,"%s<a href=\"%.4d.%s\">%s</a>%s<a name=\"%d\">%s</a>%s%s\n",
	      startline, hp->data->msgnum,
	      set_htmlsuffix, subj, break_str,
	      hp->data->msgnum,
	      date_str, line_count_str, endline);
      free(subj);

      *oldname = hp->data->name;	/* avoid copying */
    }
    printauthors(fp, hp->right, oldname, year, month);
  }
}

/*
** Prints the author index file and links sorted alphabetically.
*/

void writeauthors(int amountmsgs)
{
    int newfile;
    char filename[MAXFILELEN];
    FILE *fp;

    sprintf(filename, "%s%s%s",
	    set_dir, (set_dir[strlen(set_dir) - 1] == '/') ? "" : "/", 
            authname);

    if (isfile(filename))
	newfile = 0;
    else
	newfile = 1;

    if ((fp = fopen(filename, "w")) == NULL) {
	sprintf(errmsg, "%s \"%s\".", lang[MSG_COULD_NOT_WRITE], filename);
	progerr(errmsg);
    }

    if (set_showprogress)
	printf("%s \"%s\"...", lang[MSG_WRITING_AUTHOR_INDEX], filename);

    print_index_header(fp, set_label, set_dir, lang[MSG_BY_AUTHOR], authname);

    if (!set_usetable) {
	/* 
	 * Print out the index page links 
	 */
	print_index_header_links(fp, AUTHOR_INDEX, firstdatenum, lastdatenum,
				 amountmsgs);
    }
    else {
	fprint_menu(fp, AUTHOR_INDEX, set_archives, "", "", PAGE_TOP);
	fprint_summary(fp, PAGE_TOP, firstdatenum, lastdatenum, amountmsgs);
	/* JK: added the extra <p> here */
	if (set_showhr)
	    fprintf(fp, "<hr noshade><p>\n");
    }

    if (set_indextable) {
	fprintf(fp,
		"<div align=\"center\">\n<table>\n<tr><td><u><strong>%s</strong></u></td><td><u><strong>%s</strong></u></td><td><u><strong> %s</strong></u></td></tr>\n",
		lang[MSG_CAUTHOR], lang[MSG_CSUBJECT], lang[MSG_CDATE]);
    }
    else {
	fprintf(fp, "<ul>\n");
    }
    {
	char *prevauthor = "";
	printauthors(fp, authorlist, &prevauthor, -1, -1);
    }
    if (set_indextable) {
	fprintf(fp, "</table>\n</div>\n<p>\n");
    }
    else {
	fprintf(fp, "</ul>\n<p>\n");
    }

    /* 
     * Print out archive information links at the bottom 
     * of the index page
     */

    if (!set_usetable) 
	print_index_footer_links(fp, AUTHOR_INDEX, lastdatenum, amountmsgs);
    else {
	if (set_showhr)
	    fprintf(fp, "<hr noshade>\n");
	fprint_summary(fp, PAGE_BOTTOM, firstdatenum, lastdatenum, amountmsgs);
	fprint_menu(fp, AUTHOR_INDEX, set_archives, "", "", PAGE_BOTTOM);
    }

    printfooter(fp, ihtmlfooterfile, set_label, set_dir,
		lang[MSG_BY_AUTHOR], authname);

    fclose(fp);

    if (newfile && chmod(filename, set_filemode) == -1) {
	sprintf(errmsg, "%s \"%s\": %o.",
		lang[MSG_CANNOT_CHMOD], filename, set_filemode);
	progerr(errmsg);
    }

    if (set_showprogress)
	putchar('\n');
}

static int count_messages(struct header *hp, int year, int mo,
			  long *first_date, long *last_date)
{
    if (hp != NULL) {
	struct emailinfo *em = hp->data;
	int cnt = count_messages(hp->left, year, mo, first_date, last_date);
	if ((year == -1 || year_of_datenum(em->date) == year)
	    && (mo == -1 || month_of_datenum(em->date) == mo)) {
	    ++cnt;
	    if (em->date < *first_date)
	        *first_date = em->date;
	    if (em->date > *last_date)
	        *last_date = em->date;
	}
	return cnt + count_messages(hp->right, year, mo, first_date, last_date);
    }
    return 0;
}

static void printmonths(FILE *fp, char *summary_filename, int amountmsgs)
{
    int first_year = year_of_datenum(firstdatenum);
    int last_year = year_of_datenum(lastdatenum);
    int y, j, m;
    char *save_datename = datename;
    char *save_thrdname = thrdname;
    char *save_subjname = subjname;
    char *save_authname = authname;
    char *subject = lang[set_monthly_index ? MSG_MONTHLY_INDEX
			: MSG_YEARLY_INDEX];
    print_index_header(fp, set_label, set_dir, subject, summary_filename);
    fprintf(fp, "<TABLE>\n");
    for (y = first_year; y <= last_year; ++y) {
	for(m = (set_monthly_index ? 0 : -1);
	    m < (set_monthly_index ? 12 : 0); ++m) {
	    char month_str[80];
	    char month_str_pub[80];
	    int started_line = 0;
	    int empties = 0;
	    char period_bufs[NO_INDEX][MAXFILELEN];
	    long first_date = lastdatenum;
	    long last_date = firstdatenum;
	    int count;
	    if (!datelist->data)
	        continue;
	    count = count_messages(datelist, y, m, &first_date, &last_date);
	    if (set_monthly_index) {
		sprintf(month_str_pub, "%s %d", months[m], y);
		sprintf(month_str, "%d%.2d", y, m + 1);
	    }
	    else {
		sprintf(month_str_pub, "%d", y);
		sprintf(month_str, "%d", y);
	    }
	    for(j = 0; j <= AUTHOR_INDEX; ++j) {
		sprintf(period_bufs[j], "%sby%s", month_str,
			indextypefilename[j]);
	    }
	    datename = period_bufs[DATE_INDEX];
	    thrdname = period_bufs[THREAD_INDEX];
	    subjname = period_bufs[SUBJECT_INDEX];
	    authname = period_bufs[AUTHOR_INDEX];
	    for(j = 0; j <= AUTHOR_INDEX; ++j) {
		char filename[MAXFILELEN];
		FILE *fp1;
		char *prev_text = "";
		char subject_title[128];
		if (!show_index[j])
		    continue;
		sprintf(filename, "%s%s%sby%s", set_dir,
			(set_dir[strlen(set_dir) - 1] == '/') ? "" : "/",
			month_str, indextypefilename[j]);
		fp1 = fopen(filename,"w");
		if(!fp1) {
		    sprintf(errmsg,"can't open %s", filename);
		    progerr(NULL);
		}
		sprintf(subject_title, "%s %s", month_str_pub, indextypename[j]);
		print_index_header(fp1, set_label, set_dir, subject_title,
				   filename);
		if (!set_usetable) {
		    /* 
		     * Print out the index page links 
		     */
		    print_index_header_links(fp1, j, first_date, last_date,
					     count);
		}
		else {
		    fprint_menu(fp1, j, set_archives, "", "", PAGE_TOP);
		    fprint_summary(fp1, PAGE_TOP, first_date, last_date,
				   count);
		    if (set_showhr)
		        fprintf(fp1, "<HR NOSHADE>\n");
		}

		if (set_indextable) {
		    fprintf(fp1,
			    "<DIV ALIGN=CENTER>\n<TABLE>\n<TR><TD><U><STRONG>%s</STRONG></U></TD><TD><U><STRONG>%s</STRONG></U></TD><TD><U><STRONG> %s</STRONG></U></TD></TR>\n",
			    lang[j == AUTHOR_INDEX ? MSG_CAUTHOR : MSG_CSUBJECT],
			    lang[j == AUTHOR_INDEX ? MSG_CSUBJECT : MSG_CAUTHOR],
			    lang[MSG_CDATE]);
		}
		else {
		    fprintf(fp1, "<UL>\n");
		}
		switch(j) {
		    case DATE_INDEX:
		        printdates(fp1, datelist, y, m);
			break;
		    case THREAD_INDEX:
		        /*printthreads(fp1, datelist, 0, y, m); */
		        print_all_threads(fp1, y, m);
			break;
		    case SUBJECT_INDEX:
		        printsubjects(fp1, subjectlist, &prev_text, y, m);
			break;
		    case AUTHOR_INDEX:
		        printauthors(fp1, authorlist, &prev_text, y, m);
			break;
		}

		if (set_indextable) {
		    fprintf(fp1, "</TABLE>\n</DIV>\n<P>\n");
		}
		else {
		    fprintf(fp1, "</UL>\n<P>\n");
		}

		/* 
		 * Print out archive information links at the bottom 
		 * of the index page
		 */

		if (!set_usetable)
		    print_index_footer_links(fp1, j, last_date, count);
		else {
		    if (set_showhr)
		        fprintf(fp1, "<HR NOSHADE>\n");
		    fprint_summary(fp1, PAGE_BOTTOM, first_date, last_date,
				   count);
		    fprint_menu(fp1, j, set_archives, "", "", PAGE_BOTTOM);
		}

		printfooter(fp1, ihtmlfooterfile, set_label, set_dir,
			    subject_title, indextypefilename[j]);
		fclose(fp1);
		if (!count) {
		    remove(filename);
		    if(started_line)
		        fprintf(fp, "<TD></TD>");
		    else ++empties;
		}
		else {
		    if (!started_line) {
		        fprintf(fp, "<TR><TD>%s</TD><TD>%d %s</TD>",
				month_str_pub, count, lang[MSG_ARTICLES]);
			while(empties--)
			    fprintf(fp, "<TD></TD>");
			started_line = 1;
		    }
		    chmod(filename, set_filemode);
		    fprintf(fp, "<TD><A HREF=\"%sby%s\">%s</A></TD>",
			    month_str, indextypefilename[j],indextypename[j]);
		}
	    }
	    if (started_line) fprintf(fp, "</TR>\n");
	}
    }
    fprintf(fp,"</TABLE>\n");
    printfooter(fp, ihtmlfooterfile, set_label, set_dir,
		subject, summary_filename);
    datename = save_datename;
    thrdname = save_thrdname;
    subjname = save_subjname;
    authname = save_authname;
}

void write_summary_indices(int amount_new)
{
	indextypefilename[DATE_INDEX] = datename;
	indextypefilename[THREAD_INDEX] = thrdname;
	indextypefilename[SUBJECT_INDEX] = subjname;
	indextypefilename[AUTHOR_INDEX] = authname;
	indextypename[DATE_INDEX] = lang[MSG_BY_DATE];
	indextypename[THREAD_INDEX] = lang[MSG_BY_THREAD];
	indextypename[SUBJECT_INDEX] = lang[MSG_BY_SUBJECT];
	indextypename[AUTHOR_INDEX] = lang[MSG_BY_AUTHOR];

	if (set_monthly_index || set_yearly_index) {
		char filename[512];
		FILE *fp;
		sprintf(filename,"%s%ssummary.%s", set_dir,
			(set_dir[strlen(set_dir) - 1] == '/') ? "" : "/",
			set_htmlsuffix);
		fp = fopen(filename,"w");
		if (!fp) {
			sprintf(errmsg, "Couldn't write \"%s\".", filename);
			progerr(NULL);
		}
		printmonths(fp, filename, amount_new);
		fclose(fp);
		chmod(filename, set_filemode);
	}
}
