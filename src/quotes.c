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
#include "setup.h"
#include "string.h"
#include <ctype.h>
#if ! HAVE_MEMMOVE
#include "pcre/internal.h"
#endif

#define MAX_QPREFIX_GUESSES 8
static char quote_prefix[80];

const char *get_quote_prefix()
{
    return quote_prefix;
}

static int msg_linkp(char *p3, char *p)
{		/* has form <a href="{subdir/}nnnn.html"> */
    char *ptmp;
    if(!(isdigit(p3[-4]) && isdigit(p3[-3])
	 && isdigit(p3[-2]) && isdigit(p3[-1])))
	return 0;
    if (!set_msgsperfolder && !set_folder_by_date)
	return p3[-5] == '"';
    ptmp = strstr(p, "://");
    if (ptmp && ptmp < p3)
	return 0;
    return 1;	/* ought to be more checks here, but they are not easy */
}

/*
** Undo the html additions that prior hypermail run did to body
** Returns NULL if line probably didn't exist in original ascii
*/

char *remove_hypermail_tags(char *line)
{
    char *p, *p1;
    const char *cp;
    static int expect_end_anchor = 0;
    char *buffer = NULL;

    if (!strcasecmp(line, "<p>\n"))
	return strsav("\n");
    else if (!strncasecmp(line, "<i>", 3)) {
	buffer = strsav(line);
	memmove(buffer, buffer + 3, strlen(buffer + 3) + 1);
	if ((p = strcasestr(buffer, "</i><br />\n")) != NULL) {
	    strcpy(p, "\n");
	}
    }
    else if (!strcasecmp(line, "<br />\n"))
	return NULL;
    else if ((cp = strcasestr(line, "<br />\n")) != NULL) {
	buffer = strsav(line);
	strcpy(strcasestr(buffer, "<br />\n"), "\n");
    }
    else if (!strcasecmp(line, "<hr />\n"))
	return
	    strsav("------------------------------------------------------------------------");
    else if (!strcasecmp(line, "<pre>\n"))
	return NULL;
    else if (!strcasecmp(line, "</pre>\n"))
	return NULL;
    else if (!strcasecmp(line, "<ol>\n"))
	return NULL;
    else if (!strcasecmp(line, "<ul>\n"))
	return NULL;
    else if (!strcasecmp(line, "</ol>\n"))
	return NULL;
    else if (!strcasecmp(line, "</ul>\n"))
	return NULL;
    if (!buffer)
	buffer = strsav(line);
    if ((p = strcasestr(buffer, "<li>")) != NULL) {
	const char *p2 = p;
	while (p2 > buffer && isspace(*--p2));
	if (p2 == buffer)
	    memmove(buffer, p + 4, strlen(p + 4) + 1);
    }
#if 0
    "^caps_tag/caps_tag\n"
#endif
	if ((p = strcasestr(buffer, "<a href=\"")) != NULL) {
	char *p2 = strchr(p, '>');
	char *p3;
	char *p4 = strstr(p, "\">");
	char suf[80];
	sprintf(suf, ".%s", set_htmlsuffix);
	p3 = strstr(p, suf);
	if (p2 && p3 && p4 && p2 == p4 + 1 && msg_linkp(p3, p)) {
	    memmove(p, p2 + 1, strlen(p2 + 1) + 1);
	    if ((p = strcasestr(p2, "</a>")) != NULL)
		memmove(p, p + 4, strlen(p + 4) + 1);
	}
	else if (set_mailcommand) {
	    const char *p2 = set_mailcommand;
	    int len = strlen(p2);
	    if (strchr(p2, '$'))
		len = strchr(p2, '$') - p2;
	    if (!strncmp(p + 9, p2, len)) {
		char *p3 = strstr(p + 9 + len, "\">");
		if (p3) {
		    memmove(p, p3 + 2, strlen(p3 + 2) + 1);
		    p3 = strcasestr(p, "</a>");
		    if (p3)
			memmove(p3, p3 + 4, strlen(p3 + 4) + 1);
		}
	    }
	}
    }
    while ((p = strcasestr(buffer, "<a name=\"")) && isdigit(p[9])
	   && isdigit(p[10]) && isdigit(p[11]) && isdigit(p[12])
	   && (p1 = strstr(p, "qlink"))) {
	const char *p2 = strstr(p1, "\">");
	if (p2 && p < p2) {
	    memmove(p, p2 + 2, strlen(p2 + 2) + 1);
	    ++expect_end_anchor;
	}
	else
	    break;
    }
    if (expect_end_anchor && !strcasecmp(buffer, "</a>\n")) {
	--expect_end_anchor;
	free(buffer);
	return NULL;
    }
    if (expect_end_anchor && (p = strcasestr(buffer, "</a>"))) {
	--expect_end_anchor;
	memmove(p, p + 4, strlen(p + 4) + 1);
    }
    p = buffer;
    while ((p = strstr(p, "&nbsp;")) != NULL) {
	memmove(p + 1, p + 6, strlen(p + 6) + 1);
	*p = ' ';		/* replace &nbsp; with space */
    }
    return buffer;
}

int is_sig_start(const char *line)
{
    return (!strcmp(line, "--\n") ||
	    !strcmp(line, "-- \n") ||
	    !strcmp(line, "----\n") || !strcmp(line, "---\n"));
}

/*
** Look for something other than text at the start of a line that might be quote
*/

