#ifndef SEARCH_H_INCLUDED
#define SEARCH_H_INCLUDED

typedef struct {
    int msgnum;
    int match_len_tokens;
    int match_len_bytes;
    char *last_matched_string;	/* malloc'd by search, free'd by caller if msgnum >= 0 */
    const char *start_match;
    const char *stop_match;
} String_Match;

int search_for_quote(char *, char *, int, String_Match *);
struct body *tokenize_body(struct body *, char *, char **, int *, int);
void analyze_headers(int);
void set_alt_replylist(struct reply *);

#endif				/* SEARCH_H_INCLUDED */
