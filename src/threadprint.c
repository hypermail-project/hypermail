#include "hypermail.h"
#include "setup.h"
#include "struct.h"
#include "threadprint.h"
#include "printfile.h"

static void format_thread_info(FILE *, struct emailinfo *, int, int *,
			       struct emailinfo *);
static int finish_thread_levels(FILE **, int, int, int *, FILE **,
				char **, char **, int, struct emailinfo *);

/* Define this to make it print a whole lot of debug output to stdout: */
/* #define DEBUG_THREAD */

#define MAXSTACK 20

/*
** If year and/or month are != -1, only messages within the specified time
** period will be printed.
*/

void print_all_threads(FILE *fp, int year, int month, struct emailinfo *email)
{
    int level = 0;
    int newlevel;
    int i;
    int prev = -1;
    int hide_level = 0;
    int thread_file_depth = (year == -1 &&
			     month == -1 ? set_thread_file_depth : 0);
    static int reply_list_count = 0;
    int stack[MAXSTACK + 1];	/* should be dynamic - this will do for now */
    FILE *fp_stack[MAXSTACK + 1];
    int num_replies[MAXSTACK + 1];
    char *filename_stack[MAXSTACK + 1];
    char *subject_stack[MAXSTACK + 1];
    struct emailsubdir *subdir = email ? email->subdir : NULL;

    struct reply *rp = threadlist;
    while (rp != NULL) {
#if DEBUG_THREAD
	fprintf(stderr, "print_all_threads: message %d prev %d level %d\n",
		rp->msgnum, prev, level);
#endif
	if (rp->msgnum == -1) {
	    level =
		finish_thread_levels(&fp, level, 0, num_replies, fp_stack,
				     filename_stack, subject_stack,
				     thread_file_depth, email);
	    rp = rp->next;
	    continue;
	}
	else if(level == 0 && subdir && rp->data->subdir != subdir) {
	    rp = rp->next;
	    continue;
	}

#if DEBUG_THREAD
	fprintf(stderr, "print_all_threads: %d: %s\n", rp->msgnum,
		rp->data->name);
#endif
	if (prev == -1) {
	    level =
		finish_thread_levels(&fp, level, 0, num_replies, fp_stack,
				     filename_stack, subject_stack,
				     thread_file_depth, email);
	    stack[level] = rp->msgnum;
	}
	else if (hide_level)
	    ;			/* don't change level */ 
	else if (rp->frommsgnum == prev) {
	    if (level < MAXSTACK)
		level++;
	    else
		fprintf(stderr, "thread level too deep - sticking at %d\n",
			MAXSTACK);
	    stack[level] = rp->msgnum;
	    num_replies[level] = 0;
	    if ((level < set_thrdlevels) && !set_indextable) {
		if (level > thread_file_depth) {
		    fprintf(fp, "<ul>\n");
		}
		else if (level < MAXSTACK) {
		    char *filename;
		    char subject[TITLESTRLEN];
		    filename_stack[level] = 
		      maprintf("%u%s", reply_list_count,
			       index_name[subdir != NULL][THREAD_INDEX]);
		    filename = htmlfilename(filename_stack[level], email, "");
		    unlink(filename);	/* so chmod won't fail if someone else owned it */
		    fp_stack[level - 1] = fp;
		    if ((fp = fopen(filename, "w")) == NULL) {
			sprintf(errmsg, "Couldn't write \"%s\".",
				filename);
			progerr(NULL);
		    }
		    sprintf(subject, "thread index level %d", level + 1);
		    subject_stack[level] = strsav(subject);
		    print_index_header(fp, set_label, set_dir,
				       subject, filename);
		    fprintf(fp, "<ul>\n");
		    free(filename);
		    ++reply_list_count;
		}
	    }
	}
	else {
	    /* There are (I think) some optimisations possible here.
	     * 1. could start loop at level-1
	     * Previous branch is just a special case of this branch -
	     * possibly more efficient - probably not worth worrying about.
	     * Paul 12-may-1999
	     */
	    for (i = level; i >= 0; i--) {
		if (stack[i] == rp->frommsgnum) {
		    break;
		}
	    }
	    newlevel = i + 1;
	    if (newlevel == level) {
		/* nothing to do - same level */
	    }
	    else if (newlevel > level) {
		/* I don't think this branch will be used - do the right thing anyway */
#if DEBUG_THREAD
		fprintf(stderr,
			"print_all_threads: unexpected: message %d - level changing from %d to %d\n",
			rp->msgnum, level, newlevel);
#endif
		if (newlevel >= MAXSTACK) {
		    fprintf(stderr,
			    "thread level too deep - sticking at %d\n",
			    MAXSTACK);
		    newlevel = MAXSTACK;
		}

		if (!set_indextable) {
		    while (level < newlevel) {
			if (level < set_thrdlevels) {
			    fprintf(fp, "<ul>\n");
			}
			level++;
		    }
		}
	    }
	    else {
		level =
		    finish_thread_levels(&fp, level, newlevel, num_replies,
					 fp_stack, filename_stack,
					 subject_stack, thread_file_depth,
					 email);
	    }

	    stack[newlevel] = rp->msgnum;
	}

	/* Now print this mail */
	if ((year == -1 || year_of_datenum(rp->data->date) == year)
	    && (month == -1 || month_of_datenum(rp->data->date) == month)
	    && !rp->data->is_deleted)
	    format_thread_info(fp, rp->data, level, num_replies, email);

	prev = rp->msgnum;
	hide_level = (rp->data->is_deleted && rp->frommsgnum != rp->msgnum);
	rp = rp->next;
    }
}

