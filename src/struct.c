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

/* Define the following define to enable better date debugging */
/*#define PH_DATE_DEBUG */
int rbs = 0;
int rbs_bigtime = 0;

#include "hypermail.h"
#include "dmatch.h"
#include "setup.h"
#include "struct.h"
#include "parse.h"
#include "getname.h"

#define HAVE_PCRE
#ifdef HAVE_PCRE
#ifdef __LCC__
#include "../lcc/pcre.h"
#else
#include <pcre.h>
#endif
#endif

struct body *hashnumlookup(int, struct emailinfo **);

/* 
** Ladies and germs - the hash function.
*/

unsigned hash(char *s)
{
    unsigned hashval;

    /* We MUST deal with NULL pointers passed to this */
    if (!s)
	return HASHSIZE - 1;	/* (Daniel) I think a high value is fun ;-) */

    for (hashval = 0; *s; s++)
	hashval = *s + 31 * hashval;
    return (hashval % HASHSIZE);
}

void reinit_structs()
{
    int i;
    for (i = 0; i < HASHSIZE; ++i) {
	if (etable[i]) {
	    free(etable[i]);
	    etable[i] = NULL;
	}
    }
}

void fill_email_dates(struct emailinfo *e, char *date, char *fromdate, char *isodate, char *isofromdate)
{
    bool fromdate_valid = 1, date_valid = 1;
    /* fromdate may be empty (zero length string rather than NULL)
     * date will always have something in it - either a date or
     * the NODATE string "(no date)"
     */
    if (isodate != NULL && isofromdate != NULL) {
	e->date = iso_to_secs(isodate);
	e->fromdate = iso_to_secs(isofromdate);
	e->fromdatestr = strsav(fromdate);
	e->datestr = strsav(date);
    }
    else {
	e->date = e->fromdate = -1;
	if (date == NULL)
	    date = NODATE;
	if (!strcmp(date, NODATE)) {
	    date_valid = 0;
	}
	else {
	    if ((e->date = convtoyearsecs(date)) == -1)
		date_valid = 0;
	}
	if (!*fromdate) {
	    fromdate_valid = 0;
	}
	else {
	    if ((e->fromdate = convtoyearsecs(fromdate)) == -1)
		fromdate_valid = 0;
	}
	if (fromdate_valid && !date_valid) {
#ifdef PH_DATE_DEBUG
			fprintf(stderr, "%d: %s: using fromdate '%s' for both (date '%s')\n", num, msgid, fromdate, date);
#endif
	    e->fromdatestr = strsav(fromdate);
	    e->datestr = strsav(fromdate);
	    e->date = e->fromdate;
	}
	else if (!fromdate_valid && date_valid) {
#ifdef PH_DATE_DEBUG
			fprintf(stderr, "%d: %s: using date '%s' for both (fromdate '%s')\n", num, msgid, date, fromdate);
#endif
	    e->fromdatestr = strsav(date);
	    e->datestr = strsav(date);
	    e->fromdate = e->date;
	}
	else if (!fromdate_valid && !date_valid) {
#ifdef PH_DATE_DEBUG
			fprintf(stderr, "%d: %s: fromdate '%s' and date '%s' both bad\n", num, msgid, fromdate, date);
#endif
	    e->fromdatestr = strsav(fromdate);
	    e->datestr = strsav(date);
	}
	else {
	    e->fromdatestr = strsav(fromdate);
	    e->datestr = strsav(date);
#ifdef PH_DATE_DEBUG
	    if (e->date > e->fromdate) {
		rbs++;
		if ((e->date - e->fromdate) > 4200) {	/* if more than 1:10 off */
		    char fromdate_parsed[26];
		    char date_parsed[26];
		    strcpy(fromdate_parsed, ctime(&(e->fromdate)));
		    strcpy(date_parsed, ctime(&(e->date)));
		    date_parsed[24] = '\0';
		    fromdate_parsed[24] = '\0';
		    rbs_bigtime++;
		    fprintf(stderr, "%d [%d:%d]: %s: received before sent\n %-38.38s %-30.38s\n %-38.38s %-38.38s\n", num, rbs, rbs_bigtime, msgid, fromdate, date, fromdate_parsed, date_parsed);
		}
	    }
#endif
	}
    }
}

/*
** The structure most of everything else depends on.
** Hashes a message - header info, pointer to a list of body lines -
** by number, message ID, date, and subject, so an article can be
** handily looked up and retrieved using any of these criteria.
*/

struct emailinfo *addhash(int num, char *date, char *name, char *email, char *msgid, char *subject, char *inreply, char *fromdate, char *charset, char *isodate, char *isofromdate, struct body *sp)
{
    struct emailinfo *e;
    struct hashemail *h;

    unsigned hashval;
    char numstr[NUMSTRLEN];
    bool msgid_dup = 0;
    bool msgid_missing = 0;
    static int freedummy = 0;
    char *newmsgid;

