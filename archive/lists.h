/*
**  FILE:          lists.h  - %W% %G% 
**  AUTHOR:        Kent Landfield
**
**  ABSTRACT:      Define the lists supported by hypermail
**
** This software is Copyright (c) 1996 by Kent Landfield.
**
** Permission is hereby granted to copy, distribute or otherwise 
** use any part of this package as long as you do not try to make 
** money from it or pretend that you wrote it.  This copyright 
** notice must be maintained in any copy made.
**                                                               
*/

#ifdef HYPERDEV

#define HYPERMAIL    "/usr/local/bin/hypermail"
#define ARCHIVE      "/ftp/hypermail/mail-archive"
#define MAILBOXDIR   "/ftp/hypermail/mail-archive/mailbox"
#define LABEL        "Hypermail Development List"
#define ABOUT_LINK   "/hypermail/mail-archive"
#define LISTNAME     "hypermail"
#define CONFIGFILE   "/usr/local/lib/hypermail/hypermail.rc"

#else
# ifdef HYPERTEST

#define HYPERMAIL    "../src/hypermail"
#define ARCHIVE      "../tests/mail-archive"
#define MAILBOXDIR   "../tests/mail-archive/mailbox"
#define LABEL        "Hypermail Test Archive"
#define ABOUT_LINK   "../"
#define LISTNAME     "hypermail-test"
#define CONFIGFILE   "../tests/test.rc"

# else
#define HYPERMAIL    "/usr/local/bin/hypermail"
#define ARCHIVE      "NONE"
#define MAILBOXDIR   "NONE"
#define LABEL        "NONE"
#define ABOUT_LINK   "NONE"
#define LISTNAME     "NONE"
#define CONFIGFILE   "NONE"

# endif /* HYPERTEST */
#endif /* HYPERDEV */
