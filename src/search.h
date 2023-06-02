#ifndef _HYPERMAIL_SEARCH_H
#define _HYPERMAIL_SEARCH_H
/*
** Copyright (C) 1997, Peter McCluskey (pcm@rahul.net)
** 
** This program and library is free software; you can redistribute it and/or 
** modify it under the terms of the GNU (Library) General Public License 
** as published by the Free Software Foundation; either version 3
** of the License, or any later version. 
** 
** This program is distributed in the hope that it will be useful, 
** but WITHOUT ANY WARRANTY; without even the implied warranty of 
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the 
** GNU (Library) General Public License for more details. 
** 
** You should have received a copy of the GNU (Library) General Public License
** along with this program; if not, write to the Free Software 
** Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA 
*/

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

#endif /* _HYPERMAIL_SEARCH_H */
