/*
** Copyright (C) 1997, Peter McCluskey (pcm@rahul.net)
** Based fairly closely on Seth Golub's txt2html.pl, Revision: 1.24
** (http://www.cs.wustl.edu/~seth/txt2html/)
*/

#include "hypermail.h"
#include "proto.h"
#include "setup.h"
#include "print.h"
#include "finelink.h"
#include "txt2html.h"
#include <string.h>
#include <assert.h>
#include <ctype.h>

static int short_line_length = 40;
static int preformat_whitespace_min = 5;
static int preformat_vertical_chars_min = 3;
static int preformat_repeated_chars_min = 14;
static int preformat_repeated_carets_min = 5;
static char preformat_dots[MAXLINE];
static char preformat_carets[MAXLINE];
static int par_indent = 2;
static int preformat_trigger_lines = 2;
static int endpreformat_trigger_lines = 2;
static int hrule_min = 4;
static int min_caps_length = 3;
static char caps_tag[MAXLINE] = "strong";
static int tab_width = 8;
static int indent_width = 2;
static int unhyphenation = 1;

static char *unhyphenated_word;
static int in_pre_block;
static int insig;
static int islist;
static int is_blank_prev;
static int inquote;
static int was_quote_prefix;
static int quote_num = 0;
static int was_break;
static int was_hrule;
static int prior_was_hrule;
static int was_par;
static int was_caps;
static int was_header;
static int prev_line_length;
static int prev_indent;

#define OL 1
#define UL 2

#define MAXLISTNESTING 8
static int listnum;
static int list[MAXLISTNESTING];
static char list_indent[MAXLINE];
static char list_prefix[MAXLISTNESTING][MAXLINE];

static char *chomp(char *line)
{				/* sort of replaces rmcr() */
    int l = strlen(line);
    if (l > 0 && line[l - 1] == '\n')
	line[l - 1] = 0;
    return line;
}

static int convert_to_hrule(const char *line)
{
    int count_hrule_chars = 0;
    while (*line && isspace(*line))
	++line;
	while (*line == '-' || *line == '_' || *line == '~' || *line == '=' || *line == '*') {
	if (!count_hrule_chars || *line == line[-1])
	    ++count_hrule_chars;
	else
	    return 0;
	++line;
    }
    while (isspace(*line))
	++line;
    return (!*line && count_hrule_chars >= hrule_min);
}

/* check for vertically aligned non-alphanumeric chars */

static int find_vertical_repeats(const struct body *bp)
{
    int i;
    if (!bp->next) {
	return 0;
    }
    i = (isquote(bp->line) ? strlen(get_quote_prefix()) : 0);
    if (i > strlen(bp->line))
	return 0;
	for (; i < MAXLINE; ++i) {
	int j;
	const struct body *bp2 = bp->next;
	if (!bp->line[i] || i > strlen(bp2->line) || !bp2->line[i]) {
	    return 0;
	}
	for (j = 1; bp2; ++j) {
	    if (i >= strlen(bp2->line)
		|| bp->line[i] != bp2->line[i] || !ispunct(bp->line[i]))
		break;
	    bp2 = bp2->next;
	}
	if (j >= preformat_vertical_chars_min) {
	    return j;
	}
    }
    return 0;
}

static int find_repetition(const struct body *bp)
{
    const char *line = bp->line;
    int i = 0;
    while (line[i]) {
	int count_repeated_chars = 0;
	while (line[i + 1] == line[i]) {
	    if (++count_repeated_chars > preformat_repeated_chars_min) {
		while (isspace(line[i]))
		    ++i;	/* if trailing whitespace, return 0 */
		if (!line[i])
		    return find_vertical_repeats(bp);
		return 1;
	    }
	    ++i;
	}
	++i;
    }
    return find_vertical_repeats(bp);
}

static int has_many_carets(const char *line)
{
    return strstr(line, preformat_carets) != NULL;
}

/* Returns true if line is considered to be preformatted */

