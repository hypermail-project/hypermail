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

#include "proto.h"

#ifdef HAVE_DIRENT_H
#include <dirent.h>
#else
#include <sys/dir.h>
#endif

static char *indextypename[NO_INDEX];

#ifdef GDBM

#include "gdbm.h"

/*
** store a single message summary to an already open-for-write GDBM index
**/

int togdbm(void *gp, struct emailinfo *ep)
{
  datum key;
  datum content;
  char *buf;
  char *dp;
  int rval;
  int num = ep->msgnum;
  char *name = ep->name;
  char *email = ep->emailaddr;
  char *date = ep->datestr;
  char *msgid = ep->msgid;
  char *subject = ep->subject;
  char *inreply = ep->inreplyto;
  char *fromdate = ep->fromdatestr;
  char *charset = ep->charset;
  char *isodate = strsav(secs_to_iso(ep->date));
  char *isofromdate = strsav(secs_to_iso(ep->fromdate));
  char *exp_time_str = strsav(ep->exp_time == -1 ? ""
			      : secs_to_iso(ep->exp_time));
  char is_deleted_str[32];
  trio_snprintf(is_deleted_str, sizeof(is_deleted_str), "%d", ep->is_deleted);

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
			     strlen(exp_time_str) + strlen(is_deleted_str) +
			     12))) {
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
  strcpy(dp, exp_time_str);
  dp += strlen(dp) + 1;
  strcpy(dp, is_deleted_str);
  dp += strlen(dp) + 1;
  content.dsize = dp - buf;
  content.dptr = buf; /* the value is in this string */
  rval = gdbm_store((GDBM_FILE) gp, key, content, GDBM_REPLACE);
  free(buf);
  free(exp_time_str);
  free(isodate);
  free(isofromdate);
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

#ifdef FASTREPLYCODE
    for (rp = threadlist_by_msgnum[msgnum];
	 (rp != NULL) && (rp->msgnum != msgnum); rp = rp->next) {
	;
    }
#else
    for (rp = threadlist; (rp != NULL) && (rp->msgnum != msgnum);
	 rp = rp->next) {
	;
    }
#endif

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

void fprint_menu(FILE *fp, mindex_t idx, char *archives, char *currentid,
		 char *cursub, int pos, struct emailsubdir *subdir)
{
    char *ptr;
    int dlev = (subdir != NULL);
    int i;
    int count_l = 0;
#if DEBUG_HTML
    printcomment(fp, "fprint_menu", "begin");
#endif
    fprintf(fp,
	    "<div class=\"center\">\n<table border=\"2\" width=\"100%%\" class=\"links\">\n<tr>\n");

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

    if (set_show_index_links && set_show_index_links != (pos == PAGE_TOP ? 4 : 3)) {
        if (idx != NO_INDEX && !set_reverse) {
	    if (pos == PAGE_TOP)
	        fprintf(fp, "<th><a href=\"#end\">%s</a></th>\n",
			lang[MSG_END_OF_MESSAGES]);
	    else
	        fprintf(fp, "<th><a name=\"end\" href=\"#\">%s</a></th>\n",
			lang[MSG_START_OF_MESSAGES]);
	}

	for (i = 0; i <= AUTHOR_INDEX; ++i) {
	    if (idx != i && show_index[dlev][i]) {
	        fprintf(fp, "<th><a href=\"%s\">%s</a></th>\n",
			index_name[dlev][i], lang[MSG_DATE_VIEW + i]);
		++count_l;
	    }
	}

	if (show_index[dlev][ATTACHMENT_INDEX]) {
	    if (idx != ATTACHMENT_INDEX) {
	        fprintf(fp, "<th><a href=\"%s\">%s</a></th>\n",
			index_name[dlev][ATTACHMENT_INDEX],
			lang[MSG_ATTACHMENT_VIEW]);
		++count_l;
	    }
	}
	if (subdir && idx != NO_INDEX) {
	    int f_cols = (subdir->prior_subdir != NULL)
	      + (subdir->next_subdir != NULL);
	    const char *colspan = 2*f_cols <= count_l ? " colspan=\"2\"": "";
	    fprintf(fp, "</tr><tr>");
	    if (subdir->prior_subdir)
	        fprintf(fp, "<th%s><a href=\"%s%s%s\">%s, %s</a></th>",
			colspan, subdir->rel_path_to_top,
			subdir->prior_subdir->subdir, index_name[dlev][idx],
			lang[MSG_PREV_DIRECTORY], lang[MSG_DATE_VIEW + idx]);
	    if (subdir->next_subdir)
	        fprintf(fp, "<th%s><a href=\"%s%s%s\">%s, %s</a></th>",
			colspan, subdir->rel_path_to_top,
			subdir->next_subdir->subdir, index_name[dlev][idx],
			lang[MSG_NEXT_DIRECTORY], lang[MSG_DATE_VIEW + idx]);
	    if (show_index[0][FOLDERS_INDEX])
	        fprintf(fp, "<th><a href=\"%s%s\">%s</a></th>",
			subdir->rel_path_to_top, index_name[0][FOLDERS_INDEX],
			lang[MSG_FOLDERS_INDEX]);
	}
    }

    if (archives && *archives)
	fprintf(fp, "<th><a href=\"%s\">%s</a></th>\n", archives,
		lang[MSG_OTHER_MAIL_ARCHIVES]);

    fprintf(fp, "</tr>\n</table>\n</div>\n");
#if DEBUG_HTML
    printcomment(fp, "fprint_menu", "end");
#endif
}

/*----------------------------------------------------------------------------*/

void fprint_summary(FILE *fp, int pos, long first_d, long last_d, int num)
{
#if DEBUG_HTML
    printcomment(fp, "fprint_summary", "begin");
#endif
    fprintf(fp, "<div class=\"center\">\n");
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
#if DEBUG_HTML
    printcomment(fp, "fprint_summary", "end");
#endif
}

/*----------------------------------------------------------------------------*/

void print_index_header_links(FILE *fp, mindex_t called_from, 
			      long startdatenum, long enddatenum,
			      int amountmsgs, struct emailsubdir *subdir)
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
    int dlev = (subdir != NULL);

#if DEBUG_HTML
    printcomment(fp, "index_header_links", "begin");
#endif
    fprintf(fp, "<p>\n");
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

    if ((called_from != AUTHOR_INDEX && show_index[dlev][AUTHOR_INDEX]) ||
        (called_from != DATE_INDEX &&  show_index[dlev][DATE_INDEX]) ||
        (called_from != THREAD_INDEX && show_index[dlev][THREAD_INDEX]) ||
        (called_from != SUBJECT_INDEX && show_index[dlev][SUBJECT_INDEX]))
           fprintf(fp, "<strong>%d %s %s:</strong> \n", amountmsgs,
               lang[MSG_ARTICLES], lang[MSG_SORTED_BY]);

    if (called_from != AUTHOR_INDEX && show_index[dlev][AUTHOR_INDEX])
	fprintf(fp, "<a href=\"%s\">[ %s ]</a>\n", 
		index_name[dlev][AUTHOR_INDEX], lang[MSG_AUTHOR]);

    if (called_from != DATE_INDEX && show_index[dlev][DATE_INDEX])
	fprintf(fp, "<a href=\"%s\">[ %s ]</a>\n",
		index_name[dlev][DATE_INDEX], lang[MSG_DATE]);

    if (called_from != THREAD_INDEX && show_index[dlev][THREAD_INDEX])
	fprintf(fp, "<a href=\"%s\">[ %s ]</a>\n",
		index_name[dlev][THREAD_INDEX], lang[MSG_THREAD]);

    if (called_from != SUBJECT_INDEX && show_index[dlev][SUBJECT_INDEX])
	fprintf(fp, "<a href=\"%s\">[ %s ]</a>\n",
		index_name[dlev][SUBJECT_INDEX], lang[MSG_SUBJECT]);

    if (set_attachmentsindex) {
	if (called_from != ATTACHMENT_INDEX) {
	    fprintf(fp, "<a href=\"%s\">[ %s ]</a>\n",
		    index_name[dlev][ATTACHMENT_INDEX], lang[MSG_ATTACHMENT]);
	}
    }

    if (subdir) {
	fprintf(fp, "<br>");
	if (subdir->prior_subdir)
	    fprintf(fp, " <a href=\"%s%s%s\">%s, %s</a> | ",
		    subdir->rel_path_to_top, subdir->prior_subdir->subdir,
		    index_name[dlev][called_from], lang[MSG_PREV_DIRECTORY],
		    lang[MSG_DATE_VIEW + called_from]);
	if (subdir->next_subdir)
            fprintf(fp, " <a href=\"%s%s%s\">%s, %s</a> | ",
		    subdir->rel_path_to_top, subdir->next_subdir->subdir,
		    index_name[dlev][called_from], lang[MSG_NEXT_DIRECTORY],
		    lang[MSG_DATE_VIEW + called_from]);
	if (show_index[0][FOLDERS_INDEX])
	    fprintf(fp, " <a href=\"%s%s\">%s</a>",
		    subdir->rel_path_to_top, index_name[0][FOLDERS_INDEX],
		    lang[MSG_FOLDERS_INDEX]);
    }

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

    if (set_showhr)
        fprintf(fp, "</p><hr>\n");
    else
        fprintf(fp, "</p>\n");

    /*
     * Printout the Dates for the Starting and Ending messages 
     * in the archive, along with a count of the messages.
     */
    fprintf(fp, "<p>\n");
    fprintf(fp, "<strong>%s:</strong> <em>%s</em><br>\n",
            lang[MSG_STARTING], getdatestr(startdatenum));

    fprintf(fp, "<strong>%s:</strong> <em>%s</em><br>\n",
            lang[MSG_ENDING], getdatestr(enddatenum));
    fprintf(fp, "</p>\n");
#if DEBUG_HTML
    printcomment(fp, "index_header_links", "end");
#endif
}

