/*
** Copyright (C) 1997, Peter McCluskey (pcm@rahul.net)
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
#include "finelink.h"
#include "print.h"
#include "struct.h"
#include "search.h"
#include "setup.h"
#include "proto.h"
#include <string.h>
#include <ctype.h>


static int str_similar_str(const char *str1, const char *str2);
static char *stripwhitespace(const char *line);
static int new_reply_to = -1;

static int found_quote = 0;

static struct body *place_anchor(const String_Match * match_info,
				 struct body *bp, char *buffer, FILE *fp2,
				 char **ptr, const char *anchor)
{
    if (match_info) {
	int index;
	char token[MAXLINE];
	char *tptr;
	char *ptr1 = buffer;
	char *last_ptr0 = *ptr;
	char *penultimate_ptr0 = last_ptr0;
	char *last_ptr = ptr1;
	char *penultimate_ptr = ptr1;
	if (!bp)
	    return FALSE;
	while ((bp = tokenize_body(bp, token, ptr, &index, TRUE)) != NULL) {
	    if (*ptr == match_info->start_match) {
		strcpy(token, last_ptr);
		*last_ptr = 0;
		fprintf(fp2, "%s<a name=\"%s\">", buffer, anchor);
		strcpy(buffer, token);
		*ptr = last_ptr0;
		return bp;
	    }
	    penultimate_ptr = ptr1;
	    last_ptr = penultimate_ptr;
	    last_ptr0 = penultimate_ptr0;
	    penultimate_ptr0 = *ptr;

	    if (!(tptr = strstr(ptr1, *ptr))) {
		int len = (4 * strlen(*ptr)) / 5;
		char *temp1 = (char *)emalloc(len + 1);
		strncpy(temp1, *ptr, len);
		temp1[len] = 0;
		tptr = strstr(ptr1, temp1);
		free(temp1);
		if (!tptr)
		    break;
	    }
	    ptr1 = tptr;
	    while (ispunct(*ptr1) && *ptr1 != '<')
		++ptr1;
	}
	if (0)
	    printf("No match found %s; %s", anchor, buffer);
    }
    fprintf(fp2, "<a name=\"%s\">", anchor);
    return bp;
}

static int
place_a_end(const String_Match * match_info, struct body **bp,
	    char *buffer, FILE *fp2, char **ptr)
{
    int index;
    char token[MAXLINE];
    char *ptr1 = buffer;
    char *last_ptr = ptr1;
    char *tptr;
    if (!*bp)
	return FALSE;
    while ((*bp = tokenize_body(*bp, token, ptr, &index, TRUE)) != NULL) {
	if (*ptr == match_info->stop_match) {
	    strcpy(token, ptr1);
	    *ptr1 = 0;
	    fprintf(fp2, "%s</a>%s", buffer, token);
	    return TRUE;
	}
	last_ptr = ptr1;
	if (!(tptr = strstr(ptr1, *ptr))) {
	    int len = (4 * strlen(*ptr)) / 5;
	    char *temp1 = (char *)emalloc(len + 1);
	    strncpy(temp1, *ptr, len);
	    temp1[len] = 0;
	    tptr = strstr(ptr1, temp1);
	    free(temp1);
	    if (!tptr)
		return FALSE;
	}
	ptr1 = tptr;
	while (ispunct(*ptr1) && *ptr1 != '<')
	    ++ptr1;
    }
    return FALSE;
}

/*
** Alter the message being quoted to add an anchor specifying the quoted text
*/

