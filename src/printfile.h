#ifndef _HYPERMAIL_PRINTFILE_H
#define _HYPERMAIL_PRINTFILE_H
/*
** Copyright (C) 1997-2023 Hypermail Project
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

/*
** printfile.c functions
*/

int printfile(FILE *, char *, char *, char *, char *, char *, char *, 
              char *, char *, char *, char *);

void print_main_header(FILE *, bool, char *, char *, char *, char *, char *,
		       char *, char *, char *, int, int);

void print_msg_header(FILE *, char *, char *, char *, char *,
                      struct emailinfo *, char *, int);

void print_index_header(FILE *, char *, char *, char *, char *, struct emailinfo *);

void printfooter(FILE *, char *, char *, char *, char *, char *, bool);

void printlaststats(FILE *, long);

#endif /* _HYPERMAIL_PRINTFILE_H */
