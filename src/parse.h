/*
** parse.c Functions
*/

int ignorecontent(char *);
int inlinecontent(char *);
int preferedcontent(int *, char *);
char *tmpname(char *, char *);
char *safe_filename(char *);
char *getmaildate(char *);
char *getfromdate(char *);
char *getid(char *);
char *getsubject(char *);
char *getreply(char *);
void print_progress(int, char *, char *);
int parsemail(char *, int, int, int, char *, int, int);
int loadoldheaders(char *);
void crossindex(void);
void crossindexthread1(struct header *);
void crossindexthread2(int);
void fixnextheader(char *, int);
void fixreplyheader(char *, int, int);
void fixthreadheader(char *, int);
int isre(char *, char **);
char *findre(char *, char **);
int textcontent(char *);
char *createlink(char *, char *, char *, int, char *);
void emptydir(char *);

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
 * Path separator for attachment file path generation
 */
#define PATH_SEPARATOR '/'

/*
 * Directory where meta information will be stored
 */
#define META_DIR ".meta"

/*
 * Extension to add to meta files
 */
#define META_EXTENSION ".meta"