    if (!msgid) {
	/* SEVERE ERROR, all mails MUST have a Message-ID, ignore it! */
	if (set_require_msgids) {
	    if (set_warn_suppressions)
				printf("Message-ID is missing, ignoring message with subject '%s'.\n", subject ? subject : "<unknown>");
	    return NULL;
	}
	else {
	    msgid_missing = 1;
	}
    }

    if (!msgid_missing) {
	h = (struct hashemail *)etable[hash(msgid)];

	while (h) {
	    if (!strcmp(h->data->msgid, msgid)) {
		msgid_dup = 1;
		break;
	    }
	    h = h->next;
	}
    }

    if (msgid_dup && set_discard_dup_msgids) {
	if (set_showprogress)
		printf("%d: Message-ID collision, '%s' already present - discarding message.\n", num, msgid);
	return NULL;
    }

    if (msgid_dup || msgid_missing) {
        /* 
         * This is broken. It needs to be corrected so that
         * the original message id is a component of the
         * generated message-id instead of a total unrelated
         * dummy msg-id. Good enough for now though...
         */
	newmsgid = emalloc(MSGDSTRLEN);
	do {
	    msgid_dup = 0;
	    sprintf(newmsgid, "%d.%4.4d@hypermail.dummy", time(NULL), freedummy);
	    h = (struct hashemail *)etable[hash(newmsgid)];
	    while (h) {
		if (!strcmp(h->data->msgid, newmsgid)) {
		    msgid_dup = 1;
		    break;
		}
		h = h->next;
	    }
	    freedummy++;
	} while (msgid_dup && (freedummy < 1000));

	if (!msgid_dup) {
            if (set_showprogress) {
	        if (msgid_missing) 
		    printf("\n%d Message-ID missing, using %s instead.\n", num, newmsgid);
	        else
					printf("\n%d Message-ID collision, '%s' already present - using %s instead.\n", num, msgid, newmsgid);
            }
	    msgid = newmsgid;
	}
	else {
            if (set_showprogress) {
	        if (msgid_missing)
		    printf("\n%d Message-ID missing, failed to find free id - dropping message.\n", num);
	        else
					printf("\n%d Message-ID collision, '%s' already present - failed to find free id - dropping message.\n", num, msgid);
            }
	    return NULL;
	}
    }

    e = (struct emailinfo *)emalloc(sizeof(struct emailinfo));
#ifdef FASTREPLYCODE
    e->replylist = NULL;
    e->isreply = 0;
#endif
    e->msgnum = num;
    if (num > max_msgnum)
        max_msgnum = num;
    e->emailaddr = strsav(email);
    if ((name == NULL) || (*name == '\0'))
	e->name = strsav(email);
    else
	e->name = strsav(name);

    fill_email_dates(e, date, fromdate, isodate, isofromdate);
    e->subdir = msg_subdir(e->msgnum, set_use_sender_date ? e->date
			   : e->fromdate);
    if (e->subdir && set_increment != -1) {
	if (!e->subdir->first_email)
	    e->subdir->first_email = e;
	e->subdir->last_email = e;
	++e->subdir->count;
    }
    e->msgid = strsav(msgid);
    e->subject = strsav(subject);
    e->unre_subject = unre(subject);
    e->inreplyto = strsav(inreply);
    e->charset = strsav(charset);
    e->flags = 0;
    e->is_deleted = 0;
    e->deletion_completed = -1;
    e->exp_time = -1;
    e->bodylist = sp;
    e->initial_next_in_thread = -1;

    /* Added by Daniel 1999-03-19, we need this hash later to find the mail
       we replied to */
    h = (struct hashemail *)emalloc(sizeof(struct hashemail));
    hashval = hash(inreply);
    h->next = etable[hashval];
    h->data = e;
    etable[hashval] = h;

    h = (struct hashemail *)emalloc(sizeof(struct hashemail));
    hashval = hash(date);
    h->next = etable[hashval];
    h->data = e;
    etable[hashval] = h;

    h = (struct hashemail *)emalloc(sizeof(struct hashemail));
#if 0
    printf("ADD msgid %s to HASH!\n", msgid);
#endif
    hashval = hash(msgid);
    h->next = etable[hashval];
    h->data = e;
    etable[hashval] = h;

    h = (struct hashemail *)emalloc(sizeof(struct hashemail));
    hashval = hash(subject);
    h->next = etable[hashval];
    h->data = e;
    etable[hashval] = h;

    h = (struct hashemail *)emalloc(sizeof(struct hashemail));
    sprintf(numstr, "%d", num);
    hashval = hash(numstr);
    h->next = etable[hashval];
    h->data = e;
    etable[hashval] = h;

    return e;			/* the actual mail struct pointer */
}

