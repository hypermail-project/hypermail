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

#define MAIN_FILE 1

#include "hypermail.h"
#include "defaults.h"
#include "setup.h"
#include "parse.h"
#include "print.h"
#include "finelink.h"
#include "search.h"
#include "struct.h"

#ifdef HAVE_LOCALE_H
#include <locale.h>
#endif

/* #include <stdlib.h> */

extern int getopt(int, char *const *, const char *);
extern char *optarg;
extern int optind;
extern int opterr;

/*
** Prints user startup errors stops. 
*/

void cmderr(char *errorstr)
{
    fprintf(stderr, "\n%s: %s: %s\n", lang[MSG_ERROR], PROGNAME, errorstr);
    usage();
    unlock_archive();
    exit(-1);
}

/*
** Prints a program error string and stops. 
*/

void progerr(char *errorstr)
{
    fprintf(stderr, "%s: %s\n", PROGNAME, errorstr);
    unlock_archive();
    exit(-1);
}

static char *expand_contents(char *variable)
{
    char *return_value = NULL;

    /*
     * Potentially read the header and footer files. Check to make
     * sure the value is a valid file or else assure the original
     * value is there.
     */
    if (variable) {
	/*
	 * Check if valid filename. If not then just return the
	 * variable listed. 
	 * NOTE !! This really should be an error message!
	 */
	if (isfile(variable)) {
	    /* 
	     * Now try to assign the contents of the file to
	     * the return value. If it is able to read the file
	     * then free the existing storage and return.
	     * If not then just return the existing value.
	     * NOTE !! This really should be an error message!
	     */
	    return_value = getfilecontents(variable);
	}
    }
    return return_value;
}

static char *setindex(char *dfltindex, char *indextype, char *suffix)
{
    char *p, *rp;

    p = (!strcmp(dfltindex, indextype)) ? INDEXNAME : indextype;
    rp = (char *)emalloc(strlen(p) + strlen(suffix) + 2);
    sprintf(rp, "%s.%s", p, suffix);

    return (rp);
}


/* Print out the version number and die. */

void version(void)
{
    printf("%s: %s: %s %s: %s\n", PROGNAME,
	   lang[MSG_VERSION], VERSION, lang[MSG_PATCHLEVEL], PATCHLEVEL);
    exit(0);
}

/*
** Prints the usage.
*/

void usage(void)
{
    struct language_entry *lte;

    printf("%s: %s [options]\n", lang[MSG_USAGE], PROGNAME);

    printf("%s:\n", lang[MSG_OPTIONS]);
    printf("  -a URL        : %s\n", lang[MSG_OPTION_A]);
    printf("  -A            : %s\n", lang[MSG_OPTION_ARCHIVE]);
    printf("  -b URL        : %s\n", lang[MSG_OPTION_B]);
    printf("  -c file       : %s\n", lang[MSG_OPTION_C]);
    printf("  -d dir        : %s\n", lang[MSG_OPTION_D]);
    printf("  -g            : %s\n", lang[MSG_OPTION_G]);
    printf("  -i            : %s\n", lang[MSG_OPTION_I]);
    printf("  -l label      : %s\n", lang[MSG_OPTION_L]);
    printf("  -m mbox       : %s\n", lang[MSG_OPTION_M]);
    printf("  -M            : %s\n", "Use metadata");
    printf("  -n listaddr   : %s\n", lang[MSG_OPTION_N]);
    printf("  -o keyword=val: Set config item\n");
    printf("  -p            : %s\n", lang[MSG_OPTION_P]);
    printf("  -s htmlsuffix : %s\n", "HTML file suffix (.html, .htm, ..)");
    printf("  -t            : %s\n", "Use Tables");
    printf("  -T            : %s\n", "Use index tables");
    printf("  -u            : %s\n", lang[MSG_OPTION_U]);
    printf("  -v            : %s\n", lang[MSG_OPTION_VERBOSE]);
    printf("  -V            : %s\n", lang[MSG_OPTION_VERSION]);
    printf("  -x            : %s\n", lang[MSG_OPTION_X]);
    printf("  -1            : %s\n", lang[MSG_OPTION_1]);
    printf("  -L lang       : %s (", lang[MSG_OPTION_LANG]);

    /* Print out languages supported */
    lte = &ltable[0];
    while (lte->langcode != NULL) {
	printf("%s ", lte->langcode);
	lte++;
    }
    printf(")\n");
    printf("%s         : %s\n", lang[MSG_VERSION], VERSION);
    printf("%s      : %s\n", lang[MSG_PATCHLEVEL], PATCHLEVEL);
    printf("%s            : %s\n\n", lang[MSG_DOCS], HMURL);
    exit(1);
}