static int
add_anchor(int msgnum, int quoting_msgnum, int quote_num, const char *dir,
	   const char *anchor, char *line, int find_substr,
	   int count_quoted_lines, const String_Match * match_info)
{
    char filename[MAXFILELEN];
    char tmpfilename[MAXFILELEN];
    char buffer[MAXLINE];
    FILE *fp1, *fp2;
    int matches = 0;
    int in_body = FALSE;
    int cmp_len = strlen(line);
    struct emailinfo *ep;
    struct body *bp = hashnumlookup(msgnum, &ep);
    char *ptr;
    char *tmpptr;
    while (bp && (!strncmp(bp->line, "Date:", 5)
		  || !strncmp(bp->line, "From:", 5)
		  || !strncmp(bp->line, "From:", 5)
		  || !strncasecmp(bp->line, "Message-Id:", 11)
		  || !strncmp(bp->line, "Subject:", 8)
		  || !strncasecmp(bp->line, "In-Reply-To:", 12)
		  || !strncasecmp(bp->line, "References:", 11)
		  || !strncmp(bp->line, "To:", 3)))
	bp = bp->next;		/* skip over header info */
    ptr = bp ? bp->line : NULL;
    while (ptr && !*ptr) {
	bp = bp->next;
	if (!bp)
	    break;
	ptr = bp->line;
    }
#if 1
    sprintf(filename, "%s%s%.4d.%s",
	    dir, (dir[strlen(dir) - 1] == '/') ? "" : "/",
	    msgnum, set_htmlsuffix);
    sprintf(tmpfilename, "%s%s%.4d.tmp",
	    dir, (dir[strlen(dir) - 1] == '/') ? "" : "/", msgnum);
#else
    articlehtmlfilename(filename, dir, msgnum);
    articlefilename(tmpfilename, dir, msgnum, ".tmp");
#endif
    if ((fp1 = fopen(filename, "r")) == NULL) {
	if (msgnum > quoting_msgnum)
	    return 0;		/* just a forward ref */
	fprintf(stderr, "Couldn't read \"%s\". %d %d\n", filename, msgnum,
		quoting_msgnum);
	return 0;
    }
    if ((fp2 = fopen(tmpfilename, "w")) == NULL) {
	sprintf(errmsg, "Couldn't write \"%s\".", tmpfilename);
	progerr(NULL);
    }
    while (fgets(buffer, sizeof(buffer), fp1)) {
	if ((find_substr && str_similar_str(buffer, line))
	    || (!find_substr && !strncmp(buffer, line, cmp_len))) {
	    int wrote_a_end = FALSE;
	    ++matches;
	    bp = place_anchor(match_info, bp, buffer, fp2, &ptr, anchor);
	    if (set_link_to_replies)
		fprintf(fp2, "<a href=\"%.4d.%s#qlink%d\">%s</a>",
			quoting_msgnum, set_htmlsuffix,
			quote_num, set_link_to_replies);
	    /* Now find end of quoted text: */
	    do {
		if (!strcmp(buffer, "<!-- body=\"end\" -->\n")) {
		    count_quoted_lines = -1;
		    break;	/* reply quoted more lines than exist? */
		}
		if (match_info
		    && place_a_end(match_info, &bp, buffer, fp2, &ptr)) {
		    wrote_a_end = TRUE;
		    count_quoted_lines = 0;
		    if (bp)
			bp = bp->next;
		    if (bp)
			ptr = bp->line;
		    break;
		}
		fputs(buffer, fp2);
		/* surpress decrement for lines inserted by html conversion: */
		if (!strcasecmp(buffer, "<p>\n"))
		    ++count_quoted_lines;
		else {
		    char *tptr = remove_hypermail_tags(buffer);
		    if (tptr)
			free(tptr);
		    else
			++count_quoted_lines;
		}
	    } while (--count_quoted_lines > 0
		     && fgets(buffer, sizeof(buffer), fp1));
	    if (!wrote_a_end)
		fputs("</a>", fp2);
	    if (count_quoted_lines > 0)	/* got eof? */
		break;		/* avoid last fputs */
	    else if (count_quoted_lines == 0
		     && !fgets(buffer, sizeof(buffer), fp1))
		break;
	}
	else if (in_body && bp && (tmpptr = remove_hypermail_tags(buffer))) {
	    char *tmpptr2 = unconvchars(tmpptr);
	    if (tmpptr2 && str_similar_str(tmpptr, bp->line))
		bp = bp->next;
	    if (bp)
		ptr = bp->line;
	    if (tmpptr2)
		free(tmpptr2);
	    free(tmpptr);
	}
	else if (!in_body && !strcmp(buffer, "<!-- body=\"start\" -->\n"))
	    in_body = TRUE;
	fputs(buffer, fp2);
    }

    fclose(fp1);
    fclose(fp2);

    if (matches != 1)
	remove(tmpfilename);
    else {
	if (rename(tmpfilename, filename) == -1) {
	    sprintf(errmsg, "Couldn't rename \"%s\" to %s.",
		    tmpfilename, filename);
	    progerr(NULL);
	}
	if (chmod(filename, set_filemode) == -1) {
	    sprintf(errmsg, "Couldn't chmod \"%s\" to %o.",
		    filename, set_filemode);
	    progerr(NULL);
	}
    }
    return matches == 1;
}

