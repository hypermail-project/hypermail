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

static char *get_path(struct emailinfo *ep, struct emailinfo *ep2)
{
    char *path = "";
    if (ep2->subdir && ep2->subdir != ep->subdir)
       trio_asprintf(&path, "%s%s", ep2->subdir->rel_path_to_top, ep2->subdir->subdir);
    return path;
}

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

static int place_a_end(const String_Match * match_info, struct body **bp, char *buffer, FILE *fp2, char **ptr)
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

static int add_anchor(int msgnum, int quoting_msgnum, int quote_num, const char *anchor, char *line, int find_substr, int count_quoted_lines, const String_Match * match_info)
{
    char *filename;
    char *tmpfilename;
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
    filename = articlehtmlfilename(ep);
    if ((fp1 = fopen(filename, "r")) == NULL) {
	free(filename);
	if (msgnum > quoting_msgnum)
	    return 0;		/* just a forward ref */
	if (set_showprogress)
			fprintf(stderr, "Couldn't read message number %d (linked from %d). " "May mean message deleted with delete_level = 0.\n", msgnum, quoting_msgnum);
	return -1;
    }
    tmpfilename = htmlfilename("tmp", ep, "tmp");	/* AUDIT biege: where is the tmp-file created? cwd? what about checking the return-value */
    if ((fp2 = fopen(tmpfilename, "w")) == NULL) {
	snprintf(errmsg, sizeof(errmsg), "Couldn't write \"%s\".", tmpfilename);
	progerr(errmsg);
    }
    while (fgets(buffer, sizeof(buffer), fp1)) {
	if ((find_substr && str_similar_str(buffer, line))
	    || (!find_substr && !strncmp(buffer, line, cmp_len))) {
	    int wrote_a_end = FALSE;
	    ++matches;
	    bp = place_anchor(match_info, bp, buffer, fp2, &ptr, anchor);
	    if (set_link_to_replies) {
	        struct emailinfo *ep2;
		if (hashnumlookup(quoting_msgnum, &ep2)) {
		    char *path = get_path(ep, ep2);
					fprintf(fp2, "<a href=\"%s%.4d.%s#qlink%d\">%s</a>", path, quoting_msgnum, set_htmlsuffix, quote_num, set_link_to_replies);
		    if (*path)
		        free(path);
		}
	    }
	    /* Now find end of quoted text: */
	    do {
		if (!strcmp(buffer, "<!-- body=\"end\" -->\n")) {
		    count_quoted_lines = -1;
		    break;	/* reply quoted more lines than exist? */
		}
		if (match_info && place_a_end(match_info, &bp, buffer, fp2, &ptr)) {
		    wrote_a_end = TRUE;
		    count_quoted_lines = 0;
		    if (bp)
			bp = bp->next;
		    if (bp)
			ptr = bp->line;
		    break;
		}
		fputs(buffer, fp2);
		/* suppress decrement for lines inserted by html conversion: */
		if (!strcasecmp(buffer, "<p>\n"))
		    ++count_quoted_lines;
		else {
		    char *tptr = remove_hypermail_tags(buffer);
		    if (tptr)
			free(tptr);
		    else
			++count_quoted_lines;
		}
	    } while (--count_quoted_lines > 0 && fgets(buffer, sizeof(buffer), fp1));
	    if (!wrote_a_end)
		fputs("</a>", fp2);
	    if (count_quoted_lines > 0)	/* got eof? */
		break;		/* avoid last fputs */
            else if (count_quoted_lines == 0 && !fgets(buffer, sizeof(buffer), fp1))
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
	    snprintf(errmsg, sizeof(errmsg), "Couldn't rename \"%s\" to %s.", tmpfilename, filename);
	    progerr(errmsg);
	}
	if (chmod(filename, set_filemode) == -1) {
	    snprintf(errmsg, sizeof(errmsg), "Couldn't chmod \"%s\" to %o.", filename, set_filemode);
	    progerr(errmsg);
	}
    }
    free(filename);
    free(tmpfilename);
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

static char *url_replying_to(struct emailinfo *email, char *line1,	/* first line of quoted text, with html */
			     const char *line2,	/* first line of quoted text, w/o html */
			     const struct body *bp, int quote_num, int *quoting_msgnum, int count_quoted_lines, int maybe_reply)
{
    String_Match match_info;
    char *p;
    int subjmatch = 0;
    char *anchor;
    struct emailinfo *ep;
	int statusnum = hashreplynumlookup(*quoting_msgnum, email->inreplyto, email->subject,
			   &subjmatch);
    hashnumlookup(*quoting_msgnum, &ep);
    trio_asprintf(&anchor, "%.4dqlink%d", *quoting_msgnum, quote_num);
    if (statusnum != -1) {
	struct emailinfo *ep2;
	hashnumlookup(statusnum, &ep2);
		if (add_anchor(statusnum, *quoting_msgnum, quote_num, anchor, line1, 0, count_quoted_lines, NULL)) {
	    char *path = get_path(ep, ep2);
	    char *buf;
			trio_asprintf(&buf, "%s%.4d.%s#%s", path, statusnum, set_htmlsuffix, anchor);
	    if (maybe_reply)
		set_new_reply_to(statusnum, strlen(line2));
	    if (*path)
	        free(path);
	    free(anchor);
	    return buf;
	}
	if (strlen(line2) > 6 && (p = strstr(line2, "..."))) {
	    char *parsed;
	    char *tptr = (char *)emalloc(p - line2 + 1 + strlen(p + 3));	/* AUDIT biege: IOF unlikely */
	    strncpy(tptr, line2, p - line2);
	    strcpy(tptr + (p - line2), p + 3);
	    parsed = ConvURLsString(tptr, email->msgid, email->subject, email->charset);
	    free(tptr);
	    tptr = stripwhitespace(parsed ? parsed : "");
	    if (parsed)
		free(parsed);
			if (add_anchor(statusnum, *quoting_msgnum, quote_num, anchor, tptr, 1, count_quoted_lines, NULL)) {
	        char *path = get_path(ep, ep2);
		char *buf;
				trio_asprintf(&buf, "%s%.4d.%s#%s", path, statusnum, set_htmlsuffix, anchor);
		free(tptr);
		if (maybe_reply)
		    set_new_reply_to(statusnum, strlen(buf));
		if (*path)
	            free(path);
		free(anchor);
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
	    PushString(&full_line, p = stripwhitespace(stripped));
	    free(p);
	    free(stripped);
	    PushString(&exact_line, unquote(bp->line));
	}
		search_for_quote(PUSH_STRING(full_line), PUSH_STRING(exact_line), *quoting_msgnum, &match_info);
	free(PUSH_STRING(full_line));
	free(PUSH_STRING(exact_line));
    }
    if (match_info.msgnum >= 0) {
		char *parsed = ConvURLsString(match_info.last_matched_string, email->msgid,
			   email->subject, email->charset);
	if (parsed) {
	    char *parsed2 = stripwhitespace(parsed);
	    free(parsed);
	    if (add_anchor(match_info.msgnum, *quoting_msgnum, quote_num, anchor, parsed2, 1, count_quoted_lines, &match_info)) {
	        struct emailinfo *ep2;
                char *path;
                char *buf;
                hashnumlookup(match_info.msgnum, &ep2);
                path = get_path(ep, ep2);

		trio_asprintf(&buf, "%s%.4d.%s#%s", path, match_info.msgnum, set_htmlsuffix, anchor);
		set_new_reply_to(match_info.msgnum, match_info.match_len_bytes);
		free(parsed2);
		if (*path)
		    free(path);
		free(anchor);
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
    free(anchor);
    return NULL;
}

/*
** handles everything we do for a single line that is a quote
** of another message
*/

int handle_quoted_text(FILE *fp, struct emailinfo *email, const struct body *bp, char *line, int inquote, int quote_num, bool replace_quoted, int maybe_reply)
{
    char *url1;
    int quoting_msgnum = email->msgnum;
    const struct body *last_quoted_line = bp;
    int count_quoted_lines = 0;
    char *fmt2;
    char *cvtd_line = ConvURLsString(unquote(line), email->msgid, email->subject, email->charset);
    char *buffer1;
	trio_asprintf(&fmt2, set_iquotes ? "<em class=\"%s\">%%s</em><br>" : "<span class=\"%s\">%%s</span><br>", find_quote_class(line));
    trio_asprintf(&buffer1, fmt2, cvtd_line ? cvtd_line : "");
    if (cvtd_line)
	free(cvtd_line);
    found_quote = (quote_num > 0);
    while (last_quoted_line && isquote(last_quoted_line->line)) {
	++count_quoted_lines;
	last_quoted_line = last_quoted_line->next;
    }
    cvtd_line = unquote_and_strip(line);
    if (strlen(cvtd_line) < 5 && (!replace_quoted || !inquote)) {
	char *parsed = ConvURLsString(line, email->msgid, email->subject, email->charset);
	if (parsed) {
	    fprintf(fp, fmt2, parsed);
	    free(parsed);
	}
    }
    else if ((!inquote || !found_quote)
		 && (url1 = url_replying_to(email, buffer1, cvtd_line, bp, ++quote_num, &quoting_msgnum, count_quoted_lines, maybe_reply))) {
	static const char *fmt1 = "<a href=\"%s\">%s</a>%s<br>\n";
	char *tmpline;
	char *p2;
		bool replacing = replace_quoted && set_quote_link_string && set_quote_link_string[0];
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
	p2 = ConvURLsString(part2, email->msgid, email->subject, email->charset);
	if (replacing)
	    fprintf(fp, fmt1, url1, set_quote_link_string, p2 ? p2 : "");
	else {
	    char *tmpptr = convchars(tmpline, email->charset);
	    if (tmpptr) {
		fprintf(fp, fmt1, url1, tmpptr, p2 ? p2 : "");
		free(tmpptr);
	    }
	}
	free(url1);
	free(tmpline);
	if (p2)
	    free(p2);
	free(cvtd_line);
	free(buffer1);
	free(fmt2);
	return 1;
    }
    else if (!replace_quoted || !inquote) {
	char *parsed = ConvURLsString(bp->line, email->msgid, email->subject, email->charset);
	if (parsed) {
	    fprintf(fp, quoting_msgnum >= 0 ? fmt2 : "%s<br>\n", parsed);
	    free(parsed);
	}
    }
    free(cvtd_line);
    free(buffer1);
    free(fmt2);
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
			if (++count_alt > count_last || (count_alt == count_last && match_len > best_match_len)) {
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

void replace_maybe_replies(const char *filename, struct emailinfo *ep, int new_reply_to)
{
    char tmpfilename[MAXFILELEN];
    char buffer[MAXLINE];
    FILE *fp1, *fp2;
    struct emailinfo *ep2;
    int in_body = 0;
    char *ptr;
    static const char *prev_patt0 = ".html\">[ Previous ]</a>";

    if (!hashnumlookup(new_reply_to, &ep2))
	return;
    snprintf(tmpfilename, sizeof(tmpfilename), "%s/aaaa.tmp", set_dir);	/* AUDIT biege: poss. BOF. */
    if ((fp1 = fopen(filename, "r")) == NULL) {
        snprintf(errmsg, sizeof(errmsg), "Couldn't read \"%s\".", filename);
	progerr(errmsg);
    }
    if ((fp2 = fopen(tmpfilename, "w")) == NULL) {
        snprintf(errmsg, sizeof(errmsg), "Couldn't write \"%s\".", tmpfilename);
	progerr(errmsg);
    }
    while (fgets(buffer, sizeof(buffer), fp1)) {
	if (strstr(buffer, "<!-- body=\"end\" -->\n"))
	    in_body = 0;
	if (!strcmp(buffer, "<!-- body=\"start\" -->\n"))
	    in_body = 1;
	if (!in_body) {
	  if (!strcmp(buffer, "<!-- unextthread=\"start\" -->\n")) {
		char *tmpptr = convchars(ep2->subject, ep2->charset);
		if (tmpptr) {
		    char *path = get_path(ep, ep2);
                    fprintf(fp2,"[ <a href=\"%s%.4d.%s\" title=\"%s: &quot;%s&quot;\">%s</a> ]\n", 
			    path, new_reply_to, set_htmlsuffix, lang[MSG_LTITLE_IN_REPLY_TO], 
			    ep2->name, tmpptr ? tmpptr : "");
		    free(tmpptr);
		}
	  }
	  else
	  if (!strcmp(buffer, "<!-- lnextthread=\"start\" -->\n")) {
	        char *tmpptr = convchars(ep2->subject, ep2->charset);
		if (tmpptr) {
		    char *path = get_path(ep, ep2);
                    fprintf(fp2, "<li><dfn>%s</dfn> " 
			    "<a href=\"%s%.4d.%s\" title=\"%s\">%s: \"%s\"</a></li>\n", 
			    lang[MSG_IN_REPLY_TO], path, 
			    new_reply_to, set_htmlsuffix, lang[MSG_LTITLE_IN_REPLY_TO], 
			    ep2->name, tmpptr ? tmpptr : "");
		    free(tmpptr);
		}
	  }
	  else
	  if (!strcmp(buffer, "<!-- nextthread=\"start\" -->\n")) {
		char *tmpptr = convchars(ep2->subject, ep2->charset);
		if (tmpptr) {
		    char *path = get_path(ep, ep2);
                    fprintf(fp2, "<li> <strong>%s:</strong> " "<a href=\"%s%.4d.%s\">%s: \"%s\"</a>\n", lang[MSG_IN_REPLY_TO], path, new_reply_to, set_htmlsuffix, ep2->name, tmpptr ? tmpptr : "");
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
		    "<li><dfn>%s</dfn>: <a href=\"",
		    "<li><dfn>%s</dfn>: <a href=\"",
		    "<li><dfn>%s</dfn>: <a href=\"",
		    NULL
		};
		static const int indices[] = { MSG_MAYBE_IN_REPLY_TO,
		    MSG_MAYBE_IN_REPLY_TO,
		    MSG_IN_REPLY_TO,
		    MSG_IN_REPLY_TO,
		    MSG_PREVIOUS_MESSAGE,
		    MSG_PREVIOUS_MESSAGE,
		    MSG_MAYBE_IN_REPLY_TO,
		    MSG_IN_REPLY_TO,
		    MSG_PREVIOUS_MESSAGE
		};
		int i;
		int suppress = 0;
		for (i = 0; patts[i]; ++i) {
		    char temp[256];
		    snprintf(temp,sizeof(temp), patts[i], lang[indices[i]]);
		    if ((ptr = strcasestr(buffer, temp))
			&& (i < 4 || new_reply_to == atoi(ptr + strlen(temp)))) {
			suppress = 1;
			break;
		    }
		}
		if (suppress)
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
	snprintf(errmsg, sizeof(errmsg), "Couldn't rename \"%s\" to %s.", tmpfilename, filename);
	progerr(errmsg);
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