static void format_thread_info(FILE *fp, struct emailinfo *email,
			       int level, int *num_replies,
			       struct emailinfo* subdir_email)
{
    char *subj;

    subj = convchars(email->subject);

    /* Print the thread info */
    if (set_indextable) {
	fprintf(fp,
		"<tr><td>%s%s<strong>%s</strong></a></td>"
		"<td><a name=\"%d\">%s</a></td>" "<td>%s</td></tr>\n",
		level > 1 ? "--&gt; " : "", msg_href(email, subdir_email),
		subj, email->msgnum, email->name, getdatestr(email->date));
    }
    else {
	fprintf(fp, "<li>%s<strong>%s</strong></a> "
		"<a name=\"%d\"><em>%s <small>(%s)</small></em></a>\n",
		msg_href(email, subdir_email),
		subj, email->msgnum, email->name, getdatestr(email->date));
    }
    free(subj);
    ++num_replies[level];

    email->flags |= PRINT_THREAD;	/* its written now */
}

int isreplyto(int msgnum, int parent)
{
    /* returns true if message msgnum is a reply to message parent */
    struct reply *rp = replylist;

    while (rp != NULL) {
	if (rp->frommsgnum == parent) {
	    if (rp->msgnum == msgnum) {
		return 1;
	    }
	}
	rp = rp->next;
    }
    return 0;
}

static int finish_thread_levels(FILE **fp, int level, int newlevel,
				int *num_replies, FILE **fp_stack,
				char **filename_stack,
				char **subject_stack,
				int thread_file_depth, struct emailinfo *email)
{
    if (!set_indextable) {
	while (level > newlevel) {
	    num_replies[level - 1] += num_replies[level];
	    if (level < set_thrdlevels) {
		if (level > thread_file_depth) {
		    fprintf(*fp, "</ul>\n");
		}
		else if (level < MAXSTACK) {
		    char *filename = htmlfilename(filename_stack[level],
						  email, "");
		    fprintf(*fp, "</ul>");
		    printfooter(*fp, ihtmlfooterfile, set_label, set_dir,
				subject_stack[level], filename);
		    fclose(*fp);
		    *fp = fp_stack[level - 1];
		    if (num_replies[level]) {
			fprintf(*fp,
				"<ul><li><a href=\"%s\">%u replies</a></ul>\n",
				filename_stack[level], num_replies[level]);
			if (chmod(filename, set_filemode) == -1) {
			    sprintf(errmsg, "Couldn't chmod \"%s\" to %o.",
				    filename, set_filemode);
			    progerr(NULL);
			}
		    }
		    else
			remove(filename);
		    free(filename_stack[level]);
		    free(filename);
		}
	    }
	    level--;
	}
    }
    else {
	level = newlevel;
    }
    return level;
}

