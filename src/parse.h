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
void fixreplyheader(char *, int);
void fixthreadheader(char *, int);
int isre(char *, char **);
char *findre(char *, char **);
int textcontent(char *);
char *createlink(char *, char *, char *, int, char *);
void emptydir(char *);