static char *unquote_and_strip(char *line)
{
    char *p = unquote(line);
    char *p2;
    char *cvtd_line;
    while (*p && isspace(*p))
	++p;
    cvtd_line = (char *)emalloc(strlen(p) + 1);
    strcpy(cvtd_line, p);
    p2 = cvtd_line + strlen(cvtd_line);
    if (p2 > cvtd_line && p2[-1] == '\n')
	*--p2 = 0;
    if (p2 > cvtd_line && *--p2 == '-')
	*p2 = 0;		/* don't demand trailing '-' in searches; may have been unhyphenated */
    return cvtd_line;
}

/*
** Find URL of the message this line of quoted text was taken from
*/
#define MAX_ANCHOR_LEN 128

static char *url_replying_to(char *inreply, const char *dir, char *line1,	/* first line of quoted text, with html */
			     const char *line2,	/* first line of quoted text, w/o html */
			     const struct body *bp,
			     int quote_num,
			     int *quoting_msgnum,
			     int count_quoted_lines,
			     int maybe_reply, char *msgid, char *subject)
{
    static char buf[MAX_ANCHOR_LEN];
    String_Match match_info;
    char *p;
    int subjmatch = 0;
    char anchor[MAX_ANCHOR_LEN];
    int statusnum =
	hashreplynumlookup(*quoting_msgnum, inreply, subject, &subjmatch);
    sprintf(anchor, "%.4dqlink%d", *quoting_msgnum, quote_num);
    if (statusnum != -1) {
	sprintf(buf, "%.4d.%s", statusnum, set_htmlsuffix);
	if (add_anchor(statusnum, *quoting_msgnum, quote_num,
		       dir, anchor, line1, 0, count_quoted_lines, NULL)) {
	    sprintf(buf, "%.4d.%s#%s", statusnum, set_htmlsuffix, anchor);
	    if (maybe_reply)
		set_new_reply_to(statusnum, strlen(line2));
	    return buf;
	}
	if (strlen(line2) > 6 && (p = strstr(line2, "..."))) {
	    char *parsed;
	    char *tptr = (char *)emalloc(p - line2 + 1 + strlen(p + 3));
	    strncpy(tptr, line2, p - line2);
	    strcpy(tptr + (p - line2), p + 3);
	    parsed = ConvURLsString(tptr, msgid, subject);
	    free(tptr);
	    tptr = stripwhitespace(parsed ? parsed : "");
	    if (parsed)
		free(parsed);
	    if (add_anchor(statusnum, *quoting_msgnum, quote_num, dir,
			   anchor, tptr, 1, count_quoted_lines, NULL)) {
		free(tptr);
		sprintf(buf, "%.4d.%s#%s", statusnum, set_htmlsuffix,
			anchor);
		if (maybe_reply)
		    set_new_reply_to(statusnum, strlen(buf));
		return buf;
	    }
	    free(tptr);
	}
    }
    {
	int i;
	struct Push full_line;
	struct Push exact_line;
	INIT_PUSH(full_line);
	INIT_PUSH(exact_line);
	PushString(&full_line, p = stripwhitespace(line2));
	free(p);
	PushString(&exact_line, line2);
	for (i = 1; i < count_quoted_lines && (bp = bp->next); ++i) {
	    char *stripped = unquote_and_strip(bp->line);
	    PushByte(&full_line, '\n');
	    PushString(&full_line, stripwhitespace(stripped));
	    free(stripped);
	    PushString(&exact_line, unquote(bp->line));
	}
	search_for_quote(PUSH_STRING(full_line), PUSH_STRING(exact_line),
			 *quoting_msgnum, &match_info);
	free(PUSH_STRING(full_line));
	free(PUSH_STRING(exact_line));
    }
    if (match_info.msgnum >= 0) {
	char *parsed =
	    ConvURLsString(match_info.last_matched_string, msgid, subject);
	if (parsed) {
	    char *parsed2 = stripwhitespace(parsed);
	    free(parsed);
	    if (add_anchor
		(match_info.msgnum, *quoting_msgnum, quote_num, dir,
		 anchor, parsed2, 1, count_quoted_lines, &match_info)) {
		free(parsed2);
		sprintf(buf, "%.4d.%s#%s",
			match_info.msgnum, set_htmlsuffix, anchor);
		set_new_reply_to(match_info.msgnum,
				 match_info.match_len_bytes);
		return buf;
	    }
	    free(parsed2);
	}
	if (match_info.last_matched_string)
	    free(match_info.last_matched_string);
    }
    if (count_quoted_lines < 3 && strcmp(get_quote_prefix(), ">")
	&& strcmp(get_quote_prefix(), " >"))	/* was quote_prefix guess shaky? */
	*quoting_msgnum = -1;	/* msg probably doesn't have any quotes */
    return NULL;
}