static int is_preformatted(const char *line)
{
    int count_whitespace = 0;
    if (strstr(line, preformat_dots))
	return 1;
    while (*line) {
	if (isspace(*line)) {
	    if (*line == '\t')
				count_whitespace += tab_width - (count_whitespace % tab_width);
	    else
		++count_whitespace;
	    if (count_whitespace >= preformat_whitespace_min) {
		while (isspace(*line)) {
		    ++line;
		    if (!*line)	/* was trailing whitespace */
			return 0;
		}
		return 1;
	    }
	}
	else
	    count_whitespace = 0;
	++line;
    }
    return 0;
}

/*
 * What, if anything, identifies this line as something that should start a new
 * <LI>?
*/

static void listprefix(char *line, struct Push *number, struct Push *prefix, struct Push *rawprefix)
{
    const char *p;
    int is_prefix = 0;

    for (p = line; isspace(*p); ++p);
    if (isalnum(*p)) {
	const char *p2 = p;
	if (isdigit(*p2))
	    while (isdigit(*++p2));
	else
	    ++p2;
	if (*p2 != '.' && *p2 != ')' && *p2 != ']' && *p2 != ':')
	    return;
	if (!isspace(*++p2))
	    return;
	while (isspace(*p2))
	    ++p2;
	if (!isprint(*p2))
	    return;
	is_prefix = 1;
    }
    if (*p == '-' || *p == '=' || *p == '*' || (*p == 'o' && !is_prefix)) {
	const char *p2 = p;
	while (*++p2 == *p);
	if (!isspace(*p2))
	    return;
	while (isspace(*p2))
	    ++p2;
	if (!isprint(*p2))
	    return;
	is_prefix = 1;
    }
    if (!is_prefix)
	return;

    if (isalnum(*p) && !(*p == 'o' && isspace(p[1]))) {
	if (isdigit(*p))
	    do
		PushByte(number, *p++);
	    while (isdigit(*p));
	else
	    PushByte(number, *p++);
	PushByte(prefix, *p++);
	PushNString(rawprefix, line, p - line);
    }
    else {
	if (*p == '-' || *p == 'o' || *p == '=' || *p == '*') {
	    PushNString(rawprefix, line, p - line);
	    PushNString(prefix, line, p - line);
	    while (*p == '-' || *p == 'o' || *p == '=' || *p == '*') {
		PushByte(rawprefix, *p);
		PushByte(prefix, *p++);
	    }
	}
	PushByte(number, 0);	/* make sure it has empty string, not null pointer */
    }
}

static void set_list_indent(int n)
{
    assert(n * indent_width < MAXLINE - 1);	/* shouldn't be a problem unless MAXLISTNESTING ~ MAXLINE */
    memset(list_indent, ' ', n * indent_width);
    list_indent[n * indent_width] = 0;
}

static int startlist(FILE *fp, const char *number, const char *prefix, const char *rawprefix)
{
    strcpy(list_prefix[listnum], prefix);	/* AUDIT biege: does it fit? */
    if (*number) {
	/* It doesn't start with 1,a,A.  Let's not screw with it. */
	if ((*number != '1') && (*number != 'a') && (*number != 'A'))
	    return 0;
	fprintf(fp, "\n%s<ol>\n", list_indent);
	list[listnum] = OL;
    }
    else {
	fprintf(fp, "\n%s<ul>\n", list_indent);
	list[listnum] = UL;
    }

    if (listnum < MAXLISTNESTING)
	list[++listnum] = 0;
    set_list_indent(listnum);
    return 1;
}

static void endlist(FILE *fp, int n)
{				/* End N lists */
    for (; n > 0; n--, listnum--) {
	set_list_indent(listnum - 1);
	if (list[listnum - 1] == UL) {
	    fprintf(fp, "\n%s</ul>\n", list_indent);
	}
	else if (list[listnum - 1] == OL) {
	    fprintf(fp, "\n%s</ol>\n", list_indent);
	}
	else {
            snprintf(errmsg, sizeof(errmsg),
                "Encountered list of unknown type %d\n", list[listnum - 1]);
	    progerr(errmsg);
	}
    }
    islist = (listnum != 0);
}