void print_index_footer_links(FILE *fp, mindex_t called_from,
			      long enddatenum, int amountmsgs,
			      struct emailsubdir *subdir)
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
     int dlev = (subdir != NULL);

#if DEBUG_HTML
    printcomment(fp, "index_footer_links", "begin");
#endif
    fprintf(fp, "<p>\n");
    fprintf(fp, "<a name=\"end\"><strong>%s:</strong></a> <em>%s</em><br>\n",
		lang[MSG_LAST_MESSAGE_DATE], getdatestr(lastdatenum));

    fprintf(fp, "<strong>%s:</strong> <em>%s</em>\n",
                lang[MSG_ARCHIVED_ON], getlocaltime());

    if (set_showhr)
        fprintf(fp, "</p><hr>\n");
    else
        fprintf(fp, "</p>\n");

    fprintf(fp, "<p>\n");
    if ((called_from != AUTHOR_INDEX && show_index[dlev][AUTHOR_INDEX]) ||
        (called_from != DATE_INDEX &&  show_index[dlev][DATE_INDEX]) ||
        (called_from != THREAD_INDEX && show_index[dlev][THREAD_INDEX]) ||
        (called_from != SUBJECT_INDEX && show_index[dlev][SUBJECT_INDEX]))
           fprintf(fp, "<strong>%d %s %s:</strong> \n", amountmsgs,
               lang[MSG_ARTICLES], lang[MSG_SORTED_BY]);

    if (called_from != AUTHOR_INDEX && show_index[dlev][AUTHOR_INDEX])
	fprintf(fp, "<a href=\"%s\">[ %s ]</a>\n",
		index_name[dlev][AUTHOR_INDEX], lang[MSG_AUTHOR]);

    if (called_from != DATE_INDEX && show_index[dlev][DATE_INDEX])
	fprintf(fp, "<a href=\"%s\">[ %s ]</a>\n",
		index_name[dlev][DATE_INDEX], lang[MSG_DATE]);

    if (called_from != THREAD_INDEX && show_index[dlev][THREAD_INDEX])
	fprintf(fp, "<a href=\"%s\">[ %s ]</a>\n",
		index_name[dlev][THREAD_INDEX], lang[MSG_THREAD]);

    if (called_from != SUBJECT_INDEX && show_index[dlev][SUBJECT_INDEX])
	fprintf(fp, "<a href=\"%s\">[ %s ]</a>\n",
		index_name[dlev][SUBJECT_INDEX], lang[MSG_SUBJECT]);

    if (set_attachmentsindex) {
	if (called_from != ATTACHMENT_INDEX) {
	    fprintf(fp, "<a href=\"%s\">[ %s ]</a>\n",
		    index_name[dlev][ATTACHMENT_INDEX], lang[MSG_ATTACHMENT]);
	}
    }

    if (subdir) {
	fprintf(fp, "<br>");
	if (subdir->prior_subdir)
	    fprintf(fp, " <a href=\"%s%s%s\">%s, %s</a>",
		    subdir->rel_path_to_top, subdir->prior_subdir->subdir,
		    index_name[dlev][called_from], lang[MSG_PREV_DIRECTORY],
		    lang[MSG_DATE_VIEW + called_from]);
	if (subdir->next_subdir)
	    fprintf(fp, " <a href=\"%s%s%s\">%s, %s</a>",
		    subdir->rel_path_to_top, subdir->next_subdir->subdir,
		    index_name[dlev][called_from], lang[MSG_NEXT_DIRECTORY],
		    lang[MSG_DATE_VIEW + called_from]);
	if (show_index[0][FOLDERS_INDEX])
	    fprintf(fp, " <a href=\"%s%s\">%s</a>",
		    subdir->rel_path_to_top, index_name[0][FOLDERS_INDEX],
		    lang[MSG_FOLDERS_INDEX]);
    }

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
    fprintf(fp, "</p>\n");
#if DEBUG_HTML
    printcomment(fp, "index_footer_links", "end");
#endif
}

/*----------------------------------------------------------------------------*/

void print_haof_indices(FILE *fp, struct emailsubdir *subdir)
{
    int dlev = (subdir != NULL);

    fprintf(fp, "    <indices>\n");

    if (show_index[dlev][DATE_INDEX])
    	fprintf(fp,"       <dateindex>%s</dateindex>\n",
				index_name[dlev][DATE_INDEX]);

    if (show_index[dlev][AUTHOR_INDEX])
    	fprintf(fp,"       <subjectindex>%s</subjectindex>\n",
				index_name[dlev][AUTHOR_INDEX]);

    if (show_index[dlev][THREAD_INDEX])
        fprintf(fp,"       <threadindex>%s</threadindex>\n",
				index_name[dlev][THREAD_INDEX]);

    if (show_index[dlev][SUBJECT_INDEX])
    	fprintf(fp,"       <authorindex>%s</authorindex>\n",
				index_name[dlev][AUTHOR_INDEX]);

    if (show_index[dlev][ATTACHMENT_INDEX])
    	fprintf(fp,"       <attachmentindex>%s</attachmentindex>\n",
				index_name[dlev][ATTACHMENT_INDEX]);

    fprintf(fp, "    </indices>\n\n");
}

/*
** Prints a comment in the file.
*/

void printcomment(FILE *fp, char *label, char *value)
{
    if (value && *value) {
	/* we only save the non-NULL ones */
	char *ext_value = value;
	char *ptr = strchr(value, '@');
	if (set_spamprotect_id && ptr && (!strcmp(label, "id")
				       || !strcmp(label, "inreplyto"))) {
	    struct Push retbuf;
	    INIT_PUSH(retbuf);
	    PushNString(&retbuf, value, ptr - value);
	    PushNString(&retbuf, "_at_", 4);
	    PushString(&retbuf, ptr + 1);
	    ext_value = PUSH_STRING(retbuf);
	}
	fprintf(fp, "<!-- %s=\"%s\" -->\n", label, ext_value);
	if (ext_value != value)
	    free(ext_value);
    }
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
void printdates(FILE *fp, struct header *hp, int year, int month,
	       struct emailinfo *subdir_email)
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
    printdates(fp, hp->left, year, month, subdir_email);
    if ((year == -1 || year_of_datenum(em->date) == year)
	&& (month == -1 || month_of_datenum(em->date) == month)
	&& !em->is_deleted
	&& (!subdir_email || subdir_email->subdir == em->subdir)) {
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
	endline = "</li>";
	subj_tag = "<strong>";
	subj_end_tag = "</strong>";
      }
      fprintf(fp,"%s%s%s%s%s</a>%s<a name=\"%d\"><em>%s</em></a>%s%s%s%s\n",
	      startline, msg_href(em, subdir_email),
	      subj_tag, subj, subj_end_tag, break_str, em->msgnum, em->name,
	      break_str, date_str, line_count_str, endline);
      free(subj);
    }
    printdates(fp, hp->right, year, month, subdir_email);
  }
}

/*
** Pretty-prints the files with attachments in the index files.
*/
void printattachments(FILE *fp, struct header *hp,
		      struct emailinfo *subdir_email)
{
    char *subj;
    char *attdir;
    const char *rel_path_to_top = (subdir_email ?
				   subdir_email->subdir->rel_path_to_top : "");

