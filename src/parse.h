/*
** parse.c Functions
*/

int ignorecontent(char *);
int inlinecontent(char *);
int preferedcontent(int *, char *, int);
char *safe_filename(char *);
char *getmaildate(char *);
char *getfromdate(char *);
char *getid(char *);
char *getsubject(char *);
char *getreply(char *);
void print_progress(int, char *, char *);
int parsemail(char *, int, int, int, char *, int, int);
int parse_old_html(int, struct emailinfo *, int, int, struct reply **, int);
int loadoldheaders(char *);
int loadoldheadersfromGDBMindex(char *, int);
void crossindex(void);
void crossindexthread1(struct header *);
void crossindexthread2(int);
void fixnextheader(char *, int, int);
void fixreplyheader(char *, int, int, int);
void fixthreadheader(char *, int, int);
int isre(char *, char **);
char *findre(char *, char **);
int textcontent(char *);
char *createlink(char *, char *, char *, int, char *);
void emptydir(char *);
int count_deleted(int);

/*
 * Suffix to prepend to all saved attachments' filenames when the
 * headers don't propose a filename
 */
#define FILE_SUFFIXER "part"

/*
 * Prefix to prepend to all saved attachments' directory names (before
 * the article number)
 */
#define DIR_PREFIXER "att-"

/* 
 * Used to replace invalid characters in supplied attachment filenames
 */
#define REPLACEMENT_CHAR '_'

/*
 * Directory where meta information will be stored
 */
#define META_DIR ".meta"

/*
 * Extension to add to meta files
 */
#define META_EXTENSION ".meta"
