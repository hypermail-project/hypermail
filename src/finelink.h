#ifndef __FINELINK_H
#define __FINELINK_H

#include "hypermail.h"
#include "setup.h"		/* for bool */

int
handle_quoted_text(FILE *fp, struct emailinfo *, const struct body *,
		   char *line, int inquote, int quote_num,
		   bool replace_quoted, int maybe_reply);
void replace_maybe_replies(const char *, struct emailinfo *, int);
void set_new_reply_to(int msgnum, int match_len);
int get_new_reply_to(void);

#endif
