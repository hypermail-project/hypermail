/* 
** Copyright (C) 1994, 1995 Enterprise Integration Technologies Corp.
**         VeriFone Inc./Hewlett-Packard. All Rights Reserved.
** Kevin Hughes, kev@kevcom.com 3/11/94
** Kent Landfield, kent@landfield.com 4/6/97
** 
** This program and library is free software; you can redistribute it and/or 
** modify it under the terms of the GNU (Library) General Public License 
** as published by the Free Software Foundation; either version 2 
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

#ifndef _HYPERMAIL_HYPERMAIL_H
#define _HYPERMAIL_HYPERMAIL_H

#ifndef MAIN_FILE
#define VAR extern
#else
#define VAR
#endif

#include "../config.h"
#include "../patchlevel.h"

#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif

#ifdef HAVE_STDIO_H
#include <stdio.h>
#endif

#ifdef HAVE_SYS_TYPES_H
#include <sys/types.h>
#endif

#ifdef HAVE_SYS_STAT_H
#include <sys/stat.h>
#endif

#ifdef TM_IN_SYS_TIME
#include <sys/time.h>
#else
#include <time.h>
#endif

#ifdef HAVE_STDLIB_H
#include <stdlib.h>
#endif

#ifdef HAVE_PWD_H
#include <pwd.h>
#endif

#ifdef HAVE_STRING_H
#include <string.h>
#endif

#ifdef HAVE_CTYPE_H
#include <ctype.h>
#endif

#ifdef HAVE_ERRNO_H
#include <errno.h>
#endif

/*
* this redefines the standard *printf() to use ours 
*/
#define _MPRINTF_REPLACE
#include "mprintf.h"

#undef FALSE
#define FALSE 0
#undef TRUE
#define TRUE  1

#define PROGNAME    "hypermail"
#define HMURL       "http://www.hypermail.org/"

#define INDEXNAME   "index"
#define DIRNAME     "archive"
#define NONAME      "(no name)"
#define NODATE      "(no date)"
#define NOEMAIL     "(no email)"
#define NOSUBJECT   "(no subject)"

#define NUMSTRLEN    10
#define MAXLINE	     1024
#define MAXFILELEN   256
#define NAMESTRLEN   80
#define MAILSTRLEN   80
#define DATESTRLEN   80
#define MSGDSTRLEN   256
#define REPYSTRLEN   256
#define SUBJSTRLEN   256
#define URLSTRLEN    256
#define HOSTSTRLEN   256
#define TITLESTRLEN  64
#define HASHSIZE     673

#define SHORTDATELEN   12
#define TIMEZONELEN    10
#define YEARLEN        5
#define BASEYEAR       1970
#define DAYSPERYEAR    365
#define SECSPERMIN     60
#define SECSPERHOUR    3600
#define SECSPERDAY     86400
#define IS_LEAP(y) (y > 1752 && (y % 4 == 0 && (y % 100 != 0 || y % 400 == 0)))

#if 0				/* replaced with mindex_t */
#define FROM_AUTHOR  0
#define FROM_DATE    1
#define FROM_THREAD  2
#define FROM_SUBJECT 3
#endif

#ifdef CHANGE_12DEC2000_BC
#define FROM_ATTACHMENT 4
#endif

#define PAGE_TOP     1
#define PAGE_BOTTOM  2

#ifdef CHANGE_12DEC2000_BC
typedef enum { DATE_INDEX, THREAD_INDEX, SUBJECT_INDEX, AUTHOR_INDEX,
    ATTACHMENT_INDEX, NO_INDEX
} mindex_t;
#else
typedef enum { DATE_INDEX, THREAD_INDEX, SUBJECT_INDEX, AUTHOR_INDEX,
    NO_INDEX
} mindex_t;
#endif

/*
** Use this struct to build expandable buffers. Quick and easy.
** Daniel 1998-11-17.
*/

struct Push {
    char *string;
    size_t len;
    size_t alloc;
};

#define INIT_PUSH(x) memset(&(x), 0, sizeof(struct Push))
#define RETURN_PUSH(x) return (x).string