int insert_in_lists(struct emailinfo *emp, const bool * require_filter, int rlen)
{
    int i;
    if (set_delete_msgnum) {
	char num_str[32];
	sprintf(num_str, "%d", emp->msgnum);
	if (inlist(set_delete_msgnum, num_str)) {
#if 0
	    if (!emp->is_deleted) {
				if (emp->subdir)
		    --emp->subdir->count;
	    }
#endif
	    emp->is_deleted = FILTERED_DELETE;
	}
    }
	for (i = 0; i < rlen; ++i) {
	if (!require_filter[i]) {
	    emp->is_deleted = FILTERED_REQUIRED;
	}
    }

    if (emp->is_deleted) {
	struct hashemail *h;
	h = (struct hashemail *)malloc(sizeof(struct hashemail));
	h->data = emp;
	h->next = deletedlist;
	deletedlist = h;
	if (set_warn_suppressions && require_filter) {
	    const char *option = "??";
			switch (emp->is_deleted) {
			case FILTERED_DELETE:
				option = "deleted or delete_msgnum";
				break;
			case FILTERED_EXPIRE:
				option = "expires";
				break;
			case FILTERED_OUT:
				option = "filter_out or filter_out_full_body";
				break;
			case FILTERED_REQUIRED:
				option = "filter_require or filter_require_full_body";
				break;
			case FILTERED_OLD:
				option = "delete_older";
				break;
			case FILTERED_NEW:
				option = "delete_newer";
				break;
	    }
			printf("message %d deleted under option %s. msgid: %s\n", emp->msgnum + 1, option, emp->msgid);
	}
    }
    else {
        authorlist = addheader(authorlist, emp, 1, 0);

	subjectlist = addheader(subjectlist, emp, 0, 0);

    }
    datelist = addheader(datelist, emp, 2, 0);
    return !emp->is_deleted;
}

/*
 * Given an "in-reply-to:" field and a message number, this function
 * retrieves information about the message that this message is a 
 * reply to.
 * If all else fails but a reply is
 * found by comparing subjects, issubjmatch is set to 1.
 *
 * Daniel 1999-03-19. Changed return type and parameters.
*/
#ifdef USE_OBSOLETE_HASHREPLYLOOKUP

struct emailinfo *hashreplylookup(int msgnum, char *inreply, int *issubjmatch)
{
    struct hashemail *ep;

#if DEBUG_THREAD
    fprintf(stderr, "hashreplylookup(%d, '%s'...)\n", msgnum, inreply);
#endif
    *issubjmatch = 0;
    ep = etable[hash(inreply)];
    while (ep) {
		if ((msgnum != ep->data->msgnum) && (strcmp(inreply, ep->data->msgid) == 0)) {
#if DEBUG_THREAD
	    fprintf(stderr, "match on msgid [%d]\n", ep->data->msgnum);
#endif
	    return ep->data;
	}
	ep = ep->next;
    }

    ep = etable[hash(inreply)];
    while (ep != NULL) {
		if ((msgnum != ep->data->msgnum) && (strstr(inreply, ep->data->datestr))) {
#if DEBUG_THREAD
	    fprintf(stderr, "match on date [%d]\n", ep->data->msgnum);
#endif
	    return ep->data;
	}
	ep = ep->next;
    }

    ep = etable[hash(inreply)];
    while (ep != NULL) {
		if ((msgnum != ep->data->msgnum) && (strcmp(inreply, ep->data->subject) == 0)) {
	    *issubjmatch = 1;
#if DEBUG_THREAD
	    fprintf(stderr, "match on subject [%d]\n", ep->data->msgnum);
#endif
	    return ep->data;
	}
	ep = ep->next;
    }

    return NULL;
}
#endif

/*
 * Given an "message-id:" field, this tries to retrieve information
 * about an article that has replied. If all else fails but a reply is
 * found by comparing subjects, issubjmatch is set to 1.
 *
 * Daniel 1999-03-24. New.
*/

struct emailinfo *hashmsgidlookup(char *msgid, int *issubjmatch)
{
    struct hashemail *ep;

    *issubjmatch = 0;
    ep = etable[hash(msgid)];
    while (ep) {
	if (strcmp(msgid, ep->data->inreplyto) == 0) {
	    return ep->data;
	}
	ep = ep->next;
    }

    return NULL;
}

/*
 * Given an "in-reply-to:" field and a message number, this function
 * retrieves information about the message that this message is a 
 * reply to.
 * If all else fails but a reply is
 * found by comparing subjects, maybereply is set to 1.
*/

struct emailinfo *hashreplylookup(int msgnum, char *inreply, char *subject, int *maybereply)
{
    struct hashemail *ep;

#if DEBUG_THREAD > 1
	fprintf(stderr, "hashreplynumlookup(%d, '%s'...)\n", msgnum, (inreply == NULL) ? "" : inreply);
#endif
    *maybereply = 0;

