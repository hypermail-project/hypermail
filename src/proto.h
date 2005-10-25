#ifndef __PROTO_H_
#define __PROTO_H_ 1

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
** string.c functions
*/
char *i18n_convstring(char *, char *, char *, int *);
char *i18n_utf2numref(char *, int);
char *i18n_numref2utf(char *);

char *PushByte(struct Push *, char);
char *PushString(struct Push *, const char *);
char *PushNString(struct Push *, const char *, int);

char *strcasestr (char *, const char *);
char *strsav(const char *);
char *strreplace(char *, char *);
void strcpymax(char *, const char *, int);
void strtolower (char *);
char *stripzone(char *);
int numstrchr(char *, char);
char *getvalue(char *);
char *getconfvalue(char *, char *, char *);
char *unre(char *);
char *oneunre(char *);
int isquote(const char *);
char *replace(char *, char *, char *);
char *replacechar(char *, char, char *);
char *convdash(char *);
char *convchars(char *, char *);
char *convcharsnospamprotect(char *, char *);
char *unconvchars(char *);
char *makemailcommand(char *, char *, char *, char *);
char *unspamify(char *);
char *parseemail(char *, char *, char *);
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

#endif
