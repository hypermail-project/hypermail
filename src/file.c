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

#include "hypermail.h"
#include "setup.h"

/*
** Does a file exist?
*/

int isfile(char *path)
{
    struct stat stbuf;

    if (stat(path, &stbuf))
	return 0;
    return ((stbuf.st_mode & S_IFMT) == S_IFREG) ? 1 : 0;
}

/*
** Does a directory exist?
*/

int isdir(char *path)
{
    struct stat stbuf;

    if (stat(path, &stbuf))
	return 0;
    return ((stbuf.st_mode & S_IFMT) == S_IFDIR) ? 1 : 0;
}

/*
** This tries to create and chmod a directory.
*/

void check1dir(char *dir)
{
    struct stat sbuf;

    if (stat(dir, &sbuf)) {
	if (errno != ENOENT || mkdir(dir, set_dirmode) < 0) {
	    sprintf(errmsg, "%s \"%s\".",
		    lang[MSG_CANNOT_CREATE_DIRECTORY], dir);
	    progerr(errmsg);
	    return;
	}
	else if (set_showprogress)
	    printf(" %s \"%s\", %s %o.\n",
		   lang[MSG_CREATING_DIRECTORY], dir,
		   lang[MSG_MODE], set_dirmode);

	if (chmod(dir, set_dirmode) == -1) {
	    sprintf(errmsg, "%s \"%s\" to %o.",
		    lang[MSG_CANNOT_CHMOD], dir, set_dirmode);
	    progerr(errmsg);
	}
    }
}

/*
** This tries to create and chmod a path to a directory.
*/

void checkdir(char *dir)
{
    register char *p;
    struct stat sbuf;
    int ch;

    p = dir;

    if (*p && *p == '/')	/* get off root */
	p++;

    for (;; ++p) {
	if (!*p || *p == '/') {
	    ch = *p;
	    *p = '\0';
	    if (stat(dir, &sbuf)) {
		if (errno != ENOENT || mkdir(dir, set_dirmode) < 0) {
		    sprintf(errmsg, "%s \"%s\".",
			    lang[MSG_CANNOT_CREATE_DIRECTORY], dir);
		    progerr(errmsg);
		    return;
		}
		else if (set_showprogress)
		    printf(" %s \"%s\", %s %o.\n",
			   lang[MSG_CREATING_DIRECTORY], dir,
			   lang[MSG_MODE], set_dirmode);

		if (chmod(dir, set_dirmode) == -1) {
		    sprintf(errmsg, "%s \"%s\" to %o.",
			    lang[MSG_CANNOT_CHMOD], dir, set_dirmode);
		    progerr(errmsg);
		}
	    }
	    *p = ch;
	    if (!*p)
		break;
	}
    }
}

char *getfilecontents(char *filename)
{
    FILE *infile;
    struct stat finfo;
    char *retval;

    if ((infile = fopen(filename, "r")) == NULL)
	return (NULL);

    if (fstat(fileno(infile), &finfo) != 0) {
	(void)fclose(infile);
	return (NULL);
    }

    if (finfo.st_size == 0) {
	(void)fclose(infile);
	return (NULL);
    }

    else {
	if (!(retval = (char *)malloc((unsigned)finfo.st_size + 1))) {
	    (void)fclose(infile);
	    return (NULL);
	}
	if (!fread(retval, (size_t) finfo.st_size, 1, infile)) {
	    (void)fclose(infile);
	    free(retval);
	    return (NULL);
	}
	*(retval + finfo.st_size) = '\0';
    }
    (void)fclose(infile);
    return (retval);
}


/*
** expand_path - fill in values substituting for magic cookies
**
**      Substitution cookies supported
**
**              %d - two digit day of month (1-28/30/31)
**              %D - three letter day of the week
**              %m - two digit month of year (1-12)
**              %M - three letter month of year (Jan, Feb, ..., Dec)
**              %y - four digit year (1990,..2001)
**
** Returns:   expanded path string
*/

char *dirpath(char *frmptr)
{
    register char *aptr;
    char dtstr[DATESTRLEN];
    char c;
    struct tm *now;
    time_t clk;

    struct Push buff;

    INIT_PUSH(buff);

    clk = time((time_t *)0);
    now = localtime(&clk);

    aptr = frmptr;

    while ((c = *aptr++)) {
	if (c == '%') {
	    switch (*aptr++) {
	    case '%':		/* Add the % character */
		PushByte(&buff, '%');
		continue;
	    case 'd':		/* two digit day of month (1-31) */
		sprintf(dtstr, "%.2d", now->tm_mday);
		PushString(&buff, dtstr);
		continue;
	    case 'D':		/* three letter day of week */
		PushString(&buff, days[now->tm_wday]);
		continue;
	    case 'j':		/* julian date */
		sprintf(dtstr, "%.3d", now->tm_yday);
		PushString(&buff, dtstr);
		continue;
	    case 'm':		/* two digit month of year (1-12) */
		sprintf(dtstr, "%.2d", now->tm_mon + 1);
		PushString(&buff, dtstr);
		continue;
	    case 'M':		/* three letter month of year */
		PushString(&buff, months[now->tm_mon]);
		continue;
	    case 'y':		/* 4 digit year */
		sprintf(dtstr, "%.4d", now->tm_year + 1900);
		PushString(&buff, dtstr);
		continue;
	    default:
		PushString(&buff, "%?");
		continue;
	    }			/* end switch */
	}

	PushByte(&buff, c);
    }				/* end while */

    RETURN_PUSH(buff);
}

/*
** Reads a configuration file if it exists and puts all the right
** values into the right variables.
*/

void readconfigs(char *path, int cmd_show_variables)
{
    if (path && path[0] == '~') {
    	char *ep; 
        char tmppath[MAXFILELEN];
        struct passwd *pp;

        if ((pp = getpwuid(getuid())) != NULL) {
            sprintf(tmppath, "%s%s", pp->pw_dir, path + 1);
            ConfigInit(tmppath);
        } 
        else if ((ep = getenv("HOME")) != NULL) {
            sprintf(tmppath, "%s%s", ep, path + 1);
            ConfigInit(tmppath);
        }
        /* 
         * So what happens here if the above two conditions fail ????
         * Simply use the compiled in defaults ?
         */
    }    
    else {
	ConfigInit(path);

	if (set_showprogress && !cmd_show_variables)
	    printf("%s: %s\n", lang[MSG_PATH], path);
    }
}