    if ((inreply != NULL) && *inreply) {

	ep = etable[hash(inreply)];
	while (ep) {
	    if (strcmp(inreply, ep->data->msgid) == 0) {
#if DEBUG_THREAD
				fprintf(stderr, "match on msgid   %4d %4d\n", msgnum, ep->data->msgnum);
#endif
		return ep->data;
	    }
	    ep = ep->next;
	}

	ep = etable[hash(inreply)];
	while (ep) {
			if ((msgnum != ep->data->msgnum) && (strstr(inreply, ep->data->datestr))) {
#if DEBUG_THREAD
				fprintf(stderr, "match on date    %4d %4d\n", msgnum, ep->data->msgnum);
#endif
		return ep->data;
	    }
	    ep = ep->next;
	}

	ep = etable[hash(inreply)];
	while (ep != NULL) {
			if ((msgnum != ep->data->msgnum) && (strcmp(inreply, ep->data->subject) == 0)) {
		*maybereply = 1;
#if DEBUG_THREAD
				fprintf(stderr, "match on subject %4d %4d\n", msgnum, ep->data->msgnum);
#endif
		return ep->data;
	    }
	    ep = ep->next;
	}

    }				/* end of matching on inreply */

    /* No match so far.  Now try matching on the subject, removing
     * one instance of "re: " from the front of the subject each
     * time round the loop.
     */
    {
#define MAX_SUBJ_LEN 300

	char *s, *saved_s;
	struct emailinfo *lowest_so_far = NULL;
	int match = 0;

        size_t subj_len;

        subj_len = strlen(subject) > MAX_SUBJ_LEN ? MAX_SUBJ_LEN : strlen(subject);
        s = emalloc(subj_len + 1);
        saved_s = strncpy(s, subject, subj_len);
        s[subj_len] = '\0';

#ifdef NOTDEF
/* AUDIT biege: can we trigger a DoS here by using a very long "Subject"? */

	s = emalloc(strlen(subject) + 1);	
	saved_s = strcpy(s, subject);
#endif

        if (isre(s, NULL))
            do {
#if DEBUG_THREAD > 1
                fprintf(stderr, "extra %s\n", s);
#endif
                ep = etable[hash(s)];
                while (ep != NULL) {
					if ((strcasecmp(s, ep->data->subject) == 0) && (msgnum != ep->data->msgnum)) {
                        match = 1;
						if (lowest_so_far == NULL || ep->data->msgnum < lowest_so_far->msgnum)
                            lowest_so_far = ep->data;
                    }
                    ep = ep->next;
                }
                s = oneunre(s);
            } while (s != NULL);

	free(saved_s);

	if (match) {
	    *maybereply = 1;
	    if (lowest_so_far != NULL && lowest_so_far->msgnum < msgnum) {
#if DEBUG_THREAD
				fprintf(stderr, "match on extra   %4d %4d\n", msgnum, lowest_so_far->msgnum);
#endif
		return lowest_so_far;
	    }
	    else {
#if DEBUG_THREAD
				fprintf(stderr, "match on extra   %4d %4d discarded - less than %d\n", msgnum, lowest_so_far ? lowest_so_far->msgnum : -1, msgnum);
#endif
		return NULL;
	    }
	}
    }

#if DEBUG_THREAD
    fprintf(stderr, "match NO MATCH   %4d\n", msgnum);
#endif
    return NULL;
}

/*
** Same as the above function, but only returns the article number.
*/

int hashreplynumlookup(int msgnum, char *inreply, char *subject, int *maybereply)
{
    struct emailinfo *email = hashreplylookup(msgnum, inreply, subject,
					      maybereply);
    return email != NULL ? email->msgnum : -1;
}


/*
 * From an article's number, retrieve all information associated with
 * that article.
 */

struct body *hashnumlookup(int num, struct emailinfo **emailp)
{
    struct hashemail *ep;
    struct body *lp_tmp;
    char numstr[NUMSTRLEN];

    sprintf(numstr, "%d", num);
    for (ep = etable[hash(numstr)]; ep != NULL; ep = ep->next) {
	if (ep->data && (num == ep->data->msgnum)) {
	    /* return a mere pointer to it! */
	    *emailp = ep->data;
	    if (!ep->data->bodylist)
	        ep->data->bodylist = addbody(NULL, &lp_tmp, "\n", 0);
	    return ep->data->bodylist;
	}
    }
    return NULL;
}

/*
 * returns info about the first message associated with the given msgid.
 */

struct emailinfo *hashlookupbymsgid(char *msgid)
{
    struct hashemail *ep;
    if (!msgid || !*msgid)
	return NULL;
    ep = etable[hash(msgid)];
    while (ep) {
	if (strcmp(msgid, ep->data->msgid) == 0) {
	    return ep->data;
	}
	ep = ep->next;
    }

    return NULL;
}