static char *continuelist(FILE *fp, char *line)
{
    char *p = line;
    if (list[listnum - 1] == UL) {
	while (*p && isspace(*p))
	    ++p;
	if (*p != '-' && *p != '=' && *p != 'o' && *p != '*')
	    return line;
	++p;
		while (*p == p[-1] && (*p == '-' || *p == '=' || *p == 'o' || *p == '*'))
			++p;
	while (*p && isspace(*p))
	    ++p;
	fprintf(fp, "%s<li> ", list_indent);
	return --p;
    }
    if (list[listnum - 1] == OL) {
	while (*p && isspace(*p))
	    ++p;
	if (!isalnum(*p))
	    return line;
	if (isalpha(*p))
	    ++p;
	else
	    while (isdigit(*p))
		++p;
	if (!*p)
	    return line;
	++p;
	while (*p && isspace(*p))
	    ++p;
	fprintf(fp, "%s<li> ", list_indent);
	return --p;
    }
    return line;
}

static char *liststuff(FILE *fp, char *line, int line_indent)
{
    int i, j;
    const char *p;
    int len;
    char *prefix;
    char *rawprefix;
    char *number;
    struct Push prefix_buf;
    struct Push rawprefix_buf;
    struct Push number_buf;
    struct Push prefix_alternate;
    int total_prefix_count = 0;
    INIT_PUSH(prefix_buf);
    INIT_PUSH(rawprefix_buf);
    INIT_PUSH(number_buf);
    INIT_PUSH(prefix_alternate);
    listprefix(line, &number_buf, &prefix_buf, &rawprefix_buf);

    if (!prefix_buf.len) {
	if (!is_blank_prev)
	    return line;	/* inside a list item */
	/* This ain't no list.  We'll want to end all of them. */
	if (listnum)
	    endlist(fp, listnum);
	return line;
    }
    number = PUSH_STRING(number_buf);
    prefix = PUSH_STRING(prefix_buf);
    rawprefix = PUSH_STRING(rawprefix_buf);

    /* If numbers with more than one digit grow to the left instead of */
    /* to the right, the prefix will shrink and we'll fail to match the */
    /* right list.  We need to account for this. */
    if ((len = strlen(number)) > 1) {
	for (i = 0; i < len - 1; ++i)
	    PushByte(&prefix_alternate, ' ');
	PushString(&prefix_alternate, prefix);
    }
    else
	PushByte(&prefix_alternate, 0);

    /* Maybe we're going back up to a previous list */
    for (i = listnum - 1; (i >= 0) && strcmp(prefix, list_prefix[i]); --i) {
		if (len > 1 && !strcmp(PUSH_STRING(prefix_alternate), list_prefix[i]))
		break;
    }
    free(PUSH_STRING(prefix_alternate));

    /* Measure the indent from where the text starts, not where the */
    /* prefix starts.  This won't screw anything up, and if we don't do */
    /* it, the next line might appear to be indented relative to this */
    /* line, and get tagged as a new paragraph. */
    p = line;
    while (isspace(*p)) {
	total_prefix_count++;
	p++;
    }
    j = total_prefix_count;
    while (isalnum(*p) || *p == '-' || *p == '=' || *p == '*') {
	total_prefix_count++;
	p++;
    }
    if (total_prefix_count && total_prefix_count != j) {	/* found a prefix */
	total_prefix_count++;
	p++;
	while (*p && isspace(*p)) {
	    total_prefix_count++;
	    p++;
	}
    }
    /* Of course, we only use it if it really turns out to be a list. */

    ++i;
    if ((i > 0) && (i != listnum)) {
	endlist(fp, listnum - i);
    }
    else if (!listnum || (i != listnum)) {
		if ((line_indent > 0) || is_blank_prev || was_break || was_header || prior_was_hrule)
	    islist |= startlist(fp, number, prefix, rawprefix);
	else {
	    free(prefix);
	    free(rawprefix);
	    free(number);
	    return line;	/* We have something like this: "- foo" which usually turns out not to be a list. */
	}
    }
    else
	line_indent = total_prefix_count;

    if (islist)
	line = continuelist(fp, line);
    if (islist)
	line_indent = total_prefix_count;
    free(prefix);
    free(rawprefix);
    free(number);
    return line;
}

static int iscaps(const char *line)
{
    int count_uppercase = 0;
    int found_enough_uppercase = 0;
    while (*line) {
	if (islower(*line) || *line == '<')
	    return 0;
	if (isupper(*line)) {
	    if (++count_uppercase >= min_caps_length)
		found_enough_uppercase = 1;
	}
	else
	    count_uppercase = 0;
	++line;
    }
    return found_enough_uppercase;
}

