/*
** printfile.c functions
*/

int printfile(FILE *, char *, char *, char *, char *, char *, char *, 
              char *, char *, char *, char *);

void print_main_header(FILE *, bool, char *, char *, char *, char *, char *,
		       char *, char *);

void print_msg_header(FILE *, char *, char *, char *, char *, char *, 
                      char *, char *, time_t, char *);

void print_index_header(FILE *, char *, char *, char *, char *);

void printfooter(FILE *, char *, char *, char *, char *, char *, bool);

void printlaststats(FILE *, long);