int insert_older_msgs(int num)
{
    int i;
    int num_added = 0;
    for(i = set_startmsgnum; i < num; ++i) {
	struct emailinfo *emp;
	if (hashnumlookup(i, &emp)) {
	    authorlist = addheader(authorlist, emp, 1, 0);
	    subjectlist = addheader(subjectlist, emp, 0, 0);
	    datelist = addheader(datelist, emp, 2, 0);
	    ++num_added;
	}
    }
    return num_added;
}


/*
 * Find the nearest non-deleted email to num by adding direction to num.
 */

struct emailinfo *neighborlookup(int num, int direction)
{
    struct hashemail *ep;
    char numstr[NUMSTRLEN];
    num += direction;

    while (num >= 0 && num <= max_msgnum) {
	sprintf(numstr, "%d", num);
	for (ep = etable[hash(numstr)]; ep != NULL; ep = ep->next)
	    if (ep->data && (num == ep->data->msgnum)) {
	        if (ep->data->is_deleted)
		    break;
	          /* return a mere pointer to it! */
		return ep->data;
	    }
	num += direction;
    }
    return NULL;
}

/*
** Add a line to a linked list that makes up a boundary stack. This new one 
** should be the new "active" boundary.
**
** "Adding" a NULL will retrieve the formerly used boundary pointer.
*/

struct boundary *bound(struct boundary *bnd, char *line)
{
    struct boundary *newnode = NULL;

    if (line) {
	newnode = (struct boundary *)emalloc(sizeof(struct boundary));
	newnode->line = strsav(line);
	newnode->next = NULL;
	newnode->prev = bnd;

	if (bnd)
	    bnd->next = newnode;

	bnd = newnode;
    }
    else {
	if (bnd->prev) {
	    /* go back to the previous */
	    bnd = bnd->prev;

	    /* free the latest one */
	    free(bnd->next->line);
	    free(bnd->next);
	}
	else {
	    /* this is the last node */
	    free(bnd->line);
	    free(bnd);
	    bnd = NULL;
	}
    }
    return bnd;		/* the new "active" boundary */
}


/*
** Add a line to a linked list that makes up an article's body.
*/

struct body *addbody(struct body *bp, struct body **lp,	/* points to the last pointer */
		     char *line, int flags)
{
    struct body *tempnode;
    struct body *newnode = NULL;

    if (!(flags & BODY_CONTINUE)) {
	newnode = (struct body *)emalloc(sizeof(struct body));
	memset(newnode, 0, sizeof(struct body));
	newnode->line = spamify(strsav(line));
	newnode->html = (flags & BODY_HTMLIZED) ? 1 : 0;
	newnode->header = (flags & BODY_HEADER) ? 1 : 0;
	newnode->attached = (flags & BODY_ATTACHED) ? 1 : 0;
	newnode->next = NULL;
    }
    if (bp == NULL) {
	bp = newnode;
	*lp = bp;
    }
    else {
	tempnode = *lp;		/* get pointer right away */

	if (flags & BODY_CONTINUE) {
	    /* this is a continuation of the previous line */
	    int newlen;
	    char *newbuf;

	    /* get the new size + 1 for the terminating zero */
	    newlen = strlen(tempnode->line) + strlen(line) + 1;

	    /* extend the former memory area: */
	    newbuf = (char *)realloc(tempnode->line, newlen);

	    /* if successful, continue */
	    if (newbuf) {
		/* remove LF from the first part: */
		char *lf;
		lf = strchr(newbuf, '\n');
		if (lf)
		    *lf = 0;

		/* append the new part */
		strcat(newbuf, line);

		/* point out the new buffer instead */
		tempnode->line = newbuf;
	    }
	}
	else {
	    tempnode->next = newnode;
	    *lp = newnode;
	}
    }
    return bp;
}

/*
** Remove the last empty lines, if any, from an article body's linked list.
*/

int rmlastlines(struct body *bp)
{
    struct body *tempnode;

	if (NULL == bp)
      return 0;

	for (tempnode = bp; tempnode->next != NULL && (tempnode->next->line)[0] != '\0'; tempnode = tempnode->next);
    if ((tempnode->line)[0] == '\n') {
	(tempnode->line)[0] = '\0';
	return 1;
    }
    return 0;
}

/* copy bp on to the end of origbp, deallocate bp */

struct body *append_body(struct body *origbp, struct body **origlp, struct body *bp)
{
    while (bp) {
	struct body *next;
		origbp = addbody(origbp, origlp, bp->line, (bp->header ? BODY_HEADER : 0)
			 | (bp->html ? BODY_HTMLIZED : 0)
			 | (bp->attached ? BODY_ATTACHED : 0));
	next = bp->next;
	free(bp->line);
	free(bp);
	bp = next;
    }
    return origbp;
}

