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

#ifdef __LCC__
#include "../lcc/config.h"
#else
#include "../config.h"
#endif

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

#ifdef HAVE_GDBM_H
#define GDBM 1
#endif

#ifdef NO_MACRO
#undef isspace
#undef isdigit
#undef isalpha
#undef isupper
#undef islower
#undef isxdigit
#undef isalnum
#undef ispunct
#undef isprint
#undef isgraph
#undef iscntrl
#undef isascii
#endif

/*
* this redefines the standard *printf() to use ours 
*/
#define TRIO_REPLACE_STDIO
#define HAVE_SSCANF  /* avoid problems in setup.c with trio_sscanf */
#include <trio.h>

#ifndef NO_FASTREPLYCODE
#define FASTREPLYCODE
#endif

#undef FALSE
#define FALSE 0
#undef TRUE
#define TRUE  1

#define PROGNAME    "hypermail"
#define HMURL       "http://www.hypermail-project.org/"

#define INDEXNAME   "index"
#define DIRNAME     "archive"
#define NONAME      "(no name)"
#define NODATE      "(no date)"
#define NOEMAIL     "(no email)"
#define NOSUBJECT   "(no subject)"

#define GDBM_INDEX_NAME ".hm2index"

/* Name of the Hypertext Archive Overview File an XML file
 * which contains pointers to the various index files
 * and some header information about each mail
 * to allow further processing
 */
#define HAOF_NAME "archive_overview.haof"

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

#define PAGE_TOP     1
#define PAGE_BOTTOM  2

typedef enum { DATE_INDEX, THREAD_INDEX, SUBJECT_INDEX, AUTHOR_INDEX,
    ATTACHMENT_INDEX, FOLDERS_INDEX, NO_INDEX
} mindex_t;

typedef enum {
  FILTERED_DELETE = 1, /*spam */
  FILTERED_EXPIRE = 2,
  FILTERED_OUT = 4,
  FILTERED_REQUIRED = 8,
  FILTERED_OLD = 16,
  FILTERED_NEW = 32,
  FILTERED_DELETE_OTHER = 64
} filtered_reason_t;

typedef enum {
  ANNOTATION_ROBOT_NONE = 0,
  ANNOTATION_ROBOT_NO_FOLLOW = 1,
  ANNOTATION_ROBOT_NO_INDEX = 2
} annotation_robot_t;

typedef enum {
  ANNOTATION_CONTENT_NONE = 0,
  ANNOTATION_CONTENT_EDITED = 1,
  ANNOTATION_CONTENT_DELETED_OTHER = 2,
  ANNOTATION_CONTENT_DELETED_SPAM = 4
} annotation_content_t;

typedef enum {
  FORMAT_FIXED = 0,
  FORMAT_FLOWED = 1,
} textplain_format_t;
/* 
 * Path separator for attachment file path generation
 */
#define PATH_SEPARATOR '/'

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
    int maybereply; /* 0 means pretty sure msgnum is reply to frommsgnum; */
		    /* 1 means there is doubt. */
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

struct emailsubdir {	/* used by set_folder_by_date, set_msgsperfolder */
    struct emailinfo *first_email;
    struct emailinfo *last_email;
    struct emailsubdir *prior_subdir;
    struct emailsubdir *next_subdir;
    char *subdir;
    char *full_path;
    char *rel_path_to_top;	/* usually "../", sometimes "../../" */
				/* will be absolute url if set_base_url != 0 */
    int count;
    char *description;		/* label to go in folders.html */
    time_t a_date; /* date of msg which first generated this subdir. not guaranteed to be earliest date in subdir */
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

    int initial_next_in_thread;	/* msgnum written as next during normal print*/

    struct body *bodylist;
#ifdef FASTREPLYCODE
    struct reply *replylist;    /* list all possible direct replies to this */
    int isreply;
#endif
    struct emailsubdir *subdir;	/* NULL unless set_msgsperfolder or set_folder_by_date */
    annotation_robot_t annotation_robot;	/* special metada for controlling how robots
						   index a message */
    long exp_time;
    annotation_content_t annotation_content;	/* annotations concerning the content of the
						   message (edited, spam, deleted)  */
    int is_deleted;	/* 1=deleted (spam), 2=expired, 4=filtered out, */
			/* 8=filtered (required line missing), 16=deleted (other) */
    int deletion_completed; /* -1 or delete_level that reflects last time */
                            /* that file was rewritten to reflect is_deleted */
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
VAR struct hashemail *deletedlist;
VAR struct reply *replylist;
VAR struct reply *replylist_end; /* last node in replylist */
VAR struct reply *threadlist;
VAR struct reply *threadlist_end; /* last node in threadlist */
VAR struct reply **threadlist_by_msgnum; /* array of ptrs into threadlist */
VAR struct printed *printedlist;
VAR struct printed *printedthreadlist;
VAR struct hashemail *etable[HASHSIZE];
VAR struct emailsubdir *folders;

VAR struct hmlist *show_headers;

VAR char timezonestr[TIMEZONELEN];
VAR char thisyear[YEARLEN];
VAR char errmsg[MAXLINE];
VAR char lockfile[MAXFILELEN];
#if 0     /* replaced by index_name */
VAR char *authname;		/* By author index file             */
VAR char *datename;		/* By date index file               */
VAR char *subjname;		/* By subject index file            */
VAR char *thrdname;		/* By thread index file             */
VAR char *attname;		/* By attachment index file         */
VAR char *top_index[NO_INDEX];
#endif
#define MAX_DIRECTORY_LEVELS 2
VAR char *index_name[MAX_DIRECTORY_LEVELS][NO_INDEX];
VAR int show_index[MAX_DIRECTORY_LEVELS][NO_INDEX];

VAR char *antispamdomain;

VAR int use_mailcommand;
VAR int use_mailto;
VAR int use_domainaddr;

VAR char *ihtmlheaderfile;
VAR char *ihtmlfooterfile;
VAR char *ihtmlheadfile;
VAR char *ihtmlhelpupfile;
VAR char *ihtmlhelplowfile;
VAR char *ihtmlnavbar2upfile;
VAR char *mhtmlheaderfile;
VAR char *mhtmlfooterfile;

VAR long firstdatenum;
VAR long lastdatenum;
VAR int max_msgnum;

VAR char **msgnum_id_table;

VAR const char *latest_folder_path;

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
