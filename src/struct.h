/*
** struct.c functions
*/

unsigned hash(char *);
void reinit_structs(void);
void fill_email_dates(struct emailinfo *, char *, char *, char *, char *);

struct emailinfo *addhash(int, char *, char *, char *, char *, char *, char *,
			  char *, char *, char *, char *, struct body *);

int insert_in_lists(struct emailinfo *, const bool *, int);

struct emailinfo *hashreplylookup(int, char *, char *, int *);

struct emailinfo *hashmsgidlookup(char *, int *);
int hashreplynumlookup(int, char *, char *, int *);
struct emailinfo *hashlookupbymsgid(char *);
int insert_older_msgs(int);

struct body *hashnumlookup(int, struct emailinfo **);
struct emailinfo *neighborlookup(int, int);

struct body *addbody(struct body *, struct body **, char *, int);
struct body *append_body(struct body *, struct body **, struct body *);
void free_body(struct body *);

struct reply *addreply(struct reply *, int, struct emailinfo *, int,
		       struct reply **);
struct reply *addreply2(struct reply *, struct emailinfo *, struct emailinfo *,
			int, struct reply **);
int rmlastlines(struct body *);

struct emailsubdir *new_subdir(char *, struct emailsubdir *, char *, time_t);

struct printed *markasprinted(struct printed *, int);
int wasprinted(struct printed *, int);

struct header *addheader(struct header *, struct emailinfo *, int, int);
struct boundary *bound(struct boundary *, char *);

void print_list(char *, struct hmlist *);
int inlist(struct hmlist *, char *);
int inlist_pos(struct hmlist *, char *);
int inlist_regex_pos(struct hmlist *, char *);
struct hmlist *add_2_list(struct hmlist *, char *);
struct hmlist *add_list(struct hmlist *, char *);