void free_body(struct body *bp)
{
    while (bp != NULL) {
	struct body *cp = bp->next;
	if (bp->line)
	    free(bp->line);
	free(bp);
	bp = cp;
    }
}

/*
** If a message is a reply to another, that message's number and the number of
** the message it may be referring to is put in this list.  
*/

struct reply *addreply(struct reply *rp, int fromnum, struct emailinfo *email, int maybereply, struct reply **last_node)
{
    struct reply *tempnode, *newnode;

    newnode = (struct reply *)emalloc(sizeof(struct reply));
    newnode->frommsgnum = fromnum;
    if (email == NULL)
	newnode->msgnum = -1;
    else {
	newnode->msgnum = email->msgnum;
#ifdef FASTREPLYCODE
	email->isreply = 1;
#endif
    }
    newnode->data = email;
    newnode->maybereply = maybereply;
    newnode->next = NULL;

    if (!rp) {
	rp = newnode;
#ifdef FASTREPLYCODE
	if (!threadlist && threadlist_by_msgnum && email)
	    threadlist_by_msgnum[email->msgnum] = rp;
#endif
    }
    else {
#ifdef FASTREPLYCODE
      for (tempnode = (last_node ? *last_node : rp); tempnode->next != NULL; tempnode = tempnode->next);
      tempnode->next = newnode;
      if (rp == threadlist && threadlist_by_msgnum && email)
	    threadlist_by_msgnum[email->msgnum] = tempnode;
#else
		for (tempnode = rp; tempnode->next != NULL; tempnode = tempnode->next);
	tempnode->next = newnode;
#endif
    }
    if (last_node)
	*last_node = newnode;

    return rp;
}

struct reply *addreply2(struct reply *rp, struct emailinfo *from_email, struct emailinfo *email, int maybereply, struct reply **last_node)
{
#ifdef FASTREPLYCODE
    struct reply *tempnode;
    for (tempnode = rp; tempnode != NULL; tempnode = tempnode->next) {
	if (tempnode->msgnum == email->msgnum) { /* duplicate? */
	    if (tempnode->maybereply)
	        tempnode->maybereply = maybereply;
	    return rp;		/* don't add 2nd time */
	}
    }
    from_email->replylist = addreply(from_email->replylist, from_email->msgnum, email, maybereply, NULL);
#endif
    return addreply(rp, from_email->msgnum, email, maybereply, last_node);
}

/*
** Mark an article number as having been printed.
*/

struct printed *markasprinted(struct printed *pp, int num)
{
    struct printed *tempnode, *newnode;

    newnode = (struct printed *)emalloc(sizeof(struct printed));
    newnode->msgnum = num;
    newnode->next = NULL;

    if (pp == NULL)
	pp = newnode;
    else {
		for (tempnode = pp; tempnode->next != NULL; tempnode = tempnode->next);
	tempnode->next = newnode;
    }
    return pp;
}

/*
** Has an article already been printed?
*/

int wasprinted(struct printed *list, int num)
{
    struct printed *pp;

    for (pp = list; pp != NULL; pp = pp->next)
	if (pp->msgnum == num)
	    return 1;
    return 0;
}

/*
** Add article header information to a binary tree and sort by date,
** subject, or author. This is necessary for printing the index files.
*/

struct header *addheader(struct header *hp, struct emailinfo *email, int sorttype, int depth)
{
    int isbigger;
    long yearsecs;
    static int max_depth, count_d;

    isbigger = 0;
    if (hp == NULL) {
	max_depth = depth;
	hp = (struct header *)emalloc(sizeof(struct header));
	if (!hp)
	    return NULL;

	hp->data = email;

	if (sorttype == 2) {
	    yearsecs = hp->data->datenum = email->fromdate;
	    if (set_use_sender_date)
	        yearsecs = hp->data->datenum = email->date;
	    if (!firstdatenum || yearsecs < firstdatenum)
		firstdatenum = yearsecs;
	    if (yearsecs > lastdatenum)
		lastdatenum = yearsecs;
	}
	else
	    hp->data->datenum = 0;
	hp->left = hp->right = NULL;
	return hp;
    }

    switch (sorttype) {
    case 1:
	isbigger = (strcasecmp(email->name, hp->data->name) > 0) ? 0 : 1;
	break;
    case 0:
		isbigger = (strcasecmp(email->unre_subject, hp->data->unre_subject) > 0) ? 0 : 1;
	break;
    case 2:
	yearsecs = email->fromdate;
	if (set_use_sender_date)
	    yearsecs = email->date;
	if (set_reverse)
	    isbigger = (yearsecs < hp->data->datenum) ? 0 : 1;
	else
	    isbigger = (yearsecs >= hp->data->datenum) ? 0 : 1;
	if (!firstdatenum || yearsecs < firstdatenum)
	    firstdatenum = yearsecs;
	if (yearsecs > lastdatenum)
	    lastdatenum = yearsecs;
	break;
    }

