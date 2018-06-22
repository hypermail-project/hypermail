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

#include <errno.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <pwd.h>

#include "hypermail.h"
#include "setup.h"
#include "struct.h"
#include "parse.h"
#ifdef HAVE_DIRENT_H
#ifdef __LCC__
#include "../lcc/dirent.h"
#include <direct.h>
#else
#include <dirent.h>
#endif
#else
#include <sys/dir.h>
#endif
#ifdef GDBM
#include "gdbm.h"
#endif
#ifdef HAVE_LIBFNV
/*
** Since LCC won't use configure which moves fnv.h to this directory,
** include it in its original location.
*/
#ifdef __LCC__
#include "fnv/fnv.h"
#else
#include "fnv.h"
#endif
#endif /* HAVE_LIBFNV */

extern int snprintf(char *str, size_t size, const  char  *format, ...);


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
    /*
    ** LCC only has the short mkdir().  Fortunately, we do a chmod
    ** immediately afterward, so it's a don't care.
    */
#ifdef __LCC__
	if (errno != ENOENT || mkdir(dir) < 0) {
#else
	if (errno != ENOENT || mkdir(dir, set_dirmode) < 0) {
#endif
            if (errno != EEXIST) {
		snprintf(errmsg, sizeof(errmsg), "%s \"%s\".", lang[MSG_CANNOT_CREATE_DIRECTORY], dir);
		progerr(errmsg);
	    }
	}
	if (set_showprogress)
			printf(" %s \"%s\", %s %o.\n", lang[MSG_CREATING_DIRECTORY], dir, lang[MSG_MODE], set_dirmode);

	if (chmod(dir, set_dirmode) == -1) {
			snprintf(errmsg, sizeof(errmsg), "%s \"%s\" to %o.", lang[MSG_CANNOT_CHMOD], dir, set_dirmode);
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
	    /* See comment in check1dir */
#ifdef __LCC__
		if (errno != ENOENT || mkdir(dir) < 0) {
#else
		if (errno != ENOENT || mkdir(dir, set_dirmode) < 0) {
#endif
		    if (errno != EEXIST) {
			snprintf(errmsg, sizeof(errmsg), "%s \"%s\".", lang[MSG_CANNOT_CREATE_DIRECTORY], dir);
			progerr(errmsg);
		    }
		}
		if (set_report_new_folder) {
		    printf("%s\n", dir);
		}
		if (set_showprogress)
		    printf(" %s \"%s\", %s %o.\n", lang[MSG_CREATING_DIRECTORY], dir, lang[MSG_MODE], set_dirmode);

		if (chmod(dir, set_dirmode) == -1) {
		    snprintf(errmsg, sizeof(errmsg), "%s \"%s\" to %o.", lang[MSG_CANNOT_CHMOD], dir, set_dirmode);
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
	char tmppath[MAXFILELEN]; /*AUDIT biege: pathname + filename is more then 4KB long on linux */
        struct passwd *pp;

#ifndef __LCC__
	/*
	** Getting password data from /etc/passwd is pretty silly in
	** Win9x systems since nearly everybody builds this file after
	** they set up $HOME.  Just skip this try at finding a default
	** location for the config file and go on to try $HOME.
	*/
        if ((pp = getpwuid(getuid())) != NULL) {
			snprintf(tmppath, sizeof(tmppath), "%s%s", pp->pw_dir, path + 1);	/* AUDIT biege: who gurantees that path+1 contains data? */
            ConfigInit(tmppath);
        } 
	else
#endif
            if ((ep = getenv("HOME")) != NULL) { /* AUDIT biege: possible BOF.. but it's not setuid.. so why to care? */
			snprintf(tmppath, sizeof(tmppath), "%s%s", ep, path + 1);	/* AUDIT biege: who gurantees that path+1 contains data? */
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
    /*
    ** Symlinks work so differently in Windows that I think we'll just
    ** skip that whole thing and ignore that option.
    */
#ifdef __LCC__
    snprintf(errmsg, sizeof(errmsg),
	"WARNING: latest_folder not supported in Win32 environment.\n");
    fprintf(stderr, "%s", errmsg);
#else
    char filename[MAXFILELEN];
    
    struct stat stbuf;

    if (!latest_folder_path)
	return;			/* haven't created new folder this time? */
    trio_snprintf(filename, MAXFILELEN, "%s%s", set_dir, set_latest_folder);

    if (!stat(filename, &stbuf) && unlink(filename)) {
		snprintf(errmsg, sizeof(errmsg), "%s \"%s\" (latest_folder option).", lang[MSG_CANNOT_UNLINK], filename);
	progerr(errmsg);
	return;
    }

    if (symlink(latest_folder_path, filename)) {
		snprintf(errmsg, sizeof(errmsg), "%s \"%s\" (latest_folder option).", lang[MSG_CANNOT_CREATE_SYMLINK], filename);
	progerr(errmsg);
	return;
    }
#endif
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
    int len = (int)strlen(s_dir);

    if (len > 0 && s_dir[len - 1] == PATH_SEPARATOR)
       s_dir[len - 1] = 0;
    dir = opendir(s_dir);
    if (dir == NULL)
	return -1;
#ifdef GDBM
    if (set_folder_by_date && set_usegdbm) {
	return loadoldheadersfromGDBMindex(set_dir, 1) - 1;
    }
#endif
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
			snprintf(errmsg, sizeof(errmsg), "internal error find_max_msgnum opening \"%s\".", s_dir);
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
    return max_num;
}

/*
** Returns a buffer with  the name of the message index name file.
** The caller has to free this buffer.
*/

char *messageindex_name(void)
{
  char *buf;

	trio_asprintf(&buf, "%s%s", set_dir, "msgindex");
  return (buf);
}

/*
** Returns the corresponding message number from the messageindex file.
*/
int find_max_msgnum_id()
{
    int max_num = -1;

    FILE *fp;
    char line[MAXLINE];
    int maxnum;
    int startnum;
    char *buf;
    
    /* open the index file */
    buf = messageindex_name();
    fp = fopen(buf, "r");
    free(buf);
    if (fp) {
        fgets(line, sizeof(line), fp);
        if (2 == sscanf(line, "%04d %04d", &startnum, &maxnum))
        max_num = maxnum;
        fclose(fp);
    }
    return max_num;
}

/* 
** Get a list of msgid corresponding to hypermail msg numbers
*/
char **read_msgnum_id_table(int max_num)
{
    char **table;
    int read_msgs;
    FILE *fp;
    char line[MAXLINE];
    char *buf;

    if (max_num == -1)
      return NULL;

	table = (char **)calloc(sizeof(char *), max_num + 1);

    /* open the index file */
	buf = messageindex_name();
	fp = fopen(buf, "r");
	free(buf);

    /* skip the max_msgnum (first) line */
	fgets(line, sizeof(line), fp);

    read_msgs = 0;
	while (fgets(line, sizeof(line), fp) && read_msgs <= max_num) {
	char *msgid;
	int num;
		msgid = malloc(strlen(line) + 1);
		sscanf(line, "%d %s\n", &num, msgid);
	/* was the message skipped? */
		if (read_msgs < num) {
			while (read_msgs < num) {
		table[read_msgs] = NULL;
		read_msgs++;
	      }
	  }
	table[read_msgs] = msgid;
	read_msgs++;
      }
	fclose(fp);
    
    return table;
}

/* 
** Frees the table used to store the msgnum id correspondance
*/
void free_msgnum_id_table(char *table[], int max_num)
{
  int i;

  if (!table)
    return;

  for (i = 0; i < max_num; i++)
    if (table[i])
			free(table[i]);
	free(table);
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
    int len = (int)strlen(s_dir);
    if (len > 0 && s_dir[len - 1] == PATH_SEPARATOR)
        s_dir[len - 1] = 0;
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
				sprintf(dtstr, "%d", subdir_no + 1);
		PushString(&buff, dtstr);
		continue;
	    case 'm':		/* number of first message in directory */
				sprintf(dtstr, "%d", set_msgsperfolder * subdir_no);
		PushString(&buff, dtstr);
		continue;
	    case 'M':		/* number of last message possible */
				sprintf(dtstr, "%d", set_msgsperfolder * (subdir_no + 1) - 1);
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
    static struct emailsubdir *subdir;
    char s[DATESTRLEN];
    char desc_buf[DATESTRLEN];
    char *desc = NULL;
    char *fmt = set_describe_folder;
    if (set_msgsperfolder > 0) {
	int subdir_no = msgnum / set_msgsperfolder;
	trio_snprintf(s, DATESTRLEN, "%d/", subdir_no);
	if (!fmt)
	    fmt = "%d";
	desc = msgsperfolder_label(fmt, subdir_no);
    }
    else if (set_folder_by_date) {
	strftime(s, DATESTRLEN - 1, set_folder_by_date, localtime(&date));
	if (!fmt)
	    fmt = set_folder_by_date;
	strftime(desc_buf, DATESTRLEN, fmt, localtime(&date));
	desc = strsav(desc_buf);

	if (s[0] && s[strlen(s) - 1] != '/')
	    strcat(s, "/");
    }
    else
	return NULL;
    subdir = last_subdir;
    if (!last_subdir || strcmp(s, last_subdir->subdir)) {
	subdir = new_subdir(s, last_subdir, desc, date);
	if (set_increment != -1)
	    last_subdir = subdir;
    }
    if (desc)
      free(desc);
    return subdir;
}

/*
** Returns the filename we want to use. According to the convention, this
** can be the msgnumber, the msgid, or some other kind of name. 
*/
char *message_name (struct emailinfo *email)
{
  static char buffer[8 + sizeof (time_t) * 2 + 1];

#ifdef HAVE_LIBFNV
  if (set_nonsequential && email->msgid)
    {
      /* Call the FNV msg hash library */
      Fnv32_t hash_val; 

      hash_val = fnv_32_buf(email->msgid, strlen (email->msgid), FNV1_32_INIT);
      /* the line below is what we used before when the hash included
	 the fromdate string, and we didn't concatenate the mail date.
	 However, we changed strategies to avoid collisions. */
      /* hash_val = fnv_32_str(email->fromdatestr, hash_val); */
      sprintf (buffer, "%08x%08x", hash_val, email->fromdate);

      return buffer;
    }
  else
    {
#endif /* HAVE_LIBFNV */
      sprintf (buffer, "%.4d", email->msgnum);
      return buffer;
#ifdef HAVE_LIBFNV
    }
#endif /* HAVE_LIBFNV */
}

/*
 * returns "<a href=...>" that links to to_email from the directory of
 * from_email, or from the set_dir directory if email is NULL.
 */

char *msg_href(struct emailinfo *to_email, struct emailinfo *from_email, bool generate_markup)
/* note: you probably have to make a copy of
 * the buffer returned before the next call to this function.
 */
{
    static char buffer[MAXFILELEN + 11];
    char *ptr;

    ptr = msg_relpath(to_email, from_email);
    if (generate_markup)
      trio_snprintf(buffer, MAXFILELEN + 11, "<a href=\"%s\">", 
		    ptr);
    else
      trio_snprintf(buffer, MAXFILELEN + 11, "%s", ptr);

    return buffer;
}


char *msg_relpath(struct emailinfo *to_email, struct emailinfo *from_email)
/* called by msg_href() : note: you probably have to make a copy of
 * the buffer returned before the next call to this function.
 */
{
    static char buffer[MAXFILELEN];
    char *name;

    name = message_name(to_email);

    if (!from_email && to_email->subdir)
	trio_snprintf(buffer, MAXFILELEN, "%s%s.%s", to_email->subdir->subdir, name, set_htmlsuffix);
    else if (!to_email->subdir || to_email->subdir == from_email->subdir)
	trio_snprintf(buffer, MAXFILELEN, "%s.%s", name, set_htmlsuffix);
    else
	trio_snprintf(buffer, MAXFILELEN, "%s%s%s.%s", to_email->subdir->rel_path_to_top, to_email->subdir->subdir, name, set_htmlsuffix);

    return buffer;
}

char *articlehtmlfilename(struct emailinfo *email)
{
    char *buf;
    char *name;

    name = message_name(email);

    trio_asprintf(&buf, "%s%s.%s", email->subdir ? email->subdir->full_path : set_dir, name, set_htmlsuffix);
    return buf;
}

char *htmlfilename(const char *file, struct emailinfo *email, const char *suffix)
{
    char *buf;

    trio_asprintf(&buf, "%s%s%s%s", email && email->subdir ? email->subdir->full_path : set_dir, file, *suffix ? "." : "", suffix);

    return buf;
}

char *haofname(struct emailinfo *email)
{
    char *buf;

    trio_asprintf(&buf, "%s%s", email && email->subdir ? email->subdir->full_path : set_dir, HAOF_NAME);

    return buf;

}

/* matches_existing returns 0 if it finds a file with the same msgnum as
 argument eptr but different contents. A return value of 1 does not
 guarantee that they match, it only says a difference wasn't found
 (which can mean that no file was found). 
*/

int matches_existing(int msgnum)
{
  struct emailinfo *eptr;
  if (hashnumlookup(msgnum, &eptr) == NULL)
      return -1;

#ifdef GDBM
  if (set_usegdbm) {
      char *indexname;
      GDBM_FILE gp;
      int num;

      trio_asprintf(&indexname, "%s%s", set_dir, GDBM_INDEX_NAME);

      if ((gp = gdbm_open(indexname, 0, GDBM_READER, 0, 0))) {

	/* we _can_ read the index */

	datum content;
	datum key;
	int max_num;

	key.dptr = (char *)&num;
	key.dsize = (int)sizeof(num);

	num = -1;
	content = gdbm_fetch(gp, key);
	if (!content.dptr)
	    max_num = -1;
	else
	    max_num = atoi(content.dptr);

	if (eptr->msgnum <= max_num) {
	  char *dp, *dp_end;
	  char *name = NULL;
	  char *email = NULL;
	  char *date = NULL;
	  char *msgid = NULL;
	  char *fromdate = NULL;

	  num = eptr->msgnum;
	  key.dptr = (char *)&num;
	  key.dsize = (int)sizeof(num);
	  content = gdbm_fetch(gp, key);
	  if (!(dp = content.dptr)) {
	      return 1;
	  }
	  dp_end = dp + content.dsize;
	  fromdate = dp;
	  dp += strlen(dp) + 1;
	  date = dp;
	  dp += strlen(dp) + 1;
	  name = dp;
	  dp += strlen(dp) + 1;
	  email = dp;
	  dp += strlen(dp) + 1;
	  dp += strlen(dp) + 1;
	  msgid = dp;

	  if (strcmp(msgid, eptr->msgid))
	      return 0;
	}

	gdbm_close(gp);
      } /* end case of able to read gdbm index */

      free(indexname);
  }  
#endif
  if (!set_usegdbm) {
	int msgids_are_same;
	msgids_are_same = parse_old_html(msgnum, eptr, 0, 0, NULL, 1);
	return msgids_are_same != 0;
  }
  return 1;
}
