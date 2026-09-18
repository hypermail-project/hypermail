#ifndef _HYPERMAIL_FINELINK_H
#define _HYPERMAIL_FINELINK_H
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

#include "hypermail.h"
#include "setup.h"		/* for bool */

int
handle_quoted_text(FILE *fp, struct emailinfo *, const struct body *,
		   char *line, int inquote, int quote_num,
		   bool replace_quoted, int maybe_reply);
void replace_maybe_replies(const char *, struct emailinfo *, int);
void set_new_reply_to(int msgnum, int match_len);
int get_new_reply_to(void);

#endif /* _HYPERMAIL_FINELINK_H */
