#ifndef __FINELINK_H
#define __FINELINK_H

#include "hypermail.h"
#include "setup.h"		/* for bool */

int
handle_quoted_text(FILE *fp, int quoting_msgnum, const struct body *bp,
		   char *line, int inquote, int quote_num, char *inreply,
		   const char *dir, bool replace_quoted, int maybe_reply,
		   char *msgid, char *subject);
void replace_maybe_replies(const char *filename, const char *dir,
			   int new_reply_to);
void set_new_reply_to(int msgnum, int match_len);
int get_new_reply_to(void);

#endif