static char *unhyphenate1(struct Push *uwbuf, const char *next_line, char *line, int len)
{
    struct Push buffer;
    while (*next_line && isalpha(*next_line))
	PushByte(uwbuf, *next_line++);
    if (*next_line == '-')	/* something like 'one-of-a-kind'? */
	return line;
    while (*next_line && strchr(")}].,:;'\">", *next_line))
	PushByte(uwbuf, *next_line++);	/* include any punct with word */
	if (isupper(PUSH_STRING(*uwbuf)[0]) && islower((PUSH_STRING(*uwbuf))[1]))
		return line;	/* capitalization probably means separate word */
    INIT_PUSH(buffer);
    PushNString(&buffer, line, len - 2);
    PushString(&buffer, PUSH_STRING(*uwbuf));	/* concatenate 2 parts of word */
    PushByte(&buffer, '\n');
    RETURN_PUSH(buffer);
}

static char *unhyphenate(char *line, const char *next_line)
{
    struct Push uwbuf;
    char *buffer;
    int len = strlen(line);
    const char *p = line + strlen(line) - 3;	/* look before the final '-\n' */
    if (strlen(line) < 3 || !isalpha(*p))
	return line;		/* doesn't look like hyphenated word */
    while (p > line && isalpha(*p))
	--p;
    if (p == line || *p == '-')
	return line;		/* too long to be word or something like 'one-of-a-kind' */
    while (*next_line && isspace(*next_line))
	++next_line;
    if (!isalpha(*next_line))
	return line;
    INIT_PUSH(uwbuf);
    buffer = unhyphenate1(&uwbuf, next_line, line, len);
    if (unhyphenated_word)
	free(unhyphenated_word);
    unhyphenated_word = PUSH_STRING(uwbuf);
    return buffer;
}

static char *unhyphenate_2nd_line(char *line)
{
    struct Push buffer;
    char *next_part = unhyphenated_word;
    INIT_PUSH(buffer);
    while (*line && isspace(*line))
	PushByte(&buffer, *line++);
    while (*next_part++)
	++line;			/* skip over part that was moved to prior line */
    PushString(&buffer, line);
    free(unhyphenated_word);
    unhyphenated_word = NULL;
    RETURN_PUSH(buffer);
}

