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

int search_for_quote(char *search_line, char *exact_line, int max_msgnum,
		     String_Match * match_info);
struct body *tokenize_body(struct body *bp, char *token, char **ptr,
			   int *bigram_index, int ignore);
void analyze_headers(int amount_new);
void set_alt_replylist(struct reply *r);

#endif				/* SEARCH_H_INCLUDED */
