#include "hypermail.h"
#include "setup.h"
#include "struct.h"
#include "threadprint.h"

static void print_replies(FILE *, struct emailinfo *, struct header *, int *);
static void format_thread_info(FILE *, struct emailinfo *, int);

/* Define this to make it print a whole lot of debug output to stdout: */
/* #define DEBUG_THREAD */

#define MAXSTACK 20

void print_all_threads(FILE *fp)
{
    int level = 0;
    int newlevel;
    int i;
    int prev = -1;
    int stack[MAXSTACK + 1];	/* should be dynamic - this will do for now */

    struct reply *rp = threadlist;
    while (rp != NULL) {
#if DEBUG_THREAD
	fprintf(stderr, "print_all_threads: message %d prev %d level %d\n",
		rp->msgnum, prev, level);
#endif
	if (rp->msgnum == -1) {
	    /* end of thread */
	    if (!set_indextable) {
		while (level > 0) {
		    if (level < set_thrdlevels) {
			fprintf(fp, "</ul>\n");
		    }
		    level--;
		}
	    }
	    else {
		level = 0;
	    }
	    rp = rp->next;
	    continue;
	}

#if DEBUG_THREAD
	fprintf(stderr, "print_all_threads: %d: %s\n", rp->msgnum,
		rp->data->name);
#endif
	if (prev == -1) {
	    /* end of thread */
	    if (!set_indextable) {
		while (level > 0) {
		    if (level < set_thrdlevels) {
			fprintf(fp, "</ul>\n");
		    }
		    level--;
		}
	    }
	    else {
		level = 0;
	    }
	    stack[level] = rp->msgnum;
	}
	else if (rp->frommsgnum == prev) {
	    if (level < MAXSTACK)
		level++;
	    else
		fprintf(stderr, "thread level too deep - sticking at %d\n",
			MAXSTACK);
	    stack[level] = rp->msgnum;
	    if ((level < set_thrdlevels) && !set_indextable)
		fprintf(fp, "<ul>\n");
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
		if (!set_indextable) {
		    while (level > newlevel) {
			if (level < set_thrdlevels) {
			    fprintf(fp, "</ul>\n");
			}
			level--;
		    }
		}
		else {
		    level = newlevel;
		}
	    }

	    stack[newlevel] = rp->msgnum;
	}

	/* Now print this mail */
	format_thread_info(fp, rp->data, level);

	prev = rp->msgnum;
	rp = rp->next;
    }
}

static void format_thread_info(FILE *fp, struct emailinfo *email,
			       int level)
{
    char *subj;

    subj = convchars(email->subject);

    /* Print the thread info */
    if (set_indextable) {
	fprintf(fp,
		"<tr><td>%s<a href=\"%.4d.%s\"><strong>%s</strong></a></td>"
		"<td><a name=\"%d\">%s</a></td>" "<td>%s</td></tr>\n",
		level > 1 ? "--&gt; " : "", email->msgnum, set_htmlsuffix,
		subj, email->msgnum, email->name, getdatestr(email->date));
    }
    else {
	fprintf(fp, "<li><a href=\"%.4d.%s\">"
		"<strong>%s</strong></a> "
		"<a name=\"%d\"><em>%s <small>(%s)</small></em></a>\n",
		email->msgnum, set_htmlsuffix,
		subj, email->msgnum, email->name, getdatestr(email->date));
    }
    free(subj);

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

static void print_replies(FILE *fp, struct emailinfo *email,
			  struct header *dl, int *level)
{
    struct emailinfo *em;

    /* Find all replies to the 'email' email and print them. For each reply
       search for replies for them! */
    if (dl->left)
	print_replies(fp, email, dl->left, level);

#ifdef DEBUG_THREAD
    printf("mail %d checks %s\n", email->msgnum, dl->data->subject);
#endif


    em = dl->data;

    /* If this mail's In-Reply-To header matches the Message-Id of the
       "father" mail, we have a match; */
    if (!(em->flags & PRINT_THREAD) &&
	isreplyto(em->msgnum, email->msgnum)) {

	/* Don't "indent" more than wanted */
	if ((*level < set_thrdlevels) && !set_indextable)
	    fprintf(fp, "<ul>\n");

	(*level)++;

	if (!(em->flags & PRINT_THREAD)) {
	    /* Now print this mail */
	    format_thread_info(fp, em, *level);
	}
#ifdef DEBUG_THREAD
	printf("PRINT REPLIES to %s\n", em->subject);
#endif
	/* Now, output the replies to this mail: */
	print_replies(fp, em, datelist, level);

	(*level)--;

	/* re-indent back if we indented up there */
	if ((*level < set_thrdlevels) && !set_indextable)
	    fprintf(fp, "</ul>\n");
    }

    /* Now, find later replies */
    if (dl->right)
	print_replies(fp, email, dl->right, level);
}