/*
char *id, char *subject,
	 int msgnum, char *inreply, const char *dir, 
*/
void txt2html(FILE *fp, struct emailinfo *email, const struct body *bp, bool replace_quoted, int maybe_reply)
{
    int is_caps_line = 0;
    int is_blank_line = 1;
    int line_indent = 0;
    const char *p;
    const char *next_line = bp->next ? bp->next->line : "";
    char *line = strsav(bp->line);
    if (unhyphenated_word) {
	char *new_line = unhyphenate_2nd_line(line);
	free(line);
	line = new_line;
    }
    for (p = line; *p; ++p) {
	if (!isspace(*p)) {
	    is_blank_line = 0;
	    break;
	}
	if (*p == '\t')
	    line_indent += tab_width - (line_indent % tab_width);
	else
	    ++line_indent;
    }
    if (in_pre_block > 1 && !isquote(line)) {
	--in_pre_block;
    }
	else if (in_pre_block && preformat_trigger_lines != 0 && !is_preformatted(line) && !insig && !is_blank_line && !find_repetition(bp)
	     && !has_many_carets(line) && !has_many_carets(next_line)
		 && (endpreformat_trigger_lines == 1 || !is_preformatted(next_line))) {
	in_pre_block = 0;
	fprintf(fp, "</pre>\n");
    }
    if (!in_pre_block && convert_to_hrule(line)) {
	fprintf(fp, "<hr>\n");
	line[0] = 0;
	was_hrule = 1;
    }

    if (!in_pre_block && !is_blank_line) {
	char *new_line = liststuff(fp, line, line_indent);
	if (new_line != line) {	/* new_line points to middle of line */
	    int i;
	    for (i = 0; new_line[i] && line[i]; ++i)
		line[i] = new_line[i];
	    line[i] = new_line[i];
	}
    }

	if (!in_pre_block && !islist && endpreformat_trigger_lines != 0 && !isquote(line) && !was_hrule && (preformat_trigger_lines == 0 || is_sig_start(line) || find_repetition(bp) || has_many_carets(line) || has_many_carets(next_line) || (is_preformatted(line) && (preformat_trigger_lines == 1 || is_preformatted(next_line))))) {
	fprintf(fp, "<pre>\n");
	in_pre_block = find_repetition(bp);	/* set #lines from find_vertical_repeats */
	if (!in_pre_block)
	    in_pre_block = 1;
	if (is_sig_start(line))
	    insig = 1;
    }
	if (!is_blank_line && !in_pre_block && !was_break && !was_caps && !was_hrule && !was_par && !islist && (is_blank_prev || (line_indent > prev_indent + par_indent))) {
	fprintf(fp, "<p>\n");
	was_par = 1;
    }

	if (!islist && !in_pre_block && !is_blank_line && !inquote && !is_blank_prev && prev_line_length < short_line_length && !was_hrule && !was_par && !islist && !was_break) {
	fprintf(fp, "<br>\n");
	was_break = 1;
    }

	if (unhyphenation && !is_blank_line && !in_pre_block && !inquote && !was_break && strlen(line) >= 2 && line[strlen(line) - 2] == '-') {
	char *new_line = unhyphenate(line, next_line);
	if (new_line != line) {
	    free(line);
	    line = new_line;
	}
    }

    if ((is_caps_line = (!in_pre_block && iscaps(line))) != 0)
	fprintf(fp, "<%s>", caps_tag);

    if (isquote(line)) {
	if (!was_quote_prefix && !was_break && !was_par && !was_hrule) {
	    fprintf(fp, "<br>\n");
	    was_break = 1;
	}
	if (!set_linkquotes) {
	    fprintf(fp, "<i class=\"%s\">", find_quote_class(line));
	    ConvURLs(fp, chomp(line), email->msgid, email->subject, email->charset);
	    fprintf(fp, "</i><br>\n");
	}
		else if (handle_quoted_text(fp, email, bp, line, inquote, quote_num, replace_quoted, maybe_reply)) {
	    ++quote_num;
	    inquote = 1;
	}
	if (!inquote)
	    was_break = 1;
	was_quote_prefix = 1;
    }
    else if (line[0] != '\0') {
	char *sp;
	if (in_pre_block || insig || islist)
	    sp = line;
	else
	    sp = print_leading_whitespace(fp, line);
	ConvURLs(fp, chomp(sp), email->msgid, email->subject, email->charset);
	fprintf(fp, "\n");
    }
    if (is_caps_line)
	fprintf(fp, "</%s>\n", caps_tag);
    is_blank_prev = is_blank_line;
    if (!isquote(line))
	inquote = was_quote_prefix = 0;
    prior_was_hrule = was_hrule;
    if (!is_blank_line) {
	was_hrule = 0;
	if (!was_quote_prefix || !isquote(next_line))
	    was_break = 0;
	was_par = 0;
	was_caps = is_caps_line;
	was_header = 0;
	prev_line_length = strlen(line);
	prev_indent = line_indent;
    }
    free(line);
}

void init_txt2html()
{
    in_pre_block = 0;
    insig = 0;
    islist = 0;
    listnum = 0;
    is_blank_prev = 0;
    inquote = 0;
    was_quote_prefix = 0;
    was_break = 0;
    was_hrule = 0;
    prior_was_hrule = 0;
    was_par = 0;
    was_header = 1;
    memset(preformat_dots, ' ', preformat_whitespace_min);
    preformat_dots[preformat_whitespace_min] = 0;
    memset(preformat_carets, '^', preformat_repeated_carets_min);
    preformat_carets[preformat_repeated_carets_min] = 0;
    prev_line_length = 0;
    prev_indent = 0;
    quote_num = 0;
    unhyphenated_word = NULL;
}

void end_txt2html(FILE *fp)
{
    if (islist)
	endlist(fp, listnum);	/* End all lists */
    if (in_pre_block)
	fprintf(fp, "</pre>\n");
    if (unhyphenated_word) {
	free(unhyphenated_word);
	unhyphenated_word = NULL;
    }
#if DEBUG_HTML
    printcomment(fp, "end_txt2html", "");	
#endif
}