    if (hp != NULL) {
	struct emailinfo *em = hp->data;
	printattachments(fp, hp->left, subdir_email);
	if ((!subdir_email || subdir_email->subdir == em->subdir)
	    && !em->is_deleted) {
	    subj = convchars(em->subject);

	    /* See if there's a directory corresponding to this message */
	    trio_asprintf(&attdir, "%s%c" DIR_PREFIXER "%04d",
			  set_dir, PATH_SEPARATOR, em->msgnum);
	    if (isdir(attdir)) {
	        DIR *dir = opendir(attdir);

		if (set_indextable) {
		    fprintf(fp, "<tr><td>%s%s</a></td><td><em>%s</em></a></td>"
			    "<td>%s</td></tr>\n", msg_href(em, subdir_email),
			    subj, em->name, getdatestr(em->date));
		}
		else {
		    fprintf(fp, "<li>%s<strong>%s</strong></a>&nbsp;"
			    "<a name=\"%d\"><em>%s</em></a>&nbsp;<em>(%s)</em>\n",
			    msg_href(em, subdir_email), subj,
			    em->msgnum, em->name, getdatestr(em->date));
		}
		if (dir) {
#ifdef HAVE_DIRENT_H
		    struct dirent *entry;
#else
		    struct direct *entry;
#endif
		    struct stat fileinfo;
		    char *filename, *stripped_filename;
		    const char *fmt2 =
		      (set_indextable ? 
		       "<tr><td>&nbsp;&nbsp;&nbsp;&nbsp;<a href=\"%s%s\">%s</a></td>"
		       "<td colspan=\"2\" align=\"center\">(%d %s)</td></tr>\n"
		       : "<li><a href=\"%s%s\">%s</a> (%d %s)\n");

		    int first_time = 1;
		    while ((entry = readdir(dir))) {
		        int file_size = -1;
			if (!strcmp(".", entry->d_name) ||
			    !strcmp("..", entry->d_name)) continue;
			if (first_time && !set_indextable) {
			    first_time = 0;
			    fprintf(fp, "<ol>\n");
			}
			trio_asprintf(&filename, "%s%c%s", attdir,
				      PATH_SEPARATOR, entry->d_name);
			if (!stat(filename, &fileinfo))
			    file_size = fileinfo.st_size;
			free(filename);
			trio_asprintf(&filename, DIR_PREFIXER "%04d%c%s",
				      em->msgnum,
				      PATH_SEPARATOR, entry->d_name);
			stripped_filename = strchr(entry->d_name, '-');
			if (stripped_filename)
			    fprintf(fp, fmt2, rel_path_to_top,
				    filename, stripped_filename+1,
				    file_size, lang[MSG_BYTES]);
			free(filename);
		    }
		    if (!first_time && !set_indextable) {
		        fprintf(fp, "</ol>\n");
		    }
		    closedir(dir);
		}
	    }
	    free(attdir);
	    free(subj);
	}
	printattachments(fp, hp->right, subdir_email);
    }
}

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
    char *tmpline1;
    char *c;
    struct Push buff;
    struct emailinfo *ep = hashreplylookup(-1, inreply, mailsubject, &subjmatch);

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
    PushString(&buff, msg_href(ep, ep));
    PushString(&buff, inreply);
    PushString(&buff, "</a>");
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

