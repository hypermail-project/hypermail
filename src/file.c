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
#include "struct.h"
#ifdef HAVE_DIRENT_H
#include <dirent.h>
#else
#include <sys/dir.h>
#endif

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

void symlink_latest()
{
    char filename[MAXFILELEN];
    char dirname[MAXFILELEN];
    struct stat stbuf;
    trio_snprintf(filename, MAXFILELEN, "%s%s.%s",
		  set_dir, set_latest_folder, set_htmlsuffix);
    trio_snprintf(dirname, MAXFILELEN, "%sindex.%s",
		  latest_folder_path, set_htmlsuffix);

    if (!stat(filename, &stbuf) && unlink(filename)) {
	sprintf(errmsg, "%s \"%s\" (latest_folder option).",
		lang[MSG_CANNOT_UNLINK], filename);
	progerr(errmsg);
	return;
    }

    if (symlink(dirname, filename)) {
	sprintf(errmsg, "%s \"%s\" (latest_folder option).",
		lang[MSG_CANNOT_CREATE_SYMLINK], filename);
	progerr(errmsg);
	return;
    }
}

int find_max_msgnum()
{
    DIR *dir;
#ifdef HAVE_DIRENT_H
    struct dirent *entry;
#else
    struct direct *entry;
#endif
    int max_num = -1;
    int num;
    char *s_dir = strsav(set_dir);
    int len = strlen(s_dir);
    if (len > 0 && s_dir[len-1] == PATH_SEPARATOR)
	s_dir[len-1] = 0;
    dir = opendir(s_dir);
    if (dir == NULL)
	return -1;
    if (set_msgsperfolder) {
        int max_folder = -1;
	char *tmpptr;
	while ((entry = readdir(dir))) {
	    const char *p = entry->d_name;
	    while (isdigit(*p))
	        ++p;
	    if (!*p && p > entry->d_name) {
	        num = atoi(entry->d_name);
		if (num > max_folder) {
		    char *full_path;
		    trio_asprintf(&full_path, "%s%d", set_dir, num);
		    if (isdir(full_path))
		        max_folder = num;
		    free(full_path);
		}
	    }
	}
	closedir(dir);
	trio_asprintf(&tmpptr, "%s%d", set_dir, max_folder);
	free(s_dir);
	s_dir = tmpptr;
	if (max_folder == -1)
	    return -1;
	dir = opendir(s_dir);
	if (dir == NULL) {
	    sprintf(errmsg, "internal error find_max_msgnum opening \"%s\".",
		    s_dir);
	    progerr(errmsg);
	}
    }
    while ((entry = readdir(dir))) {
	const char *p = entry->d_name;
	while (isdigit(*p))
	    ++p;
	if (*p == '.' && p >= entry->d_name + 4) {
	    ++p;
	    if (!strcmp(p, set_htmlsuffix)) {
	        num = atoi(entry->d_name);
		if (num > max_num)
		    max_num = num;
	    }
	}
    }
    closedir(dir);
    free(s_dir);
    max_msgnum = max_num;
    return max_num;
}

int is_empty_archive()
{
    DIR *dir;
#ifdef HAVE_DIRENT_H
    struct dirent *entry;
#else
    struct direct *entry;
#endif
    int num_files = 0;
    char *s_dir = strsav(set_dir);
    int len = strlen(s_dir);
    if (len > 0 && s_dir[len-1] == PATH_SEPARATOR)
	s_dir[len-1] = 0;
    dir = opendir(s_dir);
    if (dir == NULL)
	return 1;
    while ((entry = readdir(dir))) {
	const char *p = entry->d_name;
	if (*p != '.') {
	    ++num_files;
	    break;
	}
    }
    closedir(dir);
    free(s_dir);
    return num_files == 0;
}

static char *msgsperfolder_label(char *frmptr, int subdir_no)
{
    register char *aptr;
    char dtstr[DATESTRLEN];
    char c;

    struct Push buff;

    INIT_PUSH(buff);

    aptr = frmptr;

    while ((c = *aptr++)) {
	if (c == '%') {
	    switch (*aptr++) {
	    case '%':		/* Add the % character */
		PushByte(&buff, '%');
		continue;
	    case 'd':		/* directory number, starting at 0 */
		sprintf(dtstr, "%d", subdir_no);
		PushString(&buff, dtstr);
		continue;
	    case 'D':		/* directory number, starting with 1 */
		sprintf(dtstr, "%d", subdir_no+1);
		PushString(&buff, dtstr);
		continue;
	    case 'm':		/* number of first message in directory */
		sprintf(dtstr, "%d", set_msgsperfolder*subdir_no);
		PushString(&buff, dtstr);
		continue;
	    case 'M':		/* number of last message possible */
		sprintf(dtstr, "%d", set_msgsperfolder*(subdir_no + 1) - 1);
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

struct emailsubdir *msg_subdir(int msgnum, time_t date)
{
    static struct emailsubdir *last_subdir;
    char s[DATESTRLEN];
    char desc_buf[DATESTRLEN];
    char *desc;
    char *fmt = set_describe_folder;
    if (set_msgsperfolder > 0) {
	int subdir_no = msgnum / set_msgsperfolder;
	trio_snprintf(s, DATESTRLEN, "%d/", subdir_no);
	if (!fmt)
	    fmt = "%d";
	desc = msgsperfolder_label(fmt, subdir_no);
    }
    else if (set_folder_by_date) {
	strftime(s, DATESTRLEN-1, set_folder_by_date, localtime(&date));
	if (!fmt)
	    fmt = set_folder_by_date;
	strftime(desc_buf, DATESTRLEN, fmt, localtime(&date));
	desc = strsav(desc_buf);

	if(s[0] && s[strlen(s) - 1] != '/')
	    strcat(s, "/");
    }
    else
	return NULL;
    if (!last_subdir || strcmp(s, last_subdir->subdir)) {
	last_subdir = new_subdir(s, last_subdir, desc);
    }
    return last_subdir;
}

/*
 * returns "<a href=...>" that links to to_email from the directory of
 * from_email, or from the set_dir directory if email is NULL.
 */

char *msg_href(struct emailinfo *to_email, struct emailinfo *from_email)
{
    static char buffer[MAXFILELEN];
    if (!from_email && to_email->subdir)
        trio_snprintf(buffer, MAXFILELEN, "<a href=\"%s%.4d.%s\">",
		  to_email->subdir->subdir, to_email->msgnum, set_htmlsuffix);
    else if(!to_email->subdir || to_email->subdir == from_email->subdir)
        trio_snprintf(buffer, MAXFILELEN, "<a href=\"%.4d.%s\">",
		  to_email->msgnum, set_htmlsuffix);
    else
        trio_snprintf(buffer, MAXFILELEN, "<a href=\"%s%s%.4d.%s\">",
		  to_email->subdir->rel_path_to_top,
		  to_email->subdir->subdir, to_email->msgnum, set_htmlsuffix);
    return buffer;
}

char *articlehtmlfilename(struct emailinfo *email)
{
    char *buf;
    trio_asprintf(&buf, "%s%.4d.%s", email->subdir ? email->subdir->full_path
		  : set_dir, email->msgnum, set_htmlsuffix);
    return buf;
}

char *htmlfilename(const char *file, struct emailinfo *email,
		   const char *suffix)
{
    char *buf;
    trio_asprintf(&buf, "%s%s%s%s", email && email->subdir
		  ? email->subdir->full_path : set_dir,
		  file, *suffix ? "." : "", suffix);
    return buf;
}
