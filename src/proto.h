#ifndef _HYPERMAIL_PROTO_H_
#define _HYPERMAIL_PROTO_H_
/*
** Copyright (C) 1997-2023 Hypermail Project
** 
** This program and library is free software; you can redistribute it and/or 
** modify it under the terms of the GNU (Library) General Public License 
** as published by the Free Software Foundation; either version 3
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

/*
** Date functions - date.c
*/

time_t convtoyearsecs(char *);
char *getlocaltime(void);
void gettimezone(void);
void getthisyear(void);
char *getdatestr(time_t);
char *getindexdatestr(time_t);
char *getdateindexdatestr(time_t);
char *secs_to_iso(time_t);
char *secs_to_iso_meta(time_t);
time_t iso_to_secs(char *);
int year_of_datenum(time_t);
int month_of_datenum(time_t);

/*
** domains.c
*/
int valid_root_domain(char *);


/*
** file.c functions
*/

int isdir(char *);
int isfile(char *);
void check1dir(char *);
void checkdir(char *);
char *getfilecontents(char *);
char *add_char(char *, char);
char *add_string(char *, char *);
char *dirpath(char *);

void readconfigs(char *, int);

int find_max_msgnum(void);
int is_empty_archive(void);
void symlink_latest(void);
struct emailsubdir *msg_subdir(int, time_t);
char *msg_href(struct emailinfo *, struct emailinfo *, int);
char *msg_relpath(struct emailinfo *, struct emailinfo *);
char *articlehtmlfilename(struct emailinfo *);
char *htmlfilename(const char *, struct emailinfo *, const char *);
char *haofname(struct emailinfo *);
int matches_existing(int);

char *messageindex_name(void);
int find_max_msgnum_id(void);
char **read_msgnum_id_table(int);
void free_msgnum_id_table(char **, int);
char *message_name(struct emailinfo *);

/*
** hypermail.c functions
*/
void version(void);
void progerr(char *);
void cmderr(char *);
void usage(void);
char *setindex(char *dfltindex, char *indextype, char *suffix);

/*
** lang.c function
*/
char **valid_language(char *, char **);

/*
** lock.c functions
*/
void lock_archive(char *);
void unlock_archive(void);

/*
** mem.c function
*/
void *emalloc(int);

/*
** setup.c functions
*/
#ifdef DEBUG
void dump_config(void);
#endif

/*
** printcss.c functions
*/
void print_default_css_file(char *);

/*
** string.c functions
*/
char *obfuscate_email_address (char *);
char *unobfuscate_email_address (char *);

char *i18n_convstring(char *, char *, char *, size_t *);
char *i18n_utf2numref(char *, int);
unsigned char *i18n_numref2utf(char *);
int i18n_is_valid_us_ascii(const char *);
int i18n_truncate_inalid_us_ascii(char *);
int i18n_replace_non_ascii_chars(char *);
int i18n_replace_control_chars(char *);
int i18n_replace_unicode_spaces(char *, size_t);
int i18n_is_valid_utf8(const char *);
char *i18n_charset_detect(const char *);

char *PushByte(struct Push *, char);
char *PushString(struct Push *, const char *);
char *PushNString(struct Push *, const char *, int);

/* define isblank() if there's no prototype _and_ it's not defined
   as a macro */
#if ((HAVE_DECL_ISBLANK==0) || !defined(HAVE_ISBLANK)) && !defined(isblank)
int isblank(int);
#endif

/* work around for an autoconf limitation. It detects
   strcasecmp declared even if it's not available
   in <string.h> and that we're not using 
   _GNU_SOURCE */
#if HAVE_DECL_STRCASECMP==0 || !defined(HAVE_STRINGS_H)
int strcasecmp(const char *, const char *);
int strncasecmp(const char *, const char *, size_t);
#endif

#if HAVE_DECL_STRCASESTR==0
char *strcasestr(const char *, const char *);
#endif

char *strsav(const char *);
char *strreplace(char *, char *);
void strcpymax(char *, const char *, int);
void strtolower(char *);
int strisspace(char *);

int is_start_boundary(const char *, const char *);
int is_end_boundary(const char *, const char *);
char *stripzone(char *);
char *strip_boundary_id(const char *, int);

char *strchomp(char *);
char *strlftonl(char *s);

int numstrchr(char *, char);
char *getvalue(char *);
char *getconfvalue(char *, char *, char *);
char *unre(char *);
char *oneunre(char *);
int  filter_content_type_values(char *);
void rfc3676_trim_softlb(char *);
char *rfc3676_delsp_quotes(char *);
int rfc3676_ishardlb(const char *);
int isquote(const char *);
char *replace(char *, char *, char *);
char *replacechar(char *, char, char *);
char *convdash(char *);
char *convchars(char *, char *);
char *convcharsnospamprotect(char *, char *);
char *unconvchars(char *);
char *makemailcommand(char *, char *, char *, char *);
char *makeinreplytocommand(char *, char *, char *);
char *spamify(char *input);
char *spamify_small(char *input);
char *spamify_replacedomain(char *input, char *antispamdomain);
char *unspamify(char *);
char *parseemail(char *, char *, char *, parseemail_conversion_t);
char *parseurl(char *, char *);

char *hm_strchr(const char *, int);
void iso2022_state(const char *str, int *state, int *esc);

/*
** quotes.c
*/
const char *get_quote_prefix(void);
const char *find_quote_prefix(struct body *bp, int is_reply);
char *unquote(char *line);
char *remove_hypermail_tags(char *line);
int compute_quoted_percent(struct body *bp);
int is_sig_start(const char *line);
int find_quote_depth(char *);
char *find_quote_class(char *);

#ifdef NOTDEF
#ifdef lint
int isspace(int);
int isalpha(int);
int isalnum(int);
int isxdigit(int);
#endif
#endif

#endif /* _HYPERMAIL_PROTO_H_ */