void printbody(FILE *fp, struct emailinfo *email, int maybe_reply)
{
    int insig, inblank;
    struct body *bp = email->bodylist;
    char *id = email->msgid;
    char *subject = email->subject;
    int msgnum = email->msgnum;
    char inheader = FALSE;	/* we always start in a mail header */
    int pre = FALSE;

    int inquote;
    int quote_num;
    int quoted_percent;
    bool replace_quoted;
    /* if (set_quote_hide_threshold < 100 && !email->is_deleted) */
    if (set_quote_hide_threshold <= 100)
	quoted_percent = compute_quoted_percent(bp);
    else
	quoted_percent = 100;
    replace_quoted = (quoted_percent > set_quote_hide_threshold);

    if(set_showprogress && replace_quoted)
      printf("\nMessage %d quoted text (%d %%) replaced by links\n",
	     msgnum,quoted_percent);

    if (set_showhr)
	fprintf(fp, "<hr>\n");
    printcomment(fp, "body", "start");

    if (email->is_deleted && set_delete_level != DELETE_LEAVES_TEXT
	&& !(email->is_deleted == 2
	     && set_delete_level == DELETE_LEAVES_EXPIRED_TEXT)) {
        int d_index = MSG_DELETED;
	if (email->is_deleted == 2) d_index = MSG_EXPIRED;
	if (email->is_deleted == 4 || email->is_deleted == 8)
	    d_index = MSG_FILTERED_OUT;
        fprintf(fp, lang[d_index]);
	fprintf(fp, "</p>\n");
	printcomment(fp, "body", "end");
	if (set_showhr)
	    fprintf(fp, "<hr>\n");
	return;
    }

    if (!set_showhtml) {
	fprintf(fp, "<pre>\n");
	pre = TRUE;
    }

    if (set_showhtml == 2)
      init_txt2html();
    inquote = 0;
    quote_num = 0;

    inblank = 1;
    insig = 0;

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
	    if (sscanf(bp->line, "%127[^:]", head) == 1 &&
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
		} else {
		  if (!pre) {
			fprintf(fp, "<pre>\n");
			pre = TRUE;
		  }
		}
		inheader = FALSE;
	    }
	}

	if (((bp->line)[0] != '\n') && (bp->header && !set_showheaders)) {
	    bp = bp->next;
	    continue;
	}

	if (set_showhtml == 2 && !inheader) {
	    txt2html(fp, email, bp, replace_quoted, maybe_reply);
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

	if (set_showhtml) {
	    if (is_sig_start(bp->line)) {
		insig = 1;
		if (!pre) {
		    fprintf(fp, "<pre>\n");
		    pre = TRUE;
		}
	    }

	    if (!inheader && (bp->line)[0] == '\n')
        /* within the <pre></pre> statements you do not need to
           insert <p> statements since text is already preformated.
           the W3C HTML validation script fails for such pages 
                Akis Karnouskos <akis@ceid.upatras.gr>     */
                {
                if (!pre)
                        fprintf(fp, "<p>");
                }
	    else {
		if (insig) {
		    ConvURLs(fp, bp->line, id, subject);
		}
		else if (isquote(bp->line)) {
		    if(set_linkquotes) {
			if(handle_quoted_text(fp, email, bp, bp->line, inquote,
					      quote_num, 
					      replace_quoted, maybe_reply)) {
			    ++quote_num;
			    inquote = 1;
			}
		    }
		    else {
			fprintf(fp, "<%s class=\"%s\">",
				set_iquotes ? "em" : "span",
				find_quote_class(bp->line));

			ConvURLs(fp, bp->line, id, subject);

			fprintf(fp, "%s<br>\n",
				(set_iquotes) ? "</em>" : "</span>");
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

    printcomment(fp, "body", "end");
    if (set_showhr)
	fprintf(fp, "<hr>\n");
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

static int
has_new_replies(struct emailinfo *email)
{
    struct reply *rp;
    static int max_old_msgnum = -2;
    if (max_old_msgnum == -2)
	max_old_msgnum = find_max_msgnum();
    if (email->msgnum == max_old_msgnum)
	return 1;  /* next msg needs to be linked, even if no new reply */
#ifdef FASTREPLYCODE
    for (rp = email->replylist; rp != NULL; rp = rp->next) {
	if (rp->frommsgnum == email->msgnum && rp->msgnum > max_old_msgnum)
	    return 1;
    }
#endif
    return 0;
}

/*
 * Perform deletions on old messages when run in incremental mode.
 */

void update_deletions(int num_old)
{
    struct hmlist *tlist;
    struct reply *rp;
    int save_ov = set_overwrite;
    set_overwrite = TRUE;
    for (tlist = set_delete_msgnum; tlist != NULL; tlist = tlist->next) {
	struct emailinfo *ep;
	int num = atoi(tlist->val);
	if (num >= num_old)
	    continue;		/* new message - already done */
	if (hashnumlookup(num, &ep)) {
	    char *filename = articlehtmlfilename(ep);
	    if (set_delete_level != DELETE_REMOVES_FILES) {
		struct body *bp = ep->bodylist;
		if (bp == NULL)
		    parse_old_html(num, ep, 1, 0, NULL);
		writearticles(num, num + 1);
	    }
	    else if (isfile(filename)) {
		unlink(filename);
	    }
	    free(filename);
#ifdef FASTREPLYCODE
	    for (rp = ep->replylist; rp != NULL; rp = rp->next) {
#else
	    for (rp = replylist; rp != NULL; rp = rp->next) {
#endif
	        int rnum = rp->data->msgnum;
		if (rnum < num_old) {
		    if (!rp->data->bodylist || !rp->data->bodylist->line[0])
		        parse_old_html(rnum, rp->data, TRUE, FALSE, NULL);
		    writearticles(rnum, rnum + 1);/* update MSG_IN_REPLY_TO line */
		}
	    }
	}
    }
    set_overwrite = save_ov;
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

    struct body *bp;
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

	if (set_folder_by_date && set_increment && !is_empty_archive()) {
		sprintf(errmsg, "Cannot open or create file \"%s\". Unable to "
			"do\nincremental updates with the folder_by_date "
			"option without using that file.", indexname);
		progerr(errmsg);
	}
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

    while (num < maxnum) {

	char *filename;
	if ((bp = hashnumlookup(num, &email)) == NULL) {
	    ++num;
	    continue;
	}
	filename = articlehtmlfilename(email);
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
	if (email->is_deleted && set_delete_level == DELETE_REMOVES_FILES) {
	    if (!newfile) {
		unlink(filename);
	    }
#ifdef GDBM
	    else if(gp) {
	        togdbm((void *) gp, email);
	    }
#endif
	    ++num;
	    free(filename);
	    continue;
	}
	else if (!newfile && !set_overwrite && !has_new_replies(email)
		 && !(email->is_deleted && set_delete_msgnum)) {
	    skip = 1;		/* is this really necessary with continue ??? */
	    num++;
	    free(filename);
	    continue;
	}
	else {
	    if ((fp = fopen(filename, "w")) == NULL) {
		sprintf(errmsg, "%s \"%s\".", lang[MSG_COULD_NOT_WRITE],
			filename);
		progerr(errmsg);
	    }
	}

	email_next_in_thread = nextinthread(email->msgnum);

	/*
	 * Create the comment fields necessary for incremental updating
	 */

	print_msg_header(fp, set_label,
			 email->subject, set_dir,
			 email->name, email->emailaddr, email->msgid,
			 email->charset, email->date, filename);

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
	if (email->is_deleted) {
	    char num_buf[32];
	    sprintf(num_buf, "%d", email->is_deleted);
	    printcomment(fp, "isdeleted", num_buf);
	}
	printcomment(fp, "expires", email->exp_time == -1 ? "-1"
		     : secs_to_iso(email->exp_time));
#ifdef GDBM
	if(gp) {
	  togdbm((void *) gp, email);
	}
#endif
	if (ptr)
	    free(ptr);

	if (set_usetable) {
	    fprint_menu(fp, NO_INDEX, "",
			email->msgid, email->subject, PAGE_TOP, email->subdir);
	}

	/*
	 * Print the message's author info and date.
	 */
	fprintf(fp, "<p class=\"headers\">\n");
	if (!strcmp(email->name, email->emailaddr)) {
	    if (use_mailcommand) {
		ptr = makemailcommand(set_mailcommand,
				      email->emailaddr,
				      email->msgid, email->subject);
		fprintf(fp, "<strong>%s:</strong> <a href=\"%s\">", lang[MSG_FROM], ptr ? ptr : "");
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
		fprintf(fp, "<strong>%s:</strong> %s (<a href=\"%s\">",
			lang[MSG_FROM], email->name, ptr ? ptr : "");
		if (ptr)
		    free(ptr);
		fprintf(fp, "<em>%s</em></a>)<br>\n", email->emailaddr);
	    }
	    else
	    {
		fprintf(fp,
			"<strong>%s:</strong> %s (<em>%s</em>)<br>\n",
			lang[MSG_FROM], email->name, 
			(strcmp(email->emailaddr,"(no email)")!=0) ? email->emailaddr : "no email");
	    }
	}
	fprintf(fp, "<strong>%s:</strong> %s\n", lang[MSG_CDATE],
		getdatestr(email->date));
	fprintf(fp, "</p>\n");

	/*
	 * This is here because it looks better here. The table looks
	 * better before the Author info. This stuff should be in 
	 * printfile() so it could be laid out as the user wants...
	 */

	/*
	 * Should we print message links ?
	 */

	if (set_show_msg_links && set_show_msg_links != 4) {
	    printcomment(fp, "next", "start");
	    fprintf(fp, "<ul class=\"links\">\n");

	    /*
	     * Is there a next message?
	     */

	    email2 = neighborlookup(num, 1);
	    if (email2) {
		fprintf(fp, "<li><strong>%s:</strong> ",
			lang[MSG_NEXT_MESSAGE]);

		ptr = convchars(email2->subject);
		fprintf(fp, "%s%s: \"%s\"</a>\n",
			msg_href(email2, email), email2->name, ptr ? ptr : "");
		if (ptr)
		    free(ptr);
	    }

	    /*
	     * Is there a previous message?
	     */

	    email2 = neighborlookup(num, -1);
	    if (email2) {
		fprintf(fp, "<li><strong>%s:</strong> ",
			lang[MSG_PREVIOUS_MESSAGE]);
		fprintf(fp, "%s%s: \"%s\"</a>\n", msg_href(email2, email),
			email2->name, ptr = convchars(email2->subject));
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
				    email->subject, &subjmatch);
		if (email2) {
		    char *del_msg = (email2->is_deleted ? lang[MSG_DEL_SHORT]
				     : "");
		    is_reply = 1;
		    if (subjmatch)
			fprintf(fp, "<li><strong>%s:</strong>",
				lang[MSG_MAYBE_IN_REPLY_TO]);
		    else
			fprintf(fp, "<li><strong>%s:</strong>",
				lang[MSG_IN_REPLY_TO]);
		    fprintf(fp, "%s %s%s: \"%s\"</a>\n", del_msg,
			    msg_href(email2, email), email2->name,
			    ptr = convchars(email2->subject));
		    if (ptr)
			free(ptr);
		}
	    }

	    /*
	     * Is there a message next in the thread?
	     */
	    printcomment(fp, "nextthread", "start");
	    if (email_next_in_thread) {
		fprintf(fp, "<li><strong>%s:</strong> ",
			lang[MSG_NEXT_IN_THREAD]);
		fprintf(fp, "%s%s: \"%s\"</a>\n",
			msg_href(email_next_in_thread, email),
			email_next_in_thread->name,
			ptr = convchars(email_next_in_thread->subject));
		if (ptr)
		    free(ptr);
		email->initial_next_in_thread = email_next_in_thread->msgnum;
	    }

	    /*
	     * Does this message have replies? If so, print them all!
	     */

	    if (set_showreplies) {
#ifdef FASTREPLYCODE
	        for (rp = email->replylist; rp != NULL; rp = rp->next) {
                    if (hashnumlookup(rp->msgnum, &email2)) {
#else
		for (rp = replylist; rp != NULL; rp = rp->next) {
                    if (rp->frommsgnum == num
                        && hashnumlookup(rp->msgnum, &email2)) {
#endif
		        char *del_msg = (email2->is_deleted
					 ? lang[MSG_DEL_SHORT] : "");
			if (rp->maybereply)
			    fprintf(fp, "<li><strong>%s:</strong>",
				    lang[MSG_MAYBE_REPLY]);
			else
			    fprintf(fp, "<li><strong>%s:</strong>",
				    lang[MSG_REPLY]);
			fprintf(fp, "%s %s", del_msg, msg_href(email2, email));
                        fprintf(fp, "%s: \"%s\"</a>\n", email2->name,
                                ptr = convchars(email2->subject));
			free(ptr);
		    }
		}
		printcomment(fp, "reply", "end");
	    }
	}

	if (!set_usetable) {
            int dlev = (email->subdir != NULL);
	    if (!(set_show_msg_links && set_show_msg_links != 4))
		fprintf(fp, "<ul class=\"links\">\n");
	    if (set_show_index_links && set_show_index_links != 4) {
	        fprintf(fp,"<li><strong>%s %s:</strong> \n",
			lang[MSG_MESSAGES], lang[MSG_SORTED_BY]);
	        if (show_index[dlev][DATE_INDEX])
		    fprintf(fp, "<a href=\"%s#%d\">[ %s ]</a>\n",
			    index_name[dlev][DATE_INDEX], num, lang[MSG_DATE]);
		if (show_index[dlev][THREAD_INDEX])
		    fprintf(fp, "<a href=\"%s#%d\">[ %s ]</a>\n",
			    index_name[dlev][THREAD_INDEX], num, lang[MSG_THREAD]);
		if (show_index[dlev][SUBJECT_INDEX])
		    fprintf(fp, "<a href=\"%s#%d\">[ %s ]</a>\n",
			    index_name[dlev][SUBJECT_INDEX], num, lang[MSG_SUBJECT]);
		if (show_index[dlev][AUTHOR_INDEX])
		    fprintf(fp, "<a href=\"%s#%d\">[ %s ]</a>\n",
			    index_name[dlev][AUTHOR_INDEX], num, lang[MSG_AUTHOR]);
		if (show_index[dlev][ATTACHMENT_INDEX]) {
		    fprintf(fp, "<a href=\"%s\">[ %s ]</a>\n",
			    index_name[dlev][ATTACHMENT_INDEX],
			    lang[MSG_ATTACHMENT]);
		}
	    }

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

	if ((set_show_msg_links && set_show_msg_links != 4) || !set_usetable)
	    fprintf(fp, "</ul>\n");

	if (set_linkquotes || set_showhtml == 2)
	  /* should be changed to unconditional after tested for a while?
	     - pcm@rahul.net 1999-09-09 */
	    find_quote_prefix(email->bodylist, is_reply);

	/*
	 * Finally...print the body!
	 */

	printbody(fp, email, maybe_reply);

	/*
	 * Should we print out the message links ?
	 */

	if (set_show_msg_links && set_show_msg_links != 3) {
	  /* JK: removed a <p>\n here */
	    fprintf(fp, "<ul class=\"links\">\n");
	    printcomment(fp, "next", "start");
	    email2 = neighborlookup(num, 1);

	    if (email2 != NULL) {
		fprintf(fp, "<li><strong>%s:</strong> ",
			lang[MSG_NEXT_MESSAGE]);
		fprintf(fp, "%s%s: \"%s\"</a>\n", msg_href(email2, email),
			email2->name, ptr = convchars(email2->subject));
		free(ptr);
	    }

	    email2 = neighborlookup(num, -1);
	    if (set_linkquotes && old_reply_to
		&& num - 1 == (get_new_reply_to() == -1 ? old_reply_to->msgnum
			       : get_new_reply_to()))
	        email2 = NULL;
	    if (email2) {
		fprintf(fp, "<li><strong>%s:</strong> ",
			lang[MSG_PREVIOUS_MESSAGE]);
		fprintf(fp, "%s%s: \"%s\"</a>\n", msg_href(email2, email),
			email2->name, ptr = convchars(email2->subject));
		free(ptr);
	    }

	    if (*email->inreplyto) {
		email2 =
		    hashreplylookup(email->msgnum, email->inreplyto,
				    email->subject, &subjmatch);
		if (email2) {
		    char *del_msg = (email2->is_deleted ? lang[MSG_DEL_SHORT]
				     : "");
		    if (subjmatch)
			fprintf(fp, "<li><strong>%s:</strong>",
				lang[MSG_MAYBE_IN_REPLY_TO]);
		    else
			fprintf(fp, "<li><strong>%s:</strong>",
				lang[MSG_IN_REPLY_TO]);
		    fprintf(fp, "%s %s", del_msg, msg_href(email2, email));
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
		fprintf(fp, "%s", msg_href(email_next_in_thread, email));
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
		        char *del_msg = (email2->is_deleted
					 ? lang[MSG_DEL_SHORT] : "");
			if (rp->maybereply)
			    fprintf(fp, "<li><strong>%s:</strong>",
				    lang[MSG_MAYBE_REPLY]);
			else
			    fprintf(fp, "<li><strong>%s:</strong>",
				    lang[MSG_REPLY]);
			fprintf(fp, "%s %s", del_msg, msg_href(email2, email));
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

	if (set_usetable) {
	    fprint_menu(fp, NO_INDEX, "", email->msgid, email->subject,
			PAGE_BOTTOM, email->subdir);
	} else {
	      int dlev = (email->subdir != NULL);
	      if (!(set_show_msg_links && set_show_msg_links != 3))
		  fprintf(fp, "<ul>\n");
	      if (set_show_index_links && set_show_index_links != 3) {
		  fprintf(fp,"<li><strong>%s %s:</strong> \n",
			  lang[MSG_MESSAGES], lang[MSG_SORTED_BY]);
		  if (show_index[dlev][DATE_INDEX])
		      fprintf(fp,"<a href=\"%s#%d\">[ %s ]</a>\n",
			      index_name[dlev][DATE_INDEX], num, lang[MSG_DATE]);
		  if (show_index[dlev][THREAD_INDEX])
		      fprintf(fp,"<a href=\"%s#%d\">[ %s ]</a>\n",
			      index_name[dlev][THREAD_INDEX], num,
			      lang[MSG_THREAD]);
		  if (show_index[dlev][SUBJECT_INDEX])
		      fprintf(fp,"<a href=\"%s#%d\">[ %s ]</a>\n",
			      index_name[dlev][SUBJECT_INDEX], num,
			      lang[MSG_SUBJECT]);
		  if (show_index[dlev][AUTHOR_INDEX])
		      fprintf(fp,"<a href=\"%s#%d\">[ %s ]</a>\n",
			      index_name[dlev][AUTHOR_INDEX], num,
			      lang[MSG_AUTHOR]);
		  if (show_index[dlev][ATTACHMENT_INDEX]) {
		      fprintf(fp, "<a href=\"%s\">[ %s ]</a>\n",
			      index_name[dlev][ATTACHMENT_INDEX], lang[MSG_ATTACHMENT]);
		  }
	      }
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

	if (set_txtsuffix) {
	    fprintf(fp, "<p><a href=\"%.4d.%s\">%s</a>",
		    email->msgnum, set_txtsuffix, lang[MSG_TXT_VERSION]);
	}

	printfooter(fp, mhtmlfooterfile, set_label, set_dir,
		    email->subject, filename);

	fclose(fp);

	if (get_new_reply_to() != -1) {
	  	/* will only be true if set_linkquotes is */
	    struct emailinfo *e3, *e4;
	    int was_correct = 0;
	    replace_maybe_replies(filename, email, get_new_reply_to());
	    for (rp = replylist; rp != NULL; rp = rp->next) {
	        /* get rid of old guesses for where this links */
	        if (rp->msgnum == num) {
#ifdef FASTREPLYCODE
		    struct reply *rp3;
		    was_correct = (rp->frommsgnum == get_new_reply_to());
		    if (was_correct)
		        break;
		    hashnumlookup(get_new_reply_to(), &e4);
		    hashnumlookup(rp->frommsgnum, &e3);
		    for (rp3 = e3->replylist; rp3 != NULL && rp3->next != NULL;
			 rp3 = rp3->next) {
		        if (rp3->next->msgnum == num) {
			    rp3->next = rp3->next->next; /* remove */
			}
		    }
		    e4->replylist = addreply(e4->replylist, e4->msgnum,
					     email, 0, NULL);
#endif
		    rp->frommsgnum = get_new_reply_to();
		    rp->maybereply = 0;
		    break; /* revisit me */
		}
	    }
	    if (!rp) {
		if(hashnumlookup(num, &e3)) {
#ifdef FASTREPLYCODE
		    hashnumlookup(get_new_reply_to(), &e4);
		    replylist = addreply2(replylist, e4, e3, 0,
					  &replylist_end);
#else
		    replylist = addreply(replylist, get_new_reply_to(), e3, 0,
					 &replylist_end);
#endif
		}
	    }
	    if (!was_correct)
	        fixreplyheader(set_dir, num, TRUE, num);
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
	free(filename);

	num++;
    }

#ifdef GDBM
    if(gp) {
        datum key;
	datum content;
	int nkey = -1;
	char num_buf[32];
	key.dsize = sizeof(nkey); /* the key -1 is for the last msgnum */
	key.dptr = (char *) &nkey;
	sprintf(num_buf, "%d", max_msgnum);
	content.dsize = strlen(num_buf) + 1;
	content.dptr = num_buf;
	gdbm_store((GDBM_FILE) gp, key, content, GDBM_REPLACE);
	gdbm_close(gp);
    }
#endif

    if (set_showprogress)
	printf("\b\b\b\b    \n");
} /* end writearticles() */

/*
** Write the date index...
** If email != NULL, write index for the subdir in which that email is.
*/

void writedates(int amountmsgs, struct emailinfo *email)
{
    int newfile;
    char *filename;
    FILE *fp;
    char *datename = index_name[email && email->subdir != NULL][DATE_INDEX];

    filename = htmlfilename(datename, email, "");

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
				 amountmsgs, email ? email->subdir : NULL);
    else {
	fprint_menu(fp, DATE_INDEX, set_archives, "", "", PAGE_TOP,
		    email ? email->subdir : NULL);
	fprint_summary(fp, PAGE_TOP, firstdatenum, lastdatenum, amountmsgs);
	if (set_showhr)
	    fprintf(fp, "<hr>\n"); 
    }

    /*
     * Print out the actual message index lists. Here's the beef.
     */
    if (set_indextable)
	fprintf(fp,
		"<div class=\"center\">\n<table width=\"80%%\">\n<tr><td><u><strong>%s</strong></u></td><td><u><strong>%s</strong></u></td><td><u><strong>%s</strong></u></td></tr>\n",
		lang[MSG_CSUBJECT], lang[MSG_CAUTHOR], lang[MSG_CDATE]);
    else
	fprintf(fp, "<ul>\n");

    printdates(fp, datelist, -1, -1, email);
    
    if (set_indextable)
	fprintf(fp, "</table>\n</div>\n");
    else
	fprintf(fp, "</ul>\n");

    if (!set_usetable) {
	/* 
	 * Print out archive information links at the bottom of the index
	 */
	print_index_footer_links(fp, DATE_INDEX, lastdatenum, amountmsgs,
				 email ? email->subdir : NULL);
    }
    else {
	if (set_showhr)
	    fprintf(fp, "<hr>\n");
	fprint_summary(fp, PAGE_BOTTOM, firstdatenum, lastdatenum, amountmsgs);
	fprint_menu(fp, DATE_INDEX, set_archives, "", "", PAGE_BOTTOM,
		    email ? email->subdir : NULL);
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
    free(filename);

    if (set_showprogress)
	putchar('\n');
}

/*
** Write the attachments index...
*/

void writeattachments(int amountmsgs, struct emailinfo *email)
{
    int newfile;
    char *filename;
    FILE *fp;
    char *attname = index_name[email && email->subdir != NULL][ATTACHMENT_INDEX];

    filename = htmlfilename(attname, email, "");

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
	print_index_header_links(fp, ATTACHMENT_INDEX, firstdatenum,
				 lastdatenum, amountmsgs,
				 email ? email->subdir : NULL);
    else {
	fprint_menu(fp, ATTACHMENT_INDEX, set_archives, "", "", PAGE_TOP, NULL);
	fprint_summary(fp, PAGE_TOP, firstdatenum, lastdatenum, amountmsgs);
	if (set_showhr)
	    fprintf(fp, "<hr>\n"); 
    }

    /*
     * Print out the actual message index lists. Here's the beef.
     */

    if (set_indextable) {
	fprintf(fp,
		"<div class=\"center\">\n<table width=\"80%%\">\n<tr><td><u><strong>%s</strong></u></td><td><u><strong>%s</strong></u></td><td><u><strong>%s</strong></u></td></tr>\n",
		lang[MSG_CSUBJECT], lang[MSG_CAUTHOR], lang[MSG_CDATE]);
	printattachments(fp, datelist, email);
	fprintf(fp, "</table>\n</div>\n");
    }
    else {
	fprintf(fp, "<ul>\n");
	printattachments(fp, datelist, email);
	fprintf(fp, "</ul>\n");
    }

    if (!set_usetable) {
	/* 
	 * Print out archive information links at the bottom of the index
	 */
	print_index_footer_links(fp, ATTACHMENT_INDEX, lastdatenum, amountmsgs,
				 email ? email->subdir : NULL);
    }
    else {
	if (set_showhr)
	    fprintf(fp, "<hr>\n");
	fprint_summary(fp, PAGE_BOTTOM, firstdatenum, lastdatenum, amountmsgs);
	fprint_menu(fp, ATTACHMENT_INDEX, set_archives, "", "", PAGE_BOTTOM,
		    email ? email->subdir : NULL);
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
    free(filename);

    if (set_showprogress)
	putchar('\n');
}

/*
** Write the thread index...
*/

void writethreads(int amountmsgs, struct emailinfo *email)
{
    int newfile;
    char *filename;
    FILE *fp;
    char *thrdname = index_name[email && email->subdir != NULL][THREAD_INDEX];

    struct printed *pp;

    while (printedlist) {	/* cleanup needed ?? */
	pp = printedlist;
	printedlist = printedlist->next;
	free(pp);
    }

    printedlist = NULL;

    filename = htmlfilename(thrdname, email, "");

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
				 amountmsgs, email ? email->subdir : NULL);
    }
    else {
	fprint_menu(fp, THREAD_INDEX, set_archives, "", "", PAGE_TOP,
		    email ? email->subdir : NULL);
	fprint_summary(fp, PAGE_TOP, firstdatenum, lastdatenum, amountmsgs);
	if (set_showhr)
	    fprintf(fp, "<hr>\n");
    }

    if (set_indextable) {
	fprintf(fp,
		"<div class=\"center\">\n<table>\n<tr><td><u><strong>%s</strong></u></td><td><u><strong>%s</strong></u></td><td><u><strong> %s</strong></u></td></tr>\n",
		lang[MSG_CSUBJECT], lang[MSG_CAUTHOR], lang[MSG_CDATE]);
	print_all_threads(fp, -1, -1, email);
	fprintf(fp, "</table>\n</div>\n");
    }
    else {
	fprintf(fp, "<ul>\n");
	print_all_threads(fp, -1, -1, email);
	fprintf(fp, "</ul>\n");
    }

    if (!set_usetable) {
	/* 
	 * Print out archive information links at the bottom of the index
	 */
        print_index_footer_links(fp, THREAD_INDEX, lastdatenum, amountmsgs,
				 email ? email->subdir : NULL);
    }
    else {
	if (set_showhr)
	    fprintf(fp, "<hr>\n");
	fprint_summary(fp, PAGE_BOTTOM, firstdatenum, lastdatenum, amountmsgs);
	fprint_menu(fp, THREAD_INDEX, set_archives, "", "", PAGE_BOTTOM,
		    email ? email->subdir : NULL);
    }

    printfooter(fp, ihtmlfooterfile, set_label, set_dir,
		lang[MSG_BY_THREAD], thrdname);

    fclose(fp);

    if (newfile && chmod(filename, set_filemode) == -1) {
	sprintf(errmsg, "%s \"%s\": %o.",
		lang[MSG_CANNOT_CHMOD], filename, set_filemode);
	progerr(errmsg);
    }
    free(filename);

    if (set_showprogress)
	putchar('\n');
}

/*
** Print the subject index pointers alphabetically.
*/

void printsubjects(FILE *fp, struct header *hp, char **oldsubject,
		   int year, int month, struct emailinfo *subdir_email)
{
  char *subj;
  const char *startline;
  const char *break_str;
  const char *endline;
  char date_str[DATESTRLEN+11];
  char line_count_str[50];

  if (hp != NULL) {
    printsubjects(fp, hp->left, oldsubject, year, month, subdir_email);
    if ((year == -1 || year_of_datenum(hp->data->date) == year)
	&& (month == -1 || month_of_datenum(hp->data->date) == month)
	&& !hp->data->is_deleted
	&& (!subdir_email || subdir_email->subdir == hp->data->subdir)) {
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
	    endline = "</li></ul>";
	}
	fprintf(fp,
		"%s%s%s</a>%s <a name=\"%d\">%s</a>%s%s\n", startline,
		msg_href(hp->data, subdir_email), hp->data->name, break_str,
		hp->data->msgnum, date_str, line_count_str, endline);
	*oldsubject = hp->data->unre_subject;
      
	free(subj);
    }
    printsubjects(fp, hp->right, oldsubject, year, month, subdir_email);
  }
}

/*
** Prints the subject index.
*/

void writesubjects(int amountmsgs, struct emailinfo *email)
{
    int newfile;
    char *filename;
    FILE *fp;
    char *subjname = index_name[email && email->subdir != NULL][SUBJECT_INDEX];

    filename = htmlfilename(subjname, email, "");

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
				 amountmsgs, email ? email->subdir : NULL);
    else {
	fprint_menu(fp, SUBJECT_INDEX, set_archives, "", "", PAGE_TOP,
		    email ? email->subdir : NULL);
	fprint_summary(fp, PAGE_TOP, firstdatenum, lastdatenum, amountmsgs);
	if (set_showhr)
	    fprintf(fp, "<hr>\n");
    }

    if (set_indextable) {
	fprintf(fp,
		"<div class=\"center\">\n<table>\n<tr><td><u><strong>%s</strong></u></td><td><u><strong>%s</strong></u></td><td><u><strong> %s</strong></u></td></tr>\n",
		lang[MSG_CSUBJECT], lang[MSG_CAUTHOR], lang[MSG_CDATE]);
    }
    else {
	fprintf(fp, "<ul>\n");
    }
    {
	char *oldsubject = "";	/* dummy to start with */
	printsubjects(fp, subjectlist, &oldsubject, -1, -1, email);
    }
    if (set_indextable) {
	fprintf(fp, "</table>\n</div>\n");
    }
    else {
	fprintf(fp, "</ul>\n");
    }

    /* 
     * Print out archive information links at the bottom of the index
     */

    if (!set_usetable) 
	print_index_footer_links(fp, SUBJECT_INDEX, lastdatenum, amountmsgs,
				 email ? email->subdir : NULL);
    else {
	if (set_showhr)
	    fprintf(fp, "<hr>\n");
	fprint_summary(fp, PAGE_BOTTOM, firstdatenum, lastdatenum, amountmsgs);
	fprint_menu(fp, SUBJECT_INDEX, set_archives, "", "", PAGE_BOTTOM,
		    email ? email->subdir : NULL);
    }

    printfooter(fp, ihtmlfooterfile, set_label, set_dir,
		lang[MSG_BY_SUBJECT], subjname);

    fclose(fp);

    if (newfile && chmod(filename, set_filemode) == -1) {
	sprintf(errmsg, "%s \"%s\": %o.",
		lang[MSG_CANNOT_CHMOD], filename, set_filemode);
	progerr(errmsg);
    }
    free(filename);

    if (set_showprogress)
	putchar('\n');
}

/*
** Prints the author index links sorted alphabetically.
*/

void printauthors(FILE *fp, struct header *hp, char **oldname,
		  int year, int month, struct emailinfo *subdir_email)
{
  char *subj;
  const char *startline;
  const char *break_str;
  const char *endline;
  char date_str[DATESTRLEN+11];
  char line_count_str[50];

  if (hp != NULL) {
    printauthors(fp, hp->left, oldname, year, month, subdir_email);
    if ((year == -1 || year_of_datenum(hp->data->date) == year)
	&& (month == -1 || month_of_datenum(hp->data->date) == month)
	&& !hp->data->is_deleted
	&& (!subdir_email || subdir_email->subdir == hp->data->subdir)) {

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
	endline = "</li></ul>";
      }
      fprintf(fp,"%s%s%s</a>%s<a name=\"%d\">%s</a>%s%s\n",
	      startline, msg_href(hp->data, subdir_email), subj, break_str,
	      hp->data->msgnum, date_str, line_count_str, endline);
      free(subj);

      *oldname = hp->data->name;	/* avoid copying */
    }
    printauthors(fp, hp->right, oldname, year, month, subdir_email);
  }
}

/*
** Prints the author index file and links sorted alphabetically.
*/

void writeauthors(int amountmsgs, struct emailinfo *email)
{
    int newfile;
    char *filename;
    FILE *fp;
    char *authname = index_name[email && email->subdir != NULL][AUTHOR_INDEX];

    filename = htmlfilename(authname, email, "");

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
				 amountmsgs, email ? email->subdir : NULL);
    }
    else {
	fprint_menu(fp, AUTHOR_INDEX, set_archives, "", "", PAGE_TOP,
		    email ? email->subdir : NULL);
	fprint_summary(fp, PAGE_TOP, firstdatenum, lastdatenum, amountmsgs);
	if (set_showhr)
	    fprintf(fp, "<hr>\n");
    }

    if (set_indextable) {
	fprintf(fp,
		"<div class=\"center\">\n<table>\n<tr><td><u><strong>%s</strong></u></td><td><u><strong>%s</strong></u></td><td><u><strong> %s</strong></u></td></tr>\n",
		lang[MSG_CAUTHOR], lang[MSG_CSUBJECT], lang[MSG_CDATE]);
    }
    else {
	fprintf(fp, "<ul>\n");
    }
    {
	char *prevauthor = "";
	printauthors(fp, authorlist, &prevauthor, -1, -1, email);
    }
    if (set_indextable) {
	fprintf(fp, "</table>\n</div>\n");
    }
    else {
	fprintf(fp, "</ul>\n");
    }

    /* 
     * Print out archive information links at the bottom 
     * of the index page
     */

    if (!set_usetable) 
	print_index_footer_links(fp, AUTHOR_INDEX, lastdatenum, amountmsgs,
				 email ? email->subdir : NULL);
    else {
	if (set_showhr)
	    fprintf(fp, "<hr>\n");
	fprint_summary(fp, PAGE_BOTTOM, firstdatenum, lastdatenum, amountmsgs);
	fprint_menu(fp, AUTHOR_INDEX, set_archives, "", "", PAGE_BOTTOM,
		    email ? email->subdir : NULL);
    }

    printfooter(fp, ihtmlfooterfile, set_label, set_dir,
		lang[MSG_BY_AUTHOR], authname);

    fclose(fp);

    if (newfile && chmod(filename, set_filemode) == -1) {
	sprintf(errmsg, "%s \"%s\": %o.",
		lang[MSG_CANNOT_CHMOD], filename, set_filemode);
	progerr(errmsg);
    }
    free(filename);

    if (set_showprogress)
	putchar('\n');
}

/*
** Pretty-prints the items for the haof
*/
void printhaofitems(FILE *fp, struct header *hp, int year, int month,
	       struct emailinfo *subdir_email)
{
  char *subj, *from_name, *from_emailaddr;

  if (hp != NULL) {
    struct emailinfo *em=hp->data;
    printhaofitems(fp, hp->left, year, month, subdir_email);
    if ((year == -1 || year_of_datenum(em->date) == year)
	&& (month == -1 || month_of_datenum(em->date) == month)
	&& !em->is_deleted
	&& (!subdir_email || subdir_email->subdir == em->subdir)) {

      subj = convchars(em->subject);
      from_name = convchars(em->name);
      from_emailaddr = convchars(em->emailaddr);

      fprintf(fp,
         "      <mail>\n"
         "        <subject>%s</subject>\n"
         "        <date>%s</date>\n"
         "        <fromname>%s</fromname>\n"
         "        <fromemail>%s</fromemail>\n" 
         "        <message-id>%s</message-id>\n"
         "        <file>\"%s\"</file>\n"
         "      </mail>\n\n"
         ,subj, getdatestr(em->date), from_name, from_emailaddr, em->msgid,
         msg_relpath(em,subdir_email));

      free(subj);
      free(from_name);
      free(from_emailaddr);
    }
    printhaofitems(fp, hp->right, year, month, subdir_email);
  }
}

/*
** Write the XML based hypermail archive overview file
*/

void writehaof(int amountmsgs, struct emailinfo *email)
{
    int newfile;
    char *filename;
    FILE *fp;
    char *authname = index_name[email && email->subdir != NULL][AUTHOR_INDEX];

    filename = haofname(email);
    printf("haofname %s\n", filename);

    if (isfile(filename))
	newfile = 0;
    else
	newfile = 1;

    if ((fp = fopen(filename, "w")) == NULL) {
	sprintf(errmsg, "%s \"%s\".", lang[MSG_COULD_NOT_WRITE], filename);
	progerr(errmsg);
    }

    if (set_showprogress)
	printf("%s \"%s\"...", lang[MSG_WRITING_HAOF], filename);

    fprintf(fp,"<?xml version=\"1.0\" encoding=\"ISO-8859-15\"?>\n\n");
    fprintf(fp,"  <!DOCTYPE haof PUBLIC "
        "\"-//Bernhard Reiter//DTD HOAF 0.2//EN\"\n" 
        "\"http://ffii.org/~breiter/probe/haof-0.2.dtd\">\n\n"
          );
    fprintf(fp,"  <haof version=\"0.2\">\n\n");
    fprintf(fp,"      <archiver name=\"hypermail\" version=\"" 
		    		VERSION ".pl" PATCHLEVEL "\" />\n\n");

    print_haof_indices(fp, email ? email->subdir : NULL);

	    
    fprintf(fp,"  <mails>\n");
    printhaofitems(fp, datelist, -1, -1, email);
    fprintf(fp,"  </mails>\n");

    fprintf(fp,"  </haof>\n");

    fclose(fp);

    if (newfile && chmod(filename, set_filemode) == -1) {
	sprintf(errmsg, "%s \"%s\": %o.",
		lang[MSG_CANNOT_CHMOD], filename, set_filemode);
	progerr(errmsg);
    }
    free(filename);

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
	    && (mo == -1 || month_of_datenum(em->date) == mo)
	    && !em->is_deleted) {
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
    char *save_name[NO_INDEX];
    char *subject = lang[set_monthly_index ? MSG_MONTHLY_INDEX
			: MSG_YEARLY_INDEX];
    for (j = 0; j <= AUTHOR_INDEX; ++j)
	save_name[j] = index_name[0][j];
    print_index_header(fp, set_label, set_dir, subject, summary_filename);
    fprintf(fp, "<table>\n");
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
		sprintf(period_bufs[j], "%sby%s", month_str, save_name[j]);
		index_name[0][j] = period_bufs[j];
	    }
	    for(j = 0; j <= AUTHOR_INDEX; ++j) {
		char *filename;
		char buf1[MAXFILELEN];
		FILE *fp1;
		char *prev_text = "";
		char subject_title[128];
		if (!show_index[0][j])
		    continue;
		sprintf(buf1, "%sby%s", month_str, save_name[j]);
		filename = htmlfilename(buf1, NULL, "");
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
					     count, NULL);
		}
		else {
		    fprint_menu(fp1, j, set_archives, "", "", PAGE_TOP, NULL);
		    fprint_summary(fp1, PAGE_TOP, first_date, last_date,
				   count);
		    if (set_showhr)
		        fprintf(fp1, "<hr>\n");
		}

		if (set_indextable) {
		    fprintf(fp1,
			    "<div class=\"center\">\n<table>\n<tr><td><u><strong>%s</strong></u></td><td><u><strong>%s</strong></u></td><td><u><strong> %s</strong></u></td></tr>\n",
			    lang[j == AUTHOR_INDEX ? MSG_CAUTHOR : MSG_CSUBJECT],
			    lang[j == AUTHOR_INDEX ? MSG_CSUBJECT : MSG_CAUTHOR],
			    lang[MSG_CDATE]);
		}
		else {
		    fprintf(fp1, "<ul>\n");
		}
		switch(j) {
		    case DATE_INDEX:
		        printdates(fp1, datelist, y, m, NULL);
			break;
		    case THREAD_INDEX:
		        print_all_threads(fp1, y, m, NULL);
			break;
		    case SUBJECT_INDEX:
		        printsubjects(fp1, subjectlist, &prev_text, y, m,NULL);
			break;
		    case AUTHOR_INDEX:
		        printauthors(fp1, authorlist, &prev_text, y, m, NULL);
			break;
		}

		if (set_indextable) {
		    fprintf(fp1, "</table>\n</div>\n");
		}
		else {
		    fprintf(fp1, "</ul>\n");
		}

		/* 
		 * Print out archive information links at the bottom 
		 * of the index page
		 */

		if (!set_usetable)
		    print_index_footer_links(fp1, j, last_date, count, NULL);
		else {
		    if (set_showhr)
		        fprintf(fp1, "<hr>\n");
		    fprint_summary(fp1, PAGE_BOTTOM, first_date, last_date,
				   count);
		    fprint_menu(fp1, j, set_archives, "", "", PAGE_BOTTOM,
				NULL);
		}

		printfooter(fp1, ihtmlfooterfile, set_label, set_dir,
			    subject_title, save_name[j]);
		fclose(fp1);
		if (!count) {
		    remove(filename);
		    if(started_line)
		        fprintf(fp, "<td></td>");
		    else ++empties;
		}
		else {
		    if (!started_line) {
		        fprintf(fp, "<tr><td>%s</td><td>%d %s</td>",
				month_str_pub, count, lang[MSG_ARTICLES]);
			while(empties--)
			    fprintf(fp, "<td></td>");
			started_line = 1;
		    }
		    chmod(filename, set_filemode);
		    fprintf(fp, "<td><a href=\"%sby%s\">%s</a></td>",
			    month_str, save_name[j], indextypename[j]);
		}
		free(filename);
	    }
	    if (started_line) fprintf(fp, "</tr>\n");
	}
    }
    fprintf(fp,"</table>\n");
    printfooter(fp, ihtmlfooterfile, set_label, set_dir,
		subject, summary_filename);
    for (j = 0; j <= AUTHOR_INDEX; ++j)
	index_name[0][j] = save_name[j];
}

void
init_index_names()
{
    indextypename[DATE_INDEX] = lang[MSG_BY_DATE];
    indextypename[THREAD_INDEX] = lang[MSG_BY_THREAD];
    indextypename[SUBJECT_INDEX] = lang[MSG_BY_SUBJECT];
    indextypename[AUTHOR_INDEX] = lang[MSG_BY_AUTHOR];
    indextypename[ATTACHMENT_INDEX] = lang[MSG_ATTACHMENT];
}

void write_summary_indices(int amount_new)
{
	if (set_monthly_index || set_yearly_index) {
		char *filename;
		FILE *fp;
		filename = htmlfilename("summary", NULL, set_htmlsuffix);
		fp = fopen(filename,"w");
		if (!fp) {
			sprintf(errmsg, "Couldn't write \"%s\".", filename);
			progerr(NULL);
		}
		printmonths(fp, filename, amount_new);
		fclose(fp);
		chmod(filename, set_filemode);
		free(filename);
	}
}

void write_toplevel_indices(int amountmsgs)
{
    int j, newfile;
    struct emailsubdir *sd;
    char *subject = lang[MSG_FOLDERS_INDEX];
    char *filename;
    FILE *fp;

    filename = htmlfilename(index_name[0][FOLDERS_INDEX] , NULL, "");

    if (isfile(filename))
	newfile = 0;
    else
	newfile = 1;

    if (!show_index[0][FOLDERS_INDEX])
	fp = NULL;
    else if ((fp = fopen(filename, "w")) == NULL) {
	sprintf(errmsg, "%s \"%s\".", lang[MSG_COULD_NOT_WRITE], filename);
	progerr(errmsg);
    }
    if (fp) {
	print_index_header(fp, set_label, set_dir, subject, filename);
	if (!set_usetable) 
            print_index_header_links(fp, FOLDERS_INDEX, firstdatenum,
				     lastdatenum, amountmsgs, NULL);
	else {
	    fprint_menu(fp, FOLDERS_INDEX, set_archives, "", "", PAGE_TOP, NULL);
	    fprint_summary(fp, PAGE_TOP, firstdatenum, lastdatenum, amountmsgs);
	    if (set_showhr)
	        fprintf(fp, "<hr>\n"); 
	}
	fprintf(fp, "<table>\n");
    }
    for (sd = folders; sd != NULL; sd = sd->next_subdir) {
	int started_line = 0;
	int empties = 0;
	if (!datelist->data)
	    continue;
	for(j = 0; j <= ATTACHMENT_INDEX; ++j) {
	    if (!show_index[1][j])
		continue;
	    switch(j) {
		case DATE_INDEX:
		    writedates(sd->count, sd->first_email);
		    break;
	        case THREAD_INDEX:
		    writethreads(sd->count, sd->first_email);
		    break;
	        case SUBJECT_INDEX:
		    writesubjects(sd->count, sd->first_email);
		    break;
		case AUTHOR_INDEX:
		    writeauthors(sd->count, sd->first_email);
		    break;
		case ATTACHMENT_INDEX:
		    writeattachments(sd->count, sd->first_email);
		    break;
	    }
	if (set_writehaof)
			writehaof(sd->count, sd->first_email);

	    if (!fp) continue;
	    if (!sd->count) {
		if(started_line)
		    fprintf(fp, "<td></td>");
		else ++empties;
	    }
	    else {
		if (!started_line) {
		    fprintf(fp, "<tr><td>%s</td><td>%d %s</td>",
			sd->description, sd->count, lang[MSG_ARTICLES]);
		    while(empties--)
			fprintf(fp, "<td></td>");
		    started_line = 1;
		}
		fprintf(fp, "<td><a href=\"%s%s\">%s</a></td>",
			sd->subdir, index_name[1][j], indextypename[j]);
	    }
	}
	if (started_line && fp) fprintf(fp, "</tr>\n");
    }
    if (fp) {
	fprintf(fp,"</table>\n");

	if (!set_usetable) {
	    /* 
	     * Print out archive information links at the bottom of the index
	     */
	    print_index_footer_links(fp, FOLDERS_INDEX, lastdatenum,
				     amountmsgs, NULL);
	}
	else {
	    if (set_showhr)
	        fprintf(fp, "<hr>\n");
	    fprint_summary(fp, PAGE_BOTTOM, firstdatenum, lastdatenum, amountmsgs);
	    fprint_menu(fp, FOLDERS_INDEX, set_archives, "", "", PAGE_BOTTOM, NULL);
	}
	printfooter(fp, ihtmlfooterfile, set_label, set_dir, subject, filename);
	fclose(fp);

	if (newfile && chmod(filename, set_filemode) == -1) {
	    sprintf(errmsg, "%s \"%s\": %o.",
		    lang[MSG_CANNOT_CHMOD], filename, set_filemode);
	    progerr(errmsg);
	}
    }
    free(filename);
}
