#ifndef _HYPERMAIL_PRINT_H
#define _HYPERMAIL_PRINT_H
/*
** Copyright (C) 1994, 1995 Enterprise Integration Technologies Corp.
**         VeriFone Inc./Hewlett-Packard. All Rights Reserved.
** Kevin Hughes, kev@kevcom.com 3/11/94
** Kent Landfield, kent@landfield.com 4/6/97
** Hypermail Project 1998-2023
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
** print.c functions
*/

#ifdef GDBM
int togdbm(void *gp, struct emailinfo *);
#endif

void printcomment(FILE *, char *, char *);
void print_headers(FILE *, struct emailinfo *, int);
struct body *print_headers_rfc822_att(FILE *, struct emailinfo *, struct body *);
int print_links(FILE *, struct emailinfo *, int, int);
void printbody(FILE *, struct emailinfo *, int, int);
char *print_leading_whitespace(FILE *, char *);

void update_deletions(int);
void writearticles(int, int);
void writedates(int, struct emailinfo *);
void writesubjects(int, struct emailinfo *);
void writethreads(int, struct emailinfo *);
void writeauthors(int, struct emailinfo *);
void writehaof(int, struct emailinfo *);

void writeattachments(int, struct emailinfo *);

void print_empty_archive_notice(FILE *, int);
void printdates(FILE *, struct header *, int, int, struct emailinfo *, char *);
void printsubjects(FILE *, struct header *, char **, int, int,
		   struct emailinfo *);
void printauthors(FILE *, struct header *, char **, int, int,
		  struct emailinfo *);
int printattachments(FILE *, struct header *, struct emailinfo *, bool *);

int checkreplies(FILE *, int, int level, int thread_file_depth);
void fprint_menu(FILE *, mindex_t, char *, char *, char *, int,
		 struct emailsubdir *);
void fprint_menu0(FILE *fp, struct emailinfo *email, int);
void fprint_summary(FILE *, int, long, long, int);
void print_index_header_links(FILE *, mindex_t, long, long, int,
			      struct emailsubdir *);
void print_index_footer_links(FILE *, mindex_t, long, int,
			      struct emailsubdir *);
void printhtml(FILE *, char *);
int showheader_list(char *);
int showheader_match(char *, char *);
void ConvURLs(FILE *, char *, char *, char *, char *);
char *ConvURLsString(char *, char *, char *, char *);
void write_summary_indices(int);
void write_toplevel_indices(int);
struct emailinfo *nextinthread(int);
struct emailinfo *nextinthread_skip_deleted(int);
void init_index_names(void);

void write_messageindex(int, int);

#endif /* _HYPERMAIL_PRINT_H */
