/*
** print.c functions
*/

#ifdef GDBM
int togdbm(void *gp, int num, char *name, char *email,
	   char *date, char *msgid, char *subject, char *inreply,
	   char *fromdate, char *charset, char *isodate,
	   char *isofromdate);
#endif

void printcomment(FILE *, char *, char *);
void printbody(FILE *, struct body *, char *, char *, int, char *, char *, int);
char *print_leading_whitespace(FILE *, char *);

void writearticles(int, int);
void writedates(int);
void writesubjects(int);
void writethreads(int);
void writeauthors(int);
void writeattachments(int);

void printdates(FILE *, struct header *, int, int);
void printthreads(FILE *, struct header *, int, int, int);
void printsubjects(FILE *, struct header *, char **, int, int);
void printauthors(FILE *, struct header *, char **, int, int);
void printattachments(FILE *, struct header *);

int checkreplies(FILE *, int, int level, int thread_file_depth);
void fprint_menu(FILE *, mindex_t, char *, char *, char *, int);
void fprint_summary(FILE *, int, long, long, int);
void print_index_header_links(FILE *, mindex_t, long, long, int);
void print_index_footer_links(FILE *, mindex_t, long, int);
void printhtml(FILE *, char *);
int showheader(char *);
void ConvURLs(FILE *, char *, char *, char *);
char *ConvURLsString(char *, char *, char *);
void write_summary_indices(int amount_new);
struct emailinfo *nextinthread(int);
