/*
** struct.c functions
*/

unsigned hash(char *);
struct emailinfo *addhash(int, char *, char *, char *, char *, char *, char *,
			  char *, char *, char *, char *, struct body *);

struct emailinfo *hashreplylookup(int, char *, int *);

struct emailinfo *hashmsgidlookup(char *, int *);
int hashreplynumlookup(int, char *, char *, int *);

struct body *hashnumlookup(int, struct emailinfo **);

struct body *addbody(struct body *, struct body **, char *, int);

struct reply *addreply(struct reply *, int, struct emailinfo *, int,
		       struct reply **);
struct reply *addreply2(struct reply *, struct emailinfo *, struct emailinfo *,
			int, struct reply **);
int rmlastlines(struct body *);

struct printed *markasprinted(struct printed *, int);
int wasprinted(struct printed *, int);

struct header *addheader(struct header *, struct emailinfo *, int, int);
struct boundary *bound(struct boundary *, char *);

void print_list(char *, struct hmlist *);
int inlist(struct hmlist *, char *);
int inlist_pos(struct hmlist *, char *);
struct hmlist *add_2_list(struct hmlist *, char *);
struct hmlist *add_list(struct hmlist *, char *);
