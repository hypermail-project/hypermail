/*
**  FILE:          msg2archive.c
**  AUTHOR:        Kent Landfield
**
**  ABSTRACT:      update archived mailbox and 
***                ship to appropriate hypermail to update a database.
**
** This software is Copyright (c) 1996 by Kent Landfield.
**
** Permission is hereby granted to copy, distribute or otherwise 
** use any part of this package as long as you do not try to make 
** money from it or pretend that you wrote it.  This copyright 
** notice must be maintained in any copy made.
**                                                               
*/

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <fcntl.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include "lists.h"

char s[BUFSIZ];			/* read buffer                      */
char cmdstr[BUFSIZ];

char *progname;			/* name of executable               */
char *month;			/* 3 letter mon abreviation         */

/*
** List information variables
*/

char *configfile = CONFIGFILE;	/* list's configuration file     */
char *listname = LISTNAME;	/* name of the list              */
char *hypermail = HYPERMAIL;	/* path to hypermail executable  */
char *archive = ARCHIVE;	/* list archive base directory   */
char *mailboxdir = MAILBOXDIR;	/* path to mailbox directory     */
char *label = LABEL;		/* list's title/lable            */
char *about_link = ABOUT_LINK;	/* list's about link             */

int year;			/* 4 digit year                     */
int verbose;
int test;
int c;

time_t clk;

struct tm *now;

FILE *msgfp;
FILE *mailbox;
FILE *hypfp;

char *months[] = { "Jan", "Feb", "Mar", "Apr", "May",
    "Jun", "Jul", "Aug", "Sep", "Oct",
    "Nov", "Dec"
};

extern int opterr;
extern char *optarg;

static int usage(void)
{
    (void)fprintf(stderr, "usage: %s [options] (file on stdin only)\n", progname);
    (void)fprintf(stderr, "       -A archive-basedir\n");
    (void)fprintf(stderr, "       -b about_link\n");
    (void)fprintf(stderr, "       -c configfile\n");
    (void)fprintf(stderr, "       -H hypermail executable\n");
    (void)fprintf(stderr, "       -L listname\n");
    (void)fprintf(stderr, "       -l list lable\n");
    (void)fprintf(stderr, "       -M mailbox directory\n");
    (void)fprintf(stderr, "       -t testing (no execution, assumes -v)\n");
    (void)fprintf(stderr, "       -v verbose\n");
    return (-1);
}

static FILE *safe_tmpfile(void)
{
  int fd;
  char *tfile;

  tfile = strdup("hn-inXXXXXX");

  if ((fd = mkstemp(tfile)) < 0)
    return(NULL);

  unlink(tfile);

  if (fchmod(fd, S_IRUSR | S_IWUSR) != 0)
    return(NULL);

  return(fdopen(fd, "w+b"));
}

int main(int argc, char **argv)
{
    int inheader;

    verbose = 0;
    test = 0;
    opterr = 0;

    if ((progname = strrchr(argv[0], '/')) == NULL)
	progname = argv[0];
    else
	progname++;

    if (argc > 1) {
	while ((c = getopt(argc, argv, "A:b:c:H:L:l:M:tv")) != EOF) {
	    switch (c) {
	    case 'A':
		archive = optarg;
		break;
	    case 'b':
		about_link = optarg;
		break;
	    case 'c':
		configfile = optarg;
		break;
	    case 'H':
		hypermail = optarg;
		break;
	    case 'L':
		listname = optarg;
		break;
	    case 'l':
		label = optarg;
		break;
	    case 'M':
		mailboxdir = optarg;
		break;
	    case 'v':
		verbose++;
		break;
	    case 't':
		test++;
		verbose++;
		break;
	    default:
		return (usage());
	    }
	}
    }

    if (strcasecmp(configfile, "NONE") == 0)
	configfile = NULL;

    /* get the year and the month so that we can set up archive info */

    clk = time((time_t *) 0);
    now = localtime(&clk);

    year = 1900 + now->tm_year;
    month = months[now->tm_mon];

    /*
    ** Create temporary message file
    ** and save the inbound message.
    */

    if ((msgfp = safe_tmpfile()) == NULL) {
         fprintf(stderr,"%s: Can't open tempfile\n",progname);
	 exit(10);
    }

    inheader = 1;		/* guilty until proven innocent */

    while (fgets(s, sizeof(s), stdin) != NULL) {
	/*
	   ** Need to escape lines in the body that
	   ** start with a "^From "
	 */
	if (*s == '\n')
	    inheader = 0;

	if (!inheader) {
	    if (!strncmp(s, "From ", 5))
		fputs(">", msgfp);
	}
	fputs(s, msgfp);
    }

    fflush(msgfp);

    /* 
    ** Append it to the current month's message file
    */

    /* AUDIT Thomas Biege: mailboxdir + listname are cmd line options */
    snprintf(cmdstr, sizeof(cmdstr), "%s/%s.%.2d%.2d", mailboxdir, listname,
             year, now->tm_mon + 1);

    if (verbose)
	fprintf(stderr, "Appending message to [%s]\n", cmdstr);

    if (!test) {
        /*  AUDIT Thomas Biege: "../" in mailboxdir or listname can be used
        **               to write to files below the target direc.!
        **               Take Care! I'll not fix it.
        */
	if ((mailbox = fopen(cmdstr, "a+")) == NULL) {
	    fprintf(stderr,"%s: Can't append message to %s\n",progname,cmdstr);
	    fprintf(stderr, "uid = %ld - gid = %ld euid = %ld - egid = %ld\n",
		    (long)getuid(), (long)getgid(), (long)geteuid(),
		    (long)getegid());

	    exit(10);
	}
	rewind(msgfp);
	while (fgets(s, sizeof(s), msgfp) != NULL)
	    fputs(s, mailbox);
	fclose(mailbox);
    }

    /* 
    ** Send it to hypermail to archive it in the current month's archive
    */

    if (configfile == NULL)
        snprintf(cmdstr, sizeof(cmdstr), "%s -u -i -d %s/%d/%s -l \"%s\" -b %s",
                 hypermail, archive, year, month, label, about_link);
    else
        snprintf(cmdstr,sizeof(cmdstr),"%s -u -i -c %s",hypermail,configfile);

    if (verbose)
	fprintf(stderr, "Piping message to [%s]\n", cmdstr);

    if (!test) {
       /* 
       ** Need to consider replacing with a safer popen.
       */
        if ((hypfp = popen(cmdstr, "w")) != NULL) {
	    rewind(msgfp);
	    while (fgets(s, sizeof(s), msgfp) != NULL)
		fputs(s, hypfp);
	    fflush(hypfp);
	    pclose(hypfp);
	}
    }
    fclose(msgfp); /* not needed for [s_]tmpfile() */
    return (0);			/* terminate this process */
}