#define PUSH_STRING(x) (x).string
#define PUSH_STRLEN(x) (x).len

struct reply {
    int frommsgnum;
    int msgnum;
    struct emailinfo *data;
    int maybereply;
    struct reply *next;
};

struct body {
    char *line;
    char html;			/* set to TRUE if already converted to HTML */
    char header;		/* part of header */
    char parsedheader;		/* this header line has been parsed once */
    char attached;		/* part of attachment */
    char demimed;		/* if this is a header, this is set to TRUE if
				   it has passed the decoderfc2047() function */
    int msgnum;
    struct body *next;
};

struct printed {
    int msgnum;
    struct printed *next;
};

struct hmlist {
    char *val;
    struct hmlist *next;
};

struct hashemail {
    struct emailinfo *data;
    struct hashemail *next;
};

struct emailinfo {
    int msgnum;
    char *name;
    char *emailaddr;
    char *fromdatestr;
    time_t fromdate;
    char *datestr;
    time_t date;
    char *msgid;
    char *subject;
    char *unre_subject;
    char *inreplyto;
    char *charset;		/* added in 2b10 */

    long datenum;		/* moved here from 'struct header' */
    long flags;			/* added 1999-03-19 for the new thread output functions */

#define PRINT_THREAD 1		/* set if already used in the thread output */
#define USED_THREAD  2		/* set if already stored in threadlist */

    struct body *bodylist;
};

struct header {
    struct emailinfo *data;
    struct header *left;
    struct header *right;
};

struct attach {
    char *contenttype;
    char *name;
    char *id;
    char *storedas;		/* filename used for storage */
    char *descr;		/* "Content-Description" */
    struct attach *next;
};

/* MIME Support */

#define BODY_NONE     0
#define BODY_CONTINUE (1<<0)	/* this is a continued line */
#define BODY_HTMLIZED (1<<1)	/* this is already htmlified */
#define BODY_HEADER   (1<<2)	/* this is a header line */
#define BODY_ATTACHED (1<<3)	/* this line was attached */

struct boundary {
    struct boundary *next;
    struct boundary *prev;
    char *line;
};

VAR struct header *subjectlist;
VAR struct header *authorlist;
VAR struct header *datelist;
VAR struct reply *replylist;
VAR struct reply *threadlist;
VAR struct printed *printedlist;
VAR struct printed *printedthreadlist;
VAR struct hashemail *etable[HASHSIZE];

VAR struct hmlist *show_headers;

VAR char timezonestr[TIMEZONELEN];
VAR char thisyear[YEARLEN];
VAR char errmsg[MAXLINE];
VAR char lockfile[MAXFILELEN];
VAR char *authname;		/* By author index file             */
VAR char *datename;		/* By date index file               */
VAR char *subjname;		/* By subject index file            */
VAR char *thrdname;		/* By thread index file             */
#ifdef CHANGE_12DEC2000_BC
VAR char *attname;		/* By attachment index file         */
#endif

VAR int use_mailcommand;
VAR int use_mailto;
VAR int use_domainaddr;

VAR char *ihtmlheaderfile;
VAR char *ihtmlfooterfile;
VAR char *mhtmlheaderfile;
VAR char *mhtmlfooterfile;

VAR long firstdatenum;
VAR long lastdatenum;

#ifdef MAIN_FILE
/*
** months[] and days[] should not be converted to native 
** language since these are used in protocol and SMTP date parsing.
*/

char *months[] = {
    "Jan", "Feb", "Mar", "Apr", "May", "Jun",
    "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"
};

char *days[] = {
    "Sun ", "Mon ", "Tue ", "Wed ", "Thu ", "Fri ", "Sat ", NULL
};

#else
extern char *months[], *days[];
#endif

#include "proto.h"
#include "lang.h"

#ifdef lint
extern int strcasecmp(const char *, const char *);
extern int strncasecmp(const char *, const char *, int);
#endif

#endif				/* ! _HYPERMAIL_HYPERMAIL_H */
