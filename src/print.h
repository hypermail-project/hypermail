/*
** print.c functions
*/

void printcomment(FILE *, char *, char *);
void printbody(FILE *, struct body *, char *, char *);

void writearticles(int, int);
void writedates(int);
void writesubjects(int);
void writethreads(int);
void writeauthors(int);
#ifdef CHANGE_12DEC2000_BC
void writeattachments(int);
#endif

void printdates(FILE *, struct header *);
void printthreads(FILE *, struct header *);
void printsubjects(FILE *, struct header *, char **);
void printauthors(FILE *, struct header *, char **);
#ifdef CHANGE_12DEC2000_BC
void printattachments(FILE *, struct header *);
#endif

void checkreplies(FILE *, int, int);
void fprint_menu(FILE *, mindex_t, char *, char *, char *, int);
void fprint_summary(FILE *, int, long, long, int);
void print_index_header_links(FILE *, int, int);
void print_index_footer_links(FILE *, int, int);
void printhtml(FILE *, char *);
int showheader(char *);
void ConvURLs(FILE *, char *, char *, char *);
struct emailinfo *nextinthread(int);
