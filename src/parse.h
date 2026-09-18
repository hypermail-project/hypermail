#ifndef _HYPERMAIL_PARSE_H
#define _HYPERMAIL_PARSE_H
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
** parse.c Functions
*/

int ignorecontent(char *);
int inlinecontent(char *);
int preferedcontent(int *, char *, int);
char *safe_filename(char *);
char *getmaildate(char *);
char *getfromdate(char *);
char *getid(char *);
char *getsubject(char *);
char *getreply(char *);
void print_progress(int, char *, char *);
int parsemail(char *, int, int, int, char *, int, int);
int parse_old_html(int, struct emailinfo *, int, int, struct reply **, int);
int loadoldheaders(char *);
int loadoldheadersfromGDBMindex(char *, int);
void crossindex(void);
void crossindexthread1(struct header *);
void crossindexthread2(int);
void fixnextheader(char *, int, int);
void fixreplyheader(char *, int, int, int);
void fixthreadheader(char *, int, int);
int isre(char *, char **);
char *findre(char *, char **);
int textcontent(char *);
char *createlink(char *, char *, char *, int, char *);
void emptydir(char *);
int count_deleted(int);

/*
 * Suffix to prepend to all saved attachments' filenames when the
 * headers don't propose a filename
 */
#define FILE_SUFFIXER "part"

/*
 * Prefix to prepend to all saved attachments' directory names (before
 * the article number)
 */
#define DIR_PREFIXER "att-"

/* 
 * Used to replace invalid characters in supplied attachment filenames
 */
#define REPLACEMENT_CHAR '_'

/*
 * Directory where meta information will be stored
 */
#define META_DIR ".meta"

/*
 * Extension to add to meta files
 */
#define META_EXTENSION ".meta"

#endif /* _HYPERMAIL_PARSE_H */