int main(int argc, char **argv)
{
    int i, use_stdin, use_mbox;
    char *configfile = NULL;
    char **tlang;
    int cmd_show_variables;
    int print_usage;

    int amount_old = 0;		/* number of old mails */
    int amount_new = 0;		/* number of new mails */

#ifdef HAVE_LOCALE_H
    setlocale(LC_ALL, "");
#endif

    lockfile[0] = '\0';
    use_stdin = 0;
    print_usage = 0;
    use_mbox = 0;

    firstdatenum = lastdatenum = 0;

    configfile = strsav(CONFIGFILE);

    cmd_show_variables = 0;

    opterr = 0;

    /* get pre config options here */
    while ((i = getopt(argc,argv,
#ifdef GDBM
			    "a:Ab:c:d:gil:L:m:n:o:ps:tTuvVx0:1M?"
#else
			    "a:Ab:c:d:il:L:m:n:o:ps:tTuvVx0:1M?"
#endif
			    )) != -1) {
        switch((char) i) {
	case 'c':
	    configfile = strreplace(configfile, optarg);
	    break;
	case 'v':
	    cmd_show_variables = TRUE;
	    break;
	case 'V':
	    version();
	 /*NOTREACHED*/ case 'a':
	case 'A':
	case 'b':
	case 'd':
#ifdef GDBM
	case 'g':
#endif
	case 'i':
	case 'l':
	case 'L':
	case 'm':
	case 'n':
	case 'o':
	case 'p':
	case 's':
	case 't':
	case 'T':
	case 'u':
	case 'x':
	case '0':
	case '1':
	case 'M':
	    break;
	case '?':
	default:
	    /* 
	     * Because we need to setup the language support, 
	     * printing of the usage message must be deferred 
	     * until the proper language is determined.
	     */
	    print_usage = 1;
	    break;
	}
    }

    /* 
     * ...then read the configuration file.
     */

    readconfigs(configfile, cmd_show_variables);

    /* reset the getopt() index variable */
    optind = 1;

    /* now get the post-config options! */

    while ((i = getopt(argc,argv,
#ifdef GDBM
			    "a:Ab:c:d:gil:L:m:n:o:ps:tTuvx0:1M?"
#else
			    "a:Ab:c:d:il:L:m:n:o:ps:tTuvx0:1M?"
#endif
      )) != -1) {
        switch((char) i) {
	case 'A':
	    set_append = 1;
	    break;
	case 'a':
	    set_archives = strreplace(set_archives, optarg);
	    break;
	case 'b':
	    set_about = strreplace(set_about, optarg);
	    break;
	case 'c':
	    /* config file from pre-config options */
	    break;
	case 'd':
	    set_dir = strreplace(set_dir, optarg);
	    break;
#ifdef GDBM
	case 'g':
	    set_usegdbm = 1;
	    break;
#endif
	case 'i':
	    use_stdin = TRUE;
	    break;
	case 'l':
	    set_label = strreplace(set_label, optarg);
	    break;
	case 'L':
	    set_language = strreplace(set_language, optarg);
	    break;
	case 'm':
	    set_mbox = strreplace(set_mbox, optarg);
	    break;
	case 'n':
	    set_hmail = strreplace(set_hmail, optarg);
	    break;
	case 'o':
	    ConfigAddItem(optarg);
	    break;
	case 'p':
	    set_showprogress = TRUE;
	    break;
	case 's':
	    set_htmlsuffix = strreplace(set_htmlsuffix, optarg);
	    break;
	case 't':
	    set_usetable = TRUE;
	    break;
	case 'T':
	    set_indextable = TRUE;
	    break;
	case 'u':
	    set_increment = TRUE;
	    break;
	case 'v':
	    cmd_show_variables = TRUE;
	    break;
	case 'x':
	    set_overwrite = TRUE;
	    break;
	case '0':
	    set_delete_msgnum = add_list(set_delete_msgnum, optarg);
	    break;
	case '1':
	    set_readone = TRUE;
	    break;
	case 'M':
	    set_usemeta = TRUE;
	    break;
	case '?':
	default:
	    break;
	}
    }

#ifdef DEBUG
    dump_config();
    exit(0);
#endif

    /*
     * Now override the configuration file variables with any explicitly
     * passed on the command line. This way you need not change the
     * configuration file settings for a minor change in a single run.
     */

    /* 
     * Check and make sure that the supplied language is a
     * valid language. Otherwise strange things happen quickly.
     */

    if ((tlang = valid_language(set_language)) == NULL) {
	sprintf(errmsg, "\"%s\" %s.", set_language,
		lang[MSG_LANGUAGE_NOT_SUPPORTED]);
	cmderr(errmsg);
    }

    lang = tlang;		/* A good language, make it so. */

    if (print_usage)		/* Print the usage message and terminate */
	usage();

    if (set_mbox && !strcasecmp(set_mbox, "NONE")) {
	use_stdin = TRUE;
    }

    /*
     * Did they decide to use stdin by specifying it on the command line ?
     * If not then check and see if that is the default they wanted from
     * the options.h or environment values.
     */
    if (!use_stdin) {
	if (!set_mbox || !strcasecmp(set_mbox, "NONE"))
	    use_stdin = TRUE;
	else
	    use_stdin = FALSE;
    }
    else {
	if (set_mbox)
	    free(set_mbox);
	set_mbox = NULL;
    }

    /*
     * Read the contents of the file into the variables to be used
     * in printing out the pages.
     */

    ihtmlheaderfile = expand_contents(set_ihtmlheader);
    ihtmlfooterfile = expand_contents(set_ihtmlfooter);
    mhtmlheaderfile = expand_contents(set_mhtmlheader);
    mhtmlfooterfile = expand_contents(set_mhtmlfooter);

    if (set_dir)
	set_dir = strreplace(set_dir, dirpath(set_dir));

    /*
     * Default names for directories and labels need to be figured out.
     */

    if (use_stdin && (!set_dir || !strcasecmp(set_dir, "NONE")))
	set_dir = strreplace(set_dir, DIRNAME);

    if (!set_dir || !strcasecmp(set_dir, "NONE"))
	set_dir = strreplace(set_dir, (strrchr(set_mbox, '/')) ?
			     strrchr(set_mbox, '/') + 1 : set_mbox);

    if (set_dir[strlen(set_dir) - 1] != PATH_SEPARATOR)
	trio_asprintf(&set_dir, "%s%c", set_dir, PATH_SEPARATOR);

    if (!set_label || !strcasecmp(set_label, "NONE"))
	set_label = set_mbox ?
	    (strreplace(set_label, (strrchr(set_mbox, '/')) ?
			strrchr(set_mbox, '/') + 1 : set_mbox)) : "stdin";

    /*
     * Which index file will be called "index.html"?
     */

    index_name[1][DATE_INDEX] = setindex(set_defaultindex, "date",
				      set_htmlsuffix);
    index_name[1][THREAD_INDEX] = setindex(set_defaultindex, "thread",
					set_htmlsuffix);
    index_name[1][SUBJECT_INDEX] = setindex(set_defaultindex, "subject",
					 set_htmlsuffix);
    index_name[1][AUTHOR_INDEX] = setindex(set_defaultindex, "author",
					set_htmlsuffix);
    if (set_attachmentsindex) {
	index_name[1][ATTACHMENT_INDEX]
	    = setindex(set_defaultindex, "attachment", set_htmlsuffix);
    }
    if (set_folder_by_date || set_msgsperfolder) {
	index_name[0][DATE_INDEX] = setindex(set_default_top_index, "date",
					     set_htmlsuffix);
	index_name[0][THREAD_INDEX] = setindex(set_default_top_index, "thread",
					       set_htmlsuffix);
	index_name[0][SUBJECT_INDEX] = setindex(set_default_top_index,
						"subject", set_htmlsuffix);
	index_name[0][AUTHOR_INDEX] = setindex(set_default_top_index, "author",
					       set_htmlsuffix);
	if (set_attachmentsindex) {
	    index_name[0][ATTACHMENT_INDEX] = setindex(set_default_top_index,
						       "attachment",
						       set_htmlsuffix);
	}
	index_name[0][FOLDERS_INDEX] = setindex(set_default_top_index,
						"folders", set_htmlsuffix);
    }
    else {
	index_name[0][DATE_INDEX] = index_name[1][DATE_INDEX];
	index_name[0][THREAD_INDEX] = index_name[1][THREAD_INDEX];
	index_name[0][AUTHOR_INDEX] = index_name[1][AUTHOR_INDEX];
	index_name[0][SUBJECT_INDEX] = index_name[1][SUBJECT_INDEX];
	index_name[0][ATTACHMENT_INDEX] = index_name[1][ATTACHMENT_INDEX];
    }
    init_index_names();
    if (set_msgsperfolder && set_folder_by_date) {
	progerr("msgsperfolder and folder_by_date may not be used at the same time!");
    }

    /*
     * General settings for mail command and rewriting.
     */

    if (!set_domainaddr || !strcasecmp(set_domainaddr, "NONE"))
	use_domainaddr = 0;
    else
	use_domainaddr = 1;

    if (!set_mailto || !strcasecmp(set_mailto, "NONE"))
	use_mailto = 0;
    else
	use_mailto = 1;

    if (!set_mailcommand || !strcasecmp(set_mailcommand, "NONE"))
	use_mailcommand = 0;
    else
	use_mailcommand = 1;

    /* 
     * A little performance speed up.  The following was being done
     * over and over in the write functions. This way it is done once.
     * A bigger win on larger archives.
     */

    if (set_htmlbody && !strcasecmp(set_htmlbody, "NONE")) {
	free(set_htmlbody);
	set_htmlbody = NULL;
    }

    if (set_hmail && !strcasecmp(set_hmail, "NONE")) {
	free(set_hmail);
	set_hmail = NULL;
    }

    if (set_archives && !strcasecmp(set_archives, "NONE")) {
	free(set_archives);
	set_archives = NULL;
    }

    if (set_archives && !strcasecmp(set_custom_archives, "NONE")) {
	free(set_custom_archives);
	set_custom_archives = NULL;
    }

    if (set_about && !strcasecmp(set_about, "NONE")) {
	free(set_about);
	set_about = NULL;
    }

    /* Simply show what the values of the variables are and exit */

    if (cmd_show_variables) {
	if (!set_mbox)
	    set_mbox = "NONE";
	MakeConfig(TRUE);
	free(configfile);
	return (0);
    }

    /* Injecting a little sanity... */

    if (use_mbox && use_stdin) {
	cmderr(lang[MSG_CANNOT_READ_FROM_BOTH_FILE_AND_STDIN]);
    }
    if(set_append && use_mbox) {
        cmderr(lang[MSG_CANNOT_BOTH_READ_AND_WRITE_TO_MBOX]);
    }

    gettimezone();
    getthisyear();

    /*
     * MIME processing requires the files be created as they
     * are read in loadheaders() so assure the directories are 
     * there first...
     */

    checkdir(set_dir);

    /*
     * Let's do it.
     */

    if (set_uselock)
	lock_archive(set_dir);

    if (set_increment) {
	int num_displayable;
	if (set_linkquotes)
	    replylist = NULL;
	num_displayable = loadoldheaders(set_dir);
	amount_old = max_msgnum + 1; /* counts gaps as messages */

	/* start numbering at this number */
	amount_new = num_displayable
	  + parsemail(set_mbox, use_stdin, set_readone,
		      set_increment, set_dir, set_inlinehtml, amount_old);
	if (set_linkquotes)
	    analyze_headers(max_msgnum + 1);

	writearticles(amount_old, max_msgnum + 1);

	if (set_delete_msgnum)
	    update_deletions(amount_old);

	if (set_show_msg_links) {
	    fixnextheader(set_dir, amount_old, -1);
	    if (set_showreplies)
		fixreplyheader(set_dir, amount_old, 0);
	    fixthreadheader(set_dir, amount_old);
	}
    }
    else {
	amount_new =
	    parsemail(set_mbox, use_stdin, set_readone, set_increment,
		      set_dir, set_inlinehtml, 0);	/* number from 0 */
	if (set_linkquotes)
	    analyze_headers(max_msgnum + 1);
	writearticles(0, max_msgnum + 1);
    }

    if (amount_new) {		/* Always write the index files */
	if (set_linkquotes) {
	    threadlist = NULL;
	    printedthreadlist = NULL;
	    crossindexthread1(datelist);
	}
	count_deleted(max_msgnum + 1);
	if (show_index[0][DATE_INDEX])
	    writedates(amount_new, NULL);
	if (show_index[0][THREAD_INDEX])
	    writethreads(amount_new, NULL);
	if (show_index[0][SUBJECT_INDEX])
	    writesubjects(amount_new, NULL);
	if (show_index[0][AUTHOR_INDEX])
	    writeauthors(amount_new, NULL);
	if (set_attachmentsindex) {
	    writeattachments(amount_new, NULL);
	}
	if (set_folder_by_date || set_msgsperfolder)
	    write_toplevel_indices(amount_new);
	if (set_monthly_index || set_yearly_index)
	    write_summary_indices(amount_new);
	if (set_latest_folder)
	    symlink_latest();
    }
    else {
	printf("No mails to output!\n");
    }

    if (set_uselock)
	unlock_archive();

#if 0
    /* cleanup */
    /* debug this before we activate it again (Daniel 1999-02-08) */
    ConfigCleanup();
#endif

    if (configfile)
	free(configfile);
    if (ihtmlheaderfile)
	free(ihtmlheaderfile);
    if (ihtmlfooterfile)
	free(ihtmlfooterfile);
    if (mhtmlheaderfile)
	free(mhtmlheaderfile);
    if (mhtmlfooterfile)
	free(mhtmlfooterfile);

    return (0);
}