    if (isbigger)
	hp->left = addheader(hp->left, email, sorttype, depth + 1);
    else
	hp->right = addheader(hp->right, email, sorttype, depth + 1);

    if (sorttype == 2 && depth < max_depth / 2 && !(++count_d % 3)) {
	/* semi-random rebalancing */
	struct header **hpp = (set_reverse ? &hp->left : &hp->right);
	struct header *hp1 = *hpp;
	if (hp1 != NULL && (hp1->right != NULL || hp1->left != NULL)) {
	    if (hp1->right != NULL && (hp1->left == NULL || (count_d & 1))) {
		struct header *p = hp1->right;
		hp1->right = p->left;
		p->left = hp1;
		*hpp = p;
	    }
	    else {
		struct header *p = hp1->left;
		hp1->left = p->right;
		p->right = hp1;
		*hpp = p;
	    }
	}
    }

    return hp;
}

struct emailsubdir *new_subdir(char *subdir, struct emailsubdir *last_subdir, char *description, time_t date)
{
    struct emailsubdir *sd = folders;
    struct emailsubdir *new_sd;
    struct emailsubdir *insert_point = NULL;
    int count;
    char *p;

    while (sd) {
	if (!strcmp(subdir, sd->subdir))
	    return sd;
		if (sd->a_date < date && (!insert_point || sd->a_date > insert_point->a_date))
	    insert_point = sd;
	if (sd->next_subdir == NULL)
	    break;
	sd = sd->next_subdir;
    }
    new_sd = (struct emailsubdir *)emalloc(sizeof(struct emailsubdir));
    new_sd->next_subdir = NULL;
    new_sd->prior_subdir = NULL;
    if (insert_point == NULL)
	insert_point = sd;
    if (insert_point == NULL)
	folders = (set_increment == -1 ? NULL : new_sd);
    else if (date < folders->a_date) {
	new_sd->next_subdir = folders;
	folders->prior_subdir = new_sd;
	folders = new_sd;
    }
    else {
	new_sd->next_subdir = insert_point->next_subdir;
	insert_point->next_subdir = new_sd;
	new_sd->prior_subdir = insert_point;
	if (new_sd->next_subdir)
	    new_sd->next_subdir->prior_subdir = new_sd;
    }
    new_sd->first_email = NULL;
    new_sd->last_email = NULL;
    new_sd->count = 0;
    new_sd->subdir = strsav(subdir);
    new_sd->description = description;
    new_sd->a_date = date;
    if (set_base_url != NULL) {
		if (set_base_url[strlen(set_base_url) - 1] != '/')
	    trio_asprintf(&new_sd->rel_path_to_top, "%s/", set_base_url);
	else
	    new_sd->rel_path_to_top = strsav(set_base_url);
    }
    else {
	new_sd->rel_path_to_top = strsav("../");
	count = 0;
	for (p = subdir; *p; ++p)
	    count += (*p == '/');
		while (--count > 0) {
	    trio_asprintf(&p, "../%s", new_sd->rel_path_to_top);
	    free(new_sd->rel_path_to_top);
	    new_sd->rel_path_to_top = p;
	}
		if (set_latest_folder != NULL && set_folder_by_date != NULL && strchr(set_folder_by_date, '/')) {
	    static int warned = 0;
	    if (!warned) {
				fprintf(stderr, "Warning: the latest_folder option combined with " "a folder_by_date option that includes a '/' will produce " "some invalid links unless you use the base_url option!!.\n");
	      warned = 1;
	    }
	}
    }
    trio_asprintf(&new_sd->full_path, "%s%s", set_dir, subdir);
    if (!isdir(new_sd->full_path)) {
	checkdir(new_sd->full_path);	/* make subdir(s) if needed */
	if (set_latest_folder)
	    latest_folder_path = new_sd->subdir;
    }
    return new_sd;
}

/*
** And Now The List Addition Routines!
*/

void print_list(char *listname, struct hmlist *list)
{
    struct hmlist *tlist;
    int set = 0;
    int len = 0;
    int line = 0;

    /* Daniel remade Nov 23 1998. Use a single line as far as possible */

    for (tlist = list; tlist != NULL; tlist = tlist->next) {
	set++;
	if (!line++) {
	    printf("%s = ", listname);
	    len = strlen(listname) + 3;
	}
	else {
	    if (len + strlen(tlist->val) > 78) {
		printf("\n%s = ", listname);
		len = strlen(listname) + 3;
	    }
	    else {
		printf(",");
		len++;
	    }
	}
	printf("%s", tlist->val);
	len += strlen(tlist->val);
    }
    if (!set)
	printf("%s: %s\n", listname, lang[MSG_NO_ELEMENTS]);
    else
	printf("\n");
}

/*
** Is the requested element in the list  ?
*/