/*
** handles everything we do for a single line that is a quote
** of another message
*/

int
handle_quoted_text(FILE *fp, int quoting_msgnum, const struct body *bp,
		   char *line, int inquote, int quote_num, char *inreply,
		   const char *dir, bool replace_quoted, int maybe_reply,
		   char *msgid, char *subject)
{
    char *url1;
    const struct body *last_quoted_line = bp;
    int count_quoted_lines = 0;
    const char *fmt2 = set_iquotes ? "<em>%s</em><br>" : "%s<br>";
    char *cvtd_line = ConvURLsString(unquote(line), msgid, subject);
    char *buffer1 = (char *)emalloc(strlen(cvtd_line) + strlen(fmt2) + 1);
    sprintf(buffer1, fmt2, cvtd_line ? cvtd_line : "");
    if (cvtd_line)
	free(cvtd_line);
    found_quote = (quote_num > 0);
    while (last_quoted_line && isquote(last_quoted_line->line)) {
	++count_quoted_lines;
	last_quoted_line = last_quoted_line->next;
    }
    cvtd_line = unquote_and_strip(line);
    if (strlen(cvtd_line) < 5 && (!replace_quoted || !inquote)) {
	char *parsed = ConvURLsString(line, msgid, subject);
	if (parsed) {
	    fprintf(fp, fmt2, parsed);
	    free(parsed);
	}
    }
    else if ((!inquote || !found_quote)
	     && (url1 = url_replying_to(inreply, dir, buffer1, cvtd_line,
					bp, ++quote_num, &quoting_msgnum,
					count_quoted_lines, maybe_reply,
					msgid, subject))) {
	static const char *fmt1 = "<a href=\"%s\">%s</a>%s<br>\n";
	char *tmpline;
	char *p2;
	bool replacing = replace_quoted && set_quote_link_string &&
	    set_quote_link_string[0];
	char *part2 = strcasestr(line, "<a href=");
	if (!part2) {
	    part2 = "";
	    tmpline = strsav(line);
	}
	else {
	    tmpline = (char *)emalloc(part2 - line + 1);
	    strncpy(tmpline, line, part2 - line);
	    tmpline[part2 - line] = 0;
	}
	if (set_link_to_replies)
	    fprintf(fp, "<a name=\"qlink%d\"></a>", quote_num);
	p2 = ConvURLsString(part2, msgid, subject);
	if (replacing)
	    fprintf(fp, fmt1, url1, set_quote_link_string, p2 ? p2 : "");
	else {
	    char *tmpptr = convchars(tmpline);
	    if (tmpptr) {
		fprintf(fp, fmt1, url1, tmpptr, p2 ? p2 : "");
		free(tmpptr);
	    }
	}
	free(tmpline);
	if (p2)
	    free(p2);
	free(cvtd_line);
	free(buffer1);
	return 1;
    }
    else if (!replace_quoted || !inquote) {
	char *parsed = ConvURLsString(bp->line, msgid, subject);
	if (parsed) {
	    fprintf(fp, quoting_msgnum >= 0 ? fmt2 : "%s<br>\n", parsed);
	    free(parsed);
	}
    }
    free(cvtd_line);
    free(buffer1);
    return 0;
}

/*
** Update guess about which message the current message is replying to;
** do nothing if set_new_reply_to has been called more for another msgnum since
** starting this message or if another msgnum had longer match_len.
*/

void set_new_reply_to(int msgnum, int match_len)
{
    static int alt_msgnum = -1;
    static int count_alt = 0;
    static int count_last = 0;
    static int best_match_len = -1;
    if (new_reply_to == -1 || msgnum == -1) {
	new_reply_to = msgnum;
	count_last = 1;
	count_alt = 0;
	alt_msgnum = -1;
	best_match_len = match_len;
    }
    else if (msgnum == new_reply_to) {
	++count_last;
    }
    else {
	if (msgnum == alt_msgnum || alt_msgnum == -1) {
	    if (++count_alt > count_last
		|| (count_alt == count_last && match_len > best_match_len)) {
		count_last = count_alt;
		new_reply_to = msgnum;
		if (match_len > best_match_len)
		    best_match_len = match_len;
	    }
	}
	else
	    count_alt = 0;
	alt_msgnum = msgnum;
    }
}