static const char *guess_quote(const char *line)
{
    static char buf[80];
    int i = 0;
    int found_printable = 0;
    while (!isalnum(line[i]) && !(iscntrl(line[i]) && line[i] != '\t')
	   && i < sizeof(buf) - 1) {
	if (!isspace(line[i]))
	    found_printable = 1;
	buf[i] = line[i];
	++i;
    }
    if (!found_printable)
	i = 0;
    buf[i] = 0;
    if (i > 0 && buf[i - 1] == '<') {
	if (!strncasecmp(line + i - 1, "<a href", 7))
	    buf[i - 1] = 0;
    }
    return buf;
}

/*
* Search through body for non-alphanumeric prefix most frequently used
*/

const char *find_quote_prefix(struct body *bp,
			      int is_reply
			      /* have we already identified a message it's replying to? */
			      )
{
    char guess[MAX_QPREFIX_GUESSES][80];
    int guess_times_found[MAX_QPREFIX_GUESSES];
    int guess_partial_matches[MAX_QPREFIX_GUESSES];
    int i;
    int num_guesses = 0;
    int best_index = -1;
    int best_count = 0;
    while (bp != NULL) {
	const char *q_guess = guess_quote(bp->line);
	if (*q_guess) {
	    int match_index = -1;
	    for (i = 0; i < num_guesses; ++i) {
		if (!strcmp(guess[i], q_guess)) {
		    ++guess_times_found[i];
		    match_index = i;
		}
		else if (!strncmp(guess[i], q_guess, strlen(guess[i])))
		    ++guess_partial_matches[i];
	    }
	    if (match_index == -1 && num_guesses < MAX_QPREFIX_GUESSES) {
		strncpy(guess[num_guesses], q_guess, sizeof(guess[i]));
		guess_times_found[num_guesses] = 1;
		if (strchr(q_guess, '>'))	/* bias it to std */
		    guess_times_found[num_guesses] = 5;
		guess_partial_matches[num_guesses] = 0;
		for (i = 0; i < num_guesses; ++i) {
		    if (!strncmp
			(guess[i], guess[num_guesses],
			 strlen(guess[num_guesses])))
			    guess_partial_matches[num_guesses] += guess_times_found[i];
		}
		++num_guesses;
	    }
	}
	bp = bp->next;
    }
    for (i = 0; i < num_guesses; ++i) {
	if (guess_times_found[i] > best_count
	    || (guess_times_found[i] == best_count
		&& strlen(guess[i]) < strlen(guess[best_index]))) {
	    best_count = guess_times_found[i];
	    best_index = i;
	}
    }
    for (i = 0; i < num_guesses; ++i) {
	int pcount = guess_times_found[i] + guess_partial_matches[i];
	if (pcount > best_count + 1
	    && !strncmp(guess[best_index], guess[i], strlen(guess[i]))) {
	    best_count = pcount;
	    best_index = i;
	}
    }
    if (best_count == 1 && (!is_reply || !strchr(guess[best_index], '>'))) {
	for (i = 0; i < num_guesses; ++i) {
	    if (i != best_index && guess_times_found[i] == best_count
		&& !strncmp(guess[i], guess[best_index],
			    strlen(guess[best_index]))) break;	/* found guess pair like {">","> "} */
	}
	if (i >= num_guesses)
	    best_index = -1;	/* solitary example, give up */
    }
    if (best_index == -1)
	strcpy(quote_prefix, "");
    else
	strncpy(quote_prefix, guess[best_index], sizeof(quote_prefix) - 1);
    if (0)
	printf("best_count %d has_replies %d quote_prefix=%s.\n",
	       best_count, is_reply, quote_prefix);
    return quote_prefix;
}

/*
** remove the prefix that was added by the quoting mechanism
*/

char *unquote(char *line)
{
    if (*quote_prefix) {
	int len = strlen(quote_prefix);
	if (!strncmp(quote_prefix, line, len))
	    line += len;
	return line;
    }
    else
	return line;
}

/*
 * The find_quote_depth function assumes that the line is already known
 * to be quoted text. It returns the number of times the line has been
 * quoted if it decides that is easy to determine, otherwise 1.
 */

int find_quote_depth(char *line)
{
    int cnt = 0;
    if (*quote_prefix && !strchr(quote_prefix, '>'))
	return 1;		/* too hard to deal with nonstandard quotes */
    for ( ; *line; ++line) {
	if (isspace(*line))
	    continue;
	if (*line == '>')
	    ++cnt;
	else break;
    }
    return cnt;
}

char *find_quote_class(char *line)
{
    int quote_depth = find_quote_depth(line);
    if (quote_depth > 4) quote_depth = ((quote_depth - 1) % 4) + 1;
    if (quote_depth >= 4) return "quotelev4";
    if (quote_depth >= 3) return "quotelev3";
    if (quote_depth >= 2) return "quotelev2";
    if (quote_depth >= 1) return "quotelev1";
    return "";
}

/*
** Find out what percent of the body consists of lines appearing
** to be quotes of other messages.
*/

int compute_quoted_percent(struct body *bp)
{
    int inheader = 1;
    int insig = 0;
    int count_quoted = 0;
    int count_lines = 0;
    while (bp != NULL) {
	if ((bp->line)[0] == '\n')
	    inheader = 0;
	else if (inheader) {
	    bp = bp->next;
	    continue;
	}
	if (is_sig_start(bp->line))
	    insig = 1;

	if (isquote(bp->line))
	    ++count_quoted;
	++count_lines;
	bp = bp->next;
    }
    if (!count_lines)
	return 0;
    return (int)(100 * count_quoted / (float)count_lines);
}