int inlist(struct hmlist *listname, char *wildcard)
{
    struct hmlist *tlist;

    for (tlist = listname; tlist != NULL; tlist = tlist->next) {
	/* wildcard checks enabled! */
	if (Match(wildcard, tlist->val))
	    return 1;
    }
    return 0;
}

/*
** What is the position of the requested element in the list  ?
*/

int inlist_pos(struct hmlist *listname, char *wildcard)
{
    struct hmlist *tlist;
    int i;

    for (i = 0, tlist = listname; tlist != NULL; i++, tlist = tlist->next) {
	/* wildcard checks enabled! */
	if (Match(wildcard, tlist->val))
	    return i;
    }
    return -1;
}

static int regex_index(struct hmlist *listname, int index)
{
	static int start[5] = { -1, -1, -1, -1, -1 };
    if (start[0] == -1) {
	struct hmlist *tlist;
	start[1] = start[0] = 0;
	for (tlist = set_filter_out; tlist != NULL; tlist = tlist->next)
	    ++start[1];
	start[2] = start[1];
	for (tlist = set_filter_require; tlist != NULL; tlist = tlist->next)
	    ++start[2];
	start[3] = start[2];
	for (tlist = set_filter_out_full_body; tlist != NULL; tlist = tlist->next)
	    ++start[3];
	start[4] = start[3];
	for (tlist = set_filter_require_full_body; tlist != NULL; tlist = tlist->next)
	    ++start[4];
    }
	if (index == -1)
		return start[4];
	if (listname == set_filter_out)
		return start[0] + index;
	if (listname == set_filter_require)
		return start[1] + index;
	if (listname == set_filter_out_full_body)
		return start[2] + index;
	if (listname == set_filter_require_full_body)
		return start[3] + index;
    return -1;
}

/*
** like inlist_pos, but does regex search
*/

int inlist_regex_pos(struct hmlist *listname, char *str)
{
    struct hmlist *tlist;
    int i;

    for (i = 0, tlist = listname; tlist != NULL; i++, tlist = tlist->next) {
#ifdef HAVE_PCRE
	int r;
	const char *errptr;
	int epos;
	int index = regex_index(listname, i);
	static pcre *p, **pcre_list;
	static pcre_extra *extra, **extra_list;
	if (!pcre_list) {
	    int n = regex_index(NULL, -1);
	    int i;
			pcre_list = (pcre **) emalloc(n * sizeof(pcre *));
			extra_list = (pcre_extra **) emalloc(n * sizeof(pcre_extra *));
	    for (i = 0; i < n; ++i) {
	        pcre_list[i] = NULL;
	        extra_list[i] = NULL;
	    }
	}
	if ((p = pcre_list[index]) == NULL) {
	    p = pcre_compile(tlist->val, 0, &errptr, &epos, NULL);
	    if (!p) {
		snprintf(errmsg, sizeof(errmsg), "Error at position %d of regular expression '%s': %s", epos, tlist->val, errptr);
		progerr(errmsg);
	    }
	    extra = pcre_study(p, 0, &errptr);
	    if (errptr) {
		snprintf(errmsg, sizeof(errmsg), "Error studying regular expression '%s': %s", tlist->val, errptr);
		progerr(errmsg);
	    }
	    pcre_list[index] = p;
	    extra_list[index] = extra;
	}
	extra = extra_list[index];
	r = pcre_exec(p, extra, str, strlen(str), 0, 0, NULL, 0);
	
	if (r >= 0)
	    return i;
#else
	static int warned = 0;
	if (set_showprogress && !warned) {
	    warned = 1;
	    printf("warning - regex not available\n");
	}
	if (strstr(tlist->val, str))
	    return i;
#endif
    }
    return -1;
}

/*
** Add element to the specified list
*/

struct hmlist *add_2_list(struct hmlist *listname, char *value)
{
    struct hmlist *tnode, *newnode;

    newnode = (struct hmlist *)emalloc(sizeof(struct hmlist));
    newnode->val = strsav(value);
    newnode->next = NULL;

    if (listname == NULL)
	listname = newnode;
    else {
	for (tnode = listname; tnode->next != NULL; tnode = tnode->next);
	tnode->next = newnode;
    }
    return listname;
}

struct hmlist *add_list(struct hmlist *listname, char *value)
{
    register char *cp;
    register char *valp;

    /*
     * This needs to support lists specified on a single line, such as
     *      inline = image/jpeg image/gif ...
     *      inline = image/jpeg,image/gif ...
     * as well as lists specified on multiple lines.
     */

    valp = value;
    cp = valp;

    while (*cp) {
	if (*cp == ' ' || *cp == ',') {
	    if (*valp != ' ' && *valp != ',') {
		*cp = '\0';
		listname = add_2_list(listname, valp);
	    }
	    valp = cp + 1;
	}
	cp++;
    }

    if (*valp)
	listname = add_2_list(listname, valp);

    return listname;
}