int get_new_reply_to()
{
    return new_reply_to;
}

/*
 * Update a message to change "maybe in reply to"'s to a single definite
 * "In reply to"
*/

void
replace_maybe_replies(const char *filename, const char *dir,
		      int new_reply_to)
{
    char tmpfilename[MAXFILELEN];
    char buffer[MAXLINE];
    FILE *fp1, *fp2;
    struct emailinfo *ep;
    int in_body = 0;
    char *ptr;
    static const char *prev_patt0 = ".html\">[ Previous ]</a>";

    if (!hashnumlookup(new_reply_to, &ep))
	return;
    sprintf(tmpfilename, "%s/aaaa.tmp", dir);
    if ((fp1 = fopen(filename, "r")) == NULL) {
	sprintf(errmsg, "Couldn't read \"%s\".", filename);
	progerr(NULL);
    }
    if ((fp2 = fopen(tmpfilename, "w")) == NULL) {
	sprintf(errmsg, "Couldn't write \"%s\".", tmpfilename);
	progerr(NULL);
    }
    while (fgets(buffer, sizeof(buffer), fp1)) {
	if (strstr(buffer, "<!-- body=\"end\" -->\n"))
	    in_body = 0;
	if (!strcmp(buffer, "<!-- body=\"start\" -->\n"))
	    in_body = 1;
	if (!in_body) {
	    if (!strcmp(buffer, "<!-- nextthread=\"start\" -->\n")) {
		char *tmpptr = convchars(ep->subject);
		if (tmpptr) {
		    fprintf(fp2, "<li> <strong>%s:</strong> "
			    "<a href=\"%.4d.%s\">%s: \"%s\"</a>\n",
			    lang[MSG_IN_REPLY_TO], new_reply_to,
			    set_htmlsuffix, ep->name,
			    tmpptr ? tmpptr : "");
		    free(tmpptr);
		}
	    }
	    else {
		static const char *patts[] = {
		    "<b>Maybe in reply to:</b>",
		    "<strong>%s:</strong>",
		    "<b>In reply to:</b>",
		    "<strong>%s:</strong>",
		    "<li> <b>Previous message:</b> <a href=\"",
		    "<li> <strong>%s:</strong> <a href=\"",
		    NULL
		};
		static const indices[] = { MSG_MAYBE_IN_REPLY_TO,
		    MSG_IN_REPLY_TO,
		    MSG_PREVIOUS_MESSAGE
		};
		int i;
		int surpress = 0;
		for (i = 0; patts[i]; ++i) {
		    char temp[256];
		    sprintf(temp, patts[i], lang[indices[i / 2]]);
		    if ((ptr = strcasestr(buffer, temp)) && (i < 4
							     ||
							     new_reply_to
							     ==
							     atoi(ptr +
								  strlen
								  (temp))))
		    {
			surpress = 1;
			break;
		    }
		}
		if (surpress)
		    continue;
	    }
	    /* check for old critmail format */
	    if ((ptr = strstr(buffer, prev_patt0))
		&& new_reply_to == atoi(ptr - 4)
		&& !strncasecmp(ptr - 13, "<a href", 7)) {
		ptr[-13] = 0;
	    }
	}
	fputs(buffer, fp2);
    }
    fclose(fp1);
    fclose(fp2);

    if (rename(tmpfilename, filename) == -1) {
	sprintf(errmsg, "Couldn't rename \"%s\" to %s.",
		tmpfilename, filename);
	progerr(NULL);
    }
}

/*
 * a fuzzy version of strstr()
*/

static int str_similar_str(const char *str1, const char *str2)
{
    int len;
    char *s;
    char *temp1;
    if (strstr(str1, str2))
	return TRUE;
    len = strlen(str2);
    if (strstr(str1, str2 + len / 5))
	return TRUE;
    len = (4 * len) / 5;
    temp1 = (char *)emalloc(len + 1);
    strncpy(temp1, str2, len);
    temp1[len] = 0;
    s = strstr(str1, temp1);
    free(temp1);
    if (s)
	return TRUE;
    return FALSE;
}

/*
** remove whitespace from begin and end of a string
*/

static char *stripwhitespace(const char *line)
{
    char *buffer = (char *)emalloc(strlen(line) + 1);
    int i = 0;
    while (*line && isspace(*line))
	++line;
    while (*line)
	buffer[i++] = *line++;
    buffer[i] = 0;
    while (i >= 0 && isspace(buffer[--i]))
	buffer[i] = '\0';
    return buffer;
}
