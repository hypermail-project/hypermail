/*
**  FILE:          mbox2hypermail.c
**  AUTHOR:        Kent Landfield
**
**  ABSTRACT:      read mailbox and print out messages individually and
***                ship to hypermail to update a database.
**
** This software is Copyright (c) 1989 by Kent Landfield.
**
** Permission is hereby granted to copy, distribute or otherwise 
** use any part of this package as long as you do not try to make 
** money from it or pretend that you wrote it.  This copyright 
** notice must be maintained in any copy made.
**                                                               
*/
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include "lists.h"

#undef isspace

#define USAGE  "usage: %s -D YYYY [ -tvd ] [ mailbox ]\n"

#define MAX_MONTH_LEN 20

char *progname;			/* name of executable            */
char lastline[BUFSIZ];		/* read-behind  buffer           */
char s[BUFSIZ];			/* read buffer                   */
char *year;
char *month;
char *configfile;

int debug;
int verbose;
int test;

FILE *errfp;			/* standard error file pointer   */
FILE *logfp;			/* standard output file pointer  */
FILE *mailbox;

extern char *optarg;
extern int optind;

#ifdef lint 
extern int getopt(int, char * const *, const char *);
extern int strcasecmp(const char *, const char *); 
extern char *strdup(const char *);
#endif 

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

static FILE *efopen(char *file, char *mode)
{
    FILE *fp;
 
    if ((fp = fopen(file, mode)) == NULL) {
        (void)fprintf(errfp, "Can't open file %s\n", file);
        exit(10);
    }
    return (fp);
}

static char *readline(int size, FILE *file)
{
    /*
       ** If not first time through, save the previous line read into lastline.
     */

    if (s[0] != '\0')
	(void)strcpy(lastline, s);

    if (fgets(s, size, file) == NULL)
	return (NULL);

    return (s);
}

static int blankline(char *line)
{
    register char *cp;

    for (cp = line; *cp; cp++) {
	if (!isspace(*cp))
	    return (0);
    }
    return (1);
}

static void process_messages(char *flname)
{
    char msgfile[BUFSIZ];
    char cmdstr[BUFSIZ];
    char from[BUFSIZ];

    int cntr;
    int first;

    FILE *msgfp;

    if (verbose)
	fprintf(stderr, "Processing mailbox %s\n", flname);

    /*
    ** Standard input or a mailbox file ? 
     */
    if (flname != NULL)
	mailbox = efopen(flname, "r");
    else
	mailbox = stdin;

    first = 1;
    cntr = 0;
    lastline[0] = '\0';

    /*
    ** Create message file
    */
    if ((msgfp = safe_tmpfile()) == NULL) {
         fprintf(stderr,"%s: Can't open tempfile\n",progname);
         exit(10);
    }

    if (month != NULL) {  /* AUDIT biege: external input -> cmd exec + bof */
	if (configfile != NULL) {
#ifdef HAVE_SNPRINTF
            snprintf(cmdstr, sizeof(cmdstr), 
#else
	    sprintf(cmdstr, 
#endif
	            "/bin/cat %s | %s -u -i -c %s -d %s/%s/%s",
		    msgfile, HYPERMAIL, configfile, ARCHIVE, year, month);
        }
	else {
#ifdef HAVE_SNPRINTF
            snprintf(cmdstr, sizeof(cmdstr), 
#else
	    sprintf(cmdstr,
#endif
		    "/bin/cat %s | %s -u -i -d %s/%s/%s -l \"%s\" -b %s",
		    msgfile, HYPERMAIL, ARCHIVE, year, month, LABEL,
		    ABOUT_LINK);
        }
    }
    else {
	if (configfile != NULL) {
#ifdef HAVE_SNPRINTF
            snprintf(cmdstr, sizeof(cmdstr), 
#else
	    sprintf(cmdstr, 
#endif
                    "/bin/cat %s | %s -u -i -c %s -d %s/%s",
		    msgfile, HYPERMAIL, configfile, ARCHIVE, year);
        }
	else {
#ifdef HAVE_SNPRINTF
            snprintf(cmdstr, sizeof(cmdstr), 
#else
	    sprintf(cmdstr,
#endif
		    "/bin/cat %s | %s -u -i -d %s/%s -l \"%s\" -b %s",
		    msgfile, HYPERMAIL, ARCHIVE, year, LABEL, ABOUT_LINK);
        }
    }

    if (debug) {
	fprintf(stderr, "cmdstr == [%s]\n", cmdstr);
	exit(0);
    }

    while (readline(sizeof(s), mailbox) != NULL) {
	if (strncmp(s, "From ", 5) == 0) {
	    strcpy(from, s);
	    if (first == 1) {	/* First From in file ? */
		first = 0;
	    }
	    else if (blankline(lastline)) {
		/*
		   ** If From and last line is a blank line we have
		   ** found the message separator. Time to go to work.
		 */

		fflush(msgfp);
		fclose(msgfp);
		if (verbose)
		    fputs(from, errfp);

		if (!test)
		    system(cmdstr);

		++cntr;
		msgfp = efopen(msgfile, "w");
	    }
	}
	(void)fputs(s, msgfp);
    }
    fclose(mailbox);
    fflush(msgfp);
    fclose(msgfp);

    if (verbose)
	fputs(from, errfp);

    if (!test)
	system(cmdstr);

    if (!first)
	++cntr;

    if (verbose)
	fprintf(stderr, "%d messages processed\n", cntr);
}

/*
** Ye Olde Main
*/
int main(int argc, char **argv)
{
    int c;

    progname = argv[0];
    debug = verbose = 0;
    logfp = stdout;
    errfp = stderr;

    configfile = CONFIGFILE;

    if (argc > 1) {
	while ((c = getopt(argc, argv, "c:dtvY:M:")) != EOF) {
	    switch (c) {
	    case 'c':
		configfile = strdup(optarg);
		break;
	    case 'M':
		month = optarg;
                if (strlen(month) > MAX_MONTH_LEN) {
		    (void)fprintf(errfp, "%s: Invalid month format, %d character month maximum.\n", progname, MAX_MONTH_LEN);
		    return (1);
                }
		break;
	    case 'Y':
		year = optarg;
                /* check and make sure it is a year.*/
                if (strlen(year) > 4) {
		    (void)fprintf(errfp, "%s: Invalid year format, 4 digit year maximum.\n", progname);
		    return (1);
                }
    
		break;
	    case 'd':
		debug++;
		break;
	    case 'v':
		verbose++;
		break;
	    case 't':
		test++;
		verbose++;
		break;
	    default:
		(void)fprintf(errfp, USAGE, progname);
		return (1);
	    }
	}
    }

    if (year == NULL) {
	(void)fprintf(errfp, "%s: must specify the 4 digit year\n", progname);
	(void)fprintf(errfp, USAGE, progname);
	return (1);
    }

    if (strcasecmp(configfile, "NONE") == 0)
	configfile = NULL;

    /* if no month just put it in the year directory */

    if ((optind >= argc) || (argc == 1)) {	/* file from stdin */
	process_messages(NULL);
    }
    else {
	for (; optind < argc; optind++)	/* process files to print */
	    process_messages(argv[optind]);
    }
    return (0);			/* terminate this process */
}

