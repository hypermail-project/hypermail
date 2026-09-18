#ifndef _HYPERMAIL_TXT2HTML_H
#define _HYPERMAIL_TXT2HTML_H
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

void
txt2html(FILE *, struct emailinfo *, const struct body *, bool, int);
void init_txt2html(void);
void end_txt2html(FILE *fp);

#endif /* _HYPERMAIL_TXT2HTML_H */
