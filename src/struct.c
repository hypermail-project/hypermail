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

#define HAVE_PCRE2
#ifdef HAVE_PCRE2
#ifdef __LCC__
#include "../lcc/pcre2.h"
#else
#define PCRE2_CODE_UNIT_WIDTH 8
#include <pcre2.h>
#endif
#endif

struct body *hashnumlookup(int, struct emailinfo **);

typedef struct message_node_dump_stack {
    int parent_index;
    int sibling_index;
} message_node_dump_stack;

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
** Push a boundary-id string to the boundary stack. The top
** of the list represents the current "active" boundary.
*/
struct boundary_stack *boundary_stack_push(struct boundary_stack *bnd, char *boundary_id)
{
    struct boundary_stack *newnode = NULL;

    if (boundary_id) {
	newnode = (struct boundary_stack *)emalloc(sizeof(struct boundary_stack));
        memset(newnode, 0, sizeof(struct boundary_stack));
	newnode->boundary_id = strsav(boundary_id);
	newnode->next = NULL;
	newnode->prev = bnd;

	if (bnd)
	    bnd->next = newnode;

	bnd = newnode;
    }
    
    return bnd;		/* the new "active" boundary */
}

/* Removes and frees a boundary from the stack.
** Returns NULL when the stack is empty.
 */
struct boundary_stack *boundary_stack_pop(struct boundary_stack *bnd)
{
    struct boundary_stack *tmp_bnd;

    if (!bnd)
        return NULL;

    tmp_bnd = bnd;

    if (bnd->prev) {
        /* go back to the previous */
        bnd = bnd->prev;
        bnd->next = NULL;
    }
    else {
        /* this is the last node */
        bnd = NULL;
    }

    /* JK: MAKE SURE WE ARE FREEING ALL MEMORY ASSOCIATED TO BOUNDARY */
    /* free allocated memory */
    free(tmp_bnd->boundary_id);
#ifdef CHARSETSP
    if (tmp_bnd->prefered_content_charset) {
        free(tmp_bnd->prefered_content_charset);
    }
#endif /* CHARSETSP */
    free(tmp_bnd);

    return bnd;		/* the new "active" boundary */
}

/*
** Frees the memory allocated to a boundary structure/
** Returns the number of elements freed.
*/
int boundary_stack_free(struct boundary_stack *bnd)
{
    struct boundary_stack *cursor = bnd;
      
    struct boundary_stack *tmp;
  
    int counter = 0;
    
    if (bnd && bnd->next) {
        fprintf(stderr, "boundary_stack_free(): Error: boundary has a non-empty next element.\n"
                "BOUNDARY_ID : %s\n",bnd->boundary_id);
        progerr("aborting");
    }
    
    cursor = bnd;
    while (cursor) {
        tmp = cursor->prev;
        counter++;
        if (cursor->boundary_id) {
#ifdef DEBUG_PARSE
            fprintf (stderr, "boundary_stack_free(): freeing %s\n", cursor->boundary_id);
#endif
            free(cursor->boundary_id);
        }
        free (cursor);
        cursor = tmp;
    }
  
    return counter;
}

/*
** Returns a pointer to a boundary id if a given boundary stack has an
** element with boundary_id
** Returns NULL if not found
*/
char *boundary_stack_has_id(struct boundary_stack *boundaryp, const char *boundary_id)
{
    struct boundary_stack *tempnode;
    char *rv = NULL;
    char *stripped_boundary_id;
    
    if (!boundaryp || boundary_id == NULL || *boundary_id == '\0') {
        return NULL;
    }

    stripped_boundary_id = strip_boundary_id(boundary_id, 0);
    
    if (stripped_boundary_id != NULL && *stripped_boundary_id != '\0') {
        int tbl;
        
        for (tempnode = boundaryp; tempnode; tempnode = tempnode->prev) {
            tbl = strlen(tempnode->boundary_id);
            if (!strncmp(tempnode->boundary_id, stripped_boundary_id, tbl)
                && (stripped_boundary_id[tbl] == '\0'
                    || !strcmp(&stripped_boundary_id[tbl], "--"))) {
                rv = tempnode->boundary_id;
                break;
            }
        }
    }
    
    free(stripped_boundary_id);
    
    return rv;
}

/* If a boundary_id exists in the stack, but it's not at the top,
   pop all the boundaries in the stack to make it the top.
   Returns TRUE if boundary_id was found and is now on top
*/
bool boundary_stack_pop_to_id(struct boundary_stack **boundaryp, const char *boundary_id)
{
    struct boundary_stack *s = *boundaryp;
    char *stripped_boundary_id;;
    
    if (!boundary_stack_has_id(s, boundary_id)) {
        return FALSE;
    }

    stripped_boundary_id = strip_boundary_id(boundary_id, 0);
    
    while (s && strncasecmp(s->boundary_id, stripped_boundary_id, strlen(s->boundary_id))) {
        s = boundary_stack_pop(s);
    }
    
    *boundaryp = s;
    free(stripped_boundary_id);
    
    return TRUE;
}

/*
** multipart_stack stores the mime types of a message as part of the message
** parsing process. It overloads the hm_stack generic stack.
*/

/* pushes a new type to the stack.
** returns the current value of the stack */
struct hm_stack *multipart_stack_push(struct hm_stack *s, char *type)
{
    
    return hm_stack_push(s, (void *) strsav(type));
}

/* removes and frees the first element of the stack,
** returns the current stack or NULL if it's empty */
struct hm_stack *multipart_stack_pop(struct hm_stack *s)
{
    void *p;

    if (!s) {
        return NULL;
        
    }
    s = hm_stack_pop(s, &p);
    
    free((char *) p);
    
    return s;
}

/* frees all the memory associated with multipart_stack s */
int multipart_stack_free(struct hm_stack *s)
{
    int t = 0;

    while (s) {
        void *p;
        s = hm_stack_pop(s, &p);

        free( (char *) p);
        t++;
    }
    
#if DEBUG_PARSE
    fprintf (stderr, "multipart_stack_free: freed %d elements\n", t);
#endif
    return t;
}

/*
** returns TRUE if multipart stack s contains the value of the mime_type
*/
bool multipart_stack_has_type(const struct hm_stack *s, char *mime_type)
{
    const struct hm_stack *cursor;
    bool rv = FALSE;
    
    if (mime_type != NULL && *mime_type != '\0') {
        for (cursor = s; cursor; cursor = cursor->prev) {
            if (!strcasecmp ((char *) cursor->value, mime_type)) {
                rv = TRUE;
                break;
            }
        }
    }
  
    return rv;
}

/*
** returns TRUE if top of multipart stack s contains the value of the mime_type
*/
bool multipart_stack_top_has_type(const struct hm_stack *s, char *mime_type)
{
    bool rv = FALSE;

    if (mime_type != NULL && *mime_type != '\0' && s) {
        char *top_type = (char *) s->value;
        int tt_len;
        int mime_type_len = strlen(mime_type);
        
        if (top_type) {
            tt_len = strlen(top_type);
        }
            
        if (top_type && tt_len == mime_type_len
            && !strcasecmp(top_type, mime_type)) {
            rv = TRUE;
        }
    }
    return rv;
}

/* returns the string stored in the head of a multipart stack */
const char *multipart_stack_top_type(const struct hm_stack *s)
{
    return (s) ? (const char *) s->value : NULL;
}


#ifdef CHARSETSP
struct charset_stack *charsets(struct charset_stack *bnd, char *charset, char *charsetsave)
{
    struct charset_stack *newnode = NULL;

    if (charset || charsetsave) {
        newnode = (struct charset_stack *)emalloc(sizeof(struct charset_stack));
        newnode->charset = (charset) ? strsav(charset) : NULL;
        newnode->charsetsave = (charsetsave) ? strsav(charsetsave) : NULL;
    
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
            if (bnd->next->charset) {
                free(bnd->next->charset);
            }
            free(bnd->next->charsetsave);
	    free(bnd->next);
	    bnd->next = NULL;
	}
	else {
	    /* this is the last node */
            if (bnd->charset) {
                free(bnd->charset);
            }
            free(bnd->charsetsave);
	    free(bnd);
            bnd = NULL;
	}
    }
    return bnd;		/* the new "active" boundary */
}
#endif /* CHARSETSP */

#ifdef CHARSETSP
/* returns the first element in the stack */
struct charset_stack *charsets_head(struct charset_stack *bnd)
{
  struct charset_stack *cursor = bnd;

  if (cursor) {
      while (cursor->prev) {
          cursor = cursor->prev;
      }
  }
  return cursor;
}
#endif /* CHARSETSP */


#ifdef CHARSETSP
int free_charsets(struct charset_stack *cs)
{
    struct charset_stack *cursor = cs;
      
    struct charset_stack *tmp;
  
    int counter = 0;
    
    if (cs && cs->next) {
        fprintf (stderr, "free_charsets(): Error: boundary has a non-empty next element.\n charset = %s ; charsetsave = %s\n",
                 cs->charset, cs->charsetsave);
    }
    
    cursor = cs;
    while (cursor) {
        counter++;
        tmp = cursor->prev;
#ifdef DEBUG_PARSE
        fprintf (stderr, "free_charsets(): freeing charset = %s ; charsetsave = %s\n", cursor->charset, cursor->charsetsave);
#endif
        if (cursor->charset) {
            free (cursor->charset);
        }
        if (cursor->charsetsave) {
            free (cursor->charsetsave);
        }

        free (cursor);
        cursor = tmp;
    }

#if DEBUG_PARSE
    fprintf (stderr, "free_charset_stack: freed %d elements\n", counter);
#endif
    
    return counter;
}
#endif /* CHARSETSP */


/*
** Some basic generic stack functions to push / pull any kind of data
*/
struct hm_stack *hm_stack_push(struct hm_stack *s, void *value)
{
    struct hm_stack *new_element;
    
    new_element = (struct hm_stack *)emalloc(sizeof(struct hm_stack));
    new_element->value = value;
    if (s) {
        new_element->prev = s;
    } else {
        new_element->prev = NULL;
    }
    
    return new_element;
}
   
struct hm_stack *hm_stack_pop(struct hm_stack *s, void **value)
{
    struct hm_stack *tmp_element;
    
    if (!s) {
        return NULL;
    }

    *value = s->value;
    
    tmp_element = s;
    s = s->prev;
    free(tmp_element);

    return s;
}

static struct hm_stack *dump_index_stack_push(struct hm_stack *s, int parent_index, int sibling_index)
{
    message_node_dump_stack *p;

    p = emalloc(sizeof(message_node_dump_stack));
    p->parent_index = parent_index;
    p->sibling_index = sibling_index;
    
    return hm_stack_push(s, (void *)p);
}
   
static struct hm_stack *dump_index_stack_pop(struct hm_stack *s, int *parent_index, int *sibling_index)
{
    void *p;

    if (!s) {
        return NULL;
    }

    s = hm_stack_pop(s, &p);
    *parent_index = ((message_node_dump_stack *)p)->parent_index;
    *sibling_index = ((message_node_dump_stack *)p)->sibling_index;

    free(p);

    return s;
}


/*
** Creates a structure used to organize attachments and alternates
** when parsing a message
*/
struct message_node *message_node_new(void)
{
    struct message_node *newnode;
    
    newnode = (struct message_node *)emalloc(sizeof(struct message_node));
    memset(newnode, 0, sizeof(struct message_node));

    return newnode;
}

/* Releases the memory associated with the node fields.
** release_details gives more info on extra actions:
** MN_DELETE_ATTACHMENTS: deletes all attachments associated with this node
** MN_FREE_NODE deletes the node itself
** MN_KEEP_NODE release the memory associated with the node fields, but keeps
** the node.
*/
void message_node_clear(struct message_node *node,
                        message_node_release_details_t release_details)
{
    if (node->bp) {
        free_body(node->bp);
    }
    if (release_details & MN_DELETE_ATTACHMENTS) {
        if (node->bin_filename) {
            unlink(node->bin_filename);
        }
        if (node->meta_filename) {
            unlink (node->meta_filename);
        }
    }    
    if (node->content_type)
        free(node->content_type);
    if (node->boundary_part)
        free(node->boundary_part);
    if (node->boundary_type)
        free(node->boundary_type);
    if (node->charset)
        free(node->charset);
    if (node->charsetsave)
        free(node->charsetsave);
    if (node->bin_filename)
        free(node->bin_filename);
    if (node->meta_filename)
        free(node->meta_filename);
    if (node->html_link)
        free(node->html_link);
    if (node->comment_filename)
        free(node->comment_filename);
    if (release_details & MN_FREE_NODE)
        free(node);
    else
        memset(node, 0, sizeof(struct message_node));    
}

static void _message_node_free(struct message_node *root_message_node, bool delete_attachments)
{
    struct message_node *cursor = root_message_node;
    struct message_node *tmp;
    message_node_release_details_t extra_release_details = MN_KEEP_NODE;

    if (delete_attachments) {
        extra_release_details |= MN_DELETE_ATTACHMENTS;
    }
    
    while (cursor) {
        if (cursor->attachment_child) {
            cursor = cursor->attachment_child;
            
        } else if (cursor->attachment_next_sibling) {
            tmp = cursor;
            cursor = cursor->attachment_next_sibling;
            message_node_clear(tmp, MN_FREE_NODE | extra_release_details);
            
        } else if (cursor != root_message_node) {
            tmp = cursor;
            cursor = cursor->parent;
            cursor->attachment_child = NULL;
            message_node_clear(tmp, MN_FREE_NODE | extra_release_details);
        } else {
            message_node_clear(cursor, MN_KEEP_NODE | extra_release_details);            
            break;
        }
    }
}

/* frees a message node allocated memory */
void message_node_free(struct message_node *root_message_node)
{
    _message_node_free(root_message_node, FALSE);
}

/* deletes all attachments associated with a node and its children
** if the attachments are stored files, it will unlink those files.
** the function also also deletes all the children of this node */
void message_node_delete_attachments(struct message_node *node)
{
    struct message_node *tmp;

    tmp = node->parent;
    char *boundary_part = node->boundary_part;
    node->boundary_part = NULL;
    _message_node_free(node, TRUE);
    node->parent = tmp;
    node->boundary_part = boundary_part;
    node->skip = MN_SKIP_ALL;
}

/* 
** initializes a message_node structure.
*/
struct message_node *message_node_init(struct message_node *node,
				       struct body *bp, struct body *lp,
				       char *content_type, char *charset,
                                       char *boundary_part)
{
    node->bp = bp;
    node->lp = lp;
    trio_asprintf(&node->content_type, "%s", content_type);
    if (!strncasecmp(content_type, "multipart/", 10)) {
        trio_asprintf(&node->boundary_part, "%s", boundary_part);
    }
    if (charset) {
        trio_asprintf(&node->charset, "%s", charset);
    }
    
    return node;
}

/* creates a new node and links it to the precedent using some basic
   heuristics */
struct message_node *message_node_mimetest(struct message_node *current_message_node,
                                           struct body *bp, struct body *lp,
                                           char *charset, char *charsetsave,
                                           char *content_type,
                                           char *boundary_part, char *boundary_type,
                                           char *bin_filename,
                                           char *meta_filename,
                                           char *html_link,
                                           char *comment_filename,
                                           char is_attachment_rfc822,
                                           message_node_skip_t skip)
{
    struct message_node *tmp_node;
    char is_sibling;
    char is_child;

    /* if this is a text/plain attachment, remove all empty lines
       at the end of the message */
    if (!strncasecmp("text/plain", content_type, 10)) {
        while (rmlastlines(bp));
    }

    tmp_node = message_node_new();
    
    tmp_node->bp = bp;
    tmp_node->lp = lp;
    tmp_node->skip= skip;

    tmp_node->content_type = strsav(content_type);
    if (boundary_part) {
        tmp_node->boundary_part = strsav(boundary_part);
    }
    if ( !strncasecmp( content_type,
                       "multipart/", 10)) {

        if (boundary_type) {
            tmp_node->boundary_type = strsav(boundary_type);
        }
    }
    if (charset && *charset) {
        tmp_node->charset = strsav(charset);
    }
    if (charsetsave && *charsetsave) {
        tmp_node->charsetsave = strsav(charsetsave);
    }
    
    if (!current_message_node) {
        return tmp_node;
    }
    
    is_sibling = current_message_node->boundary_part
        && !strcasecmp(current_message_node->boundary_part,
                       boundary_part);
        
    is_child = !is_sibling
        && current_message_node->boundary_type
        && boundary_part
        && !strcasecmp(current_message_node->boundary_type,
                       boundary_part);

    if (!is_sibling && !is_child) {
        fprintf(stderr, "\nerror: cannot identify if attachment is sibling or child\n");
        fprintf(stderr, "error: current_message_node->boundary_type %s\n",
                current_message_node->boundary_type);
        fprintf(stderr, "error: boundary part: %s\n", boundary_part);
        fprintf(stderr, "error: Content-Type: %s\n", content_type);
        if (boundary_type) {
            fprintf(stderr, "error: boundary_id: %s\n", boundary_type);
        }
        progerr("aborting");
    }

    if (bin_filename && *bin_filename) {
        tmp_node->bin_filename = strsav(bin_filename);
    }

    if (meta_filename && *meta_filename) {
        tmp_node->meta_filename = strsav(meta_filename);
    }
    
    if (html_link && *html_link) {
        tmp_node->html_link = strsav(html_link);
    }
    if (comment_filename && *comment_filename) {
            tmp_node->comment_filename = strsav(comment_filename);
    }

    if (is_attachment_rfc822) {
        tmp_node->attachment_rfc822 = TRUE;
    }
        
    if (is_sibling) {
        current_message_node->attachment_next_sibling = tmp_node;
        tmp_node->parent = current_message_node->parent;
        
    } else if (is_child) {
        /*child */
        message_node_append_child (current_message_node, tmp_node);
        tmp_node->parent = current_message_node;
        
    } else {
        /* something went wrong, exit strategy */
        progerr("message_node broken\n");
    }

    return tmp_node;
}

/* this is a kludge work-around for the way we are handling message/rfc822 */
struct message_node *message_node_get_parent(struct message_node *current_message_node)
{
    struct message_node *rv;
    if (current_message_node->parent) {
        rv = current_message_node->parent;
    } else {
        rv = current_message_node;
    }
    
    return rv;
}

/*
** append a child to a parent node; if there are already children in the list,
** it will append it to the last one
*/
void message_node_append_child (struct message_node *parent, struct message_node *child)
{
    struct message_node *cursor;

    if (!parent->attachment_child) {
        parent->attachment_child = child;
    }
    else {
        cursor = parent->attachment_child;
        while (cursor->attachment_next_sibling) {
            cursor = cursor->attachment_next_sibling;
        }
        cursor->attachment_next_sibling = child;
    }
}

void message_node_attachment_list(struct body **attach_bp, struct body **attach_lp,
                                  struct message_node *node)
{
    struct body *bp;
    struct body *lp;
    char buffer[1024];

    if (!node->comment_filename) {
        return;
    }

    bp = *attach_bp;
    lp = *attach_lp;

    bp = addbody(bp, &lp, node->html_link,
                 BODY_HTMLIZED | BODY_ATTACHMENT_LINKS);
    
    trio_snprintf(buffer, sizeof(buffer),
                  "<!-- attachment=\"%.80s\" -->\n",
                  node->comment_filename);
    bp = addbody(bp, &lp, buffer,
                 BODY_HTMLIZED | BODY_ATTACHMENT_LINKS);
    
    *attach_bp = bp;
    *attach_lp = lp;
}

typedef struct attachment_list_stack {
    struct body *attachment_bp;
    struct body *attachment_lp;
} attachment_list_stack;

static struct hm_stack *stack_attachment_list_push(struct hm_stack *s,
                                                   struct body *attachment_bp,
                                                   struct body *attachment_lp)
{
    attachment_list_stack *p;

    p = emalloc(sizeof(attachment_list_stack));
    p->attachment_bp = attachment_bp;
    p->attachment_lp = attachment_lp;
    
    return hm_stack_push(s, (void *)p);
}
   
static struct hm_stack *stack_attachment_list_pop(struct hm_stack *s,
                                                  struct body **attachment_bp,
                                                  struct body **attachment_lp)
{
    void *p;

    if (!s) {
        return NULL;
    }

    s = hm_stack_pop(s, &p);
    *attachment_bp = ((attachment_list_stack *)p)->attachment_bp;
    *attachment_lp = ((attachment_list_stack *)p)->attachment_lp;

    free(p);

    return s;
}

/* 
** Accordingly to the type of node, its skip status and the
** skip status of its children, it returns an adjusted
** value of the node's skip value.
** This is is notably used when flattening nodes to avoid
** creating empty sections.
*/
static message_node_skip_t message_node_skip_adjust(struct message_node *root)
{
    bool rv = MN_SKIP_ALL;
    struct message_node *cursor = root;
    bool found_stored_attachment = FALSE;

    /* not a multipart node, return the current skip value */
    if (!cursor->boundary_type
        || strncasecmp(cursor->content_type, "multipart/", 10)) {
        return cursor->skip;
    }

    /* node is not marked as keep, return its current value */
    if (cursor->skip != MN_KEEP) {
        return cursor->skip;
    }

    /* if we get here, we have a multipart node marked as keep,
       but it has no children, we skip it */
    /* JK: @@ check if this won't fail for a root multipart that
       has no body (start and end boundary or no boundaries)  */
    if (!cursor->attachment_child) {
        return MN_SKIP_ALL;
    }
    
    cursor = cursor->attachment_child;
    
    while (cursor) {

        /* at least one of its children has MN_KEEP, we can stop */
        if (cursor->skip == MN_KEEP) {
            rv = MN_KEEP;
            break;
        }
        /* we don't know if we'll find a child with MN_KEEP,
           record we found a stored attachment */
        else if (cursor->skip == MN_SKIP_STORED_ATTACHMENT) {
            found_stored_attachment = TRUE;
        }

        /* go down the tree */
        
        if (cursor->attachment_child) {
            cursor = cursor->attachment_child;
          
        } else if (cursor->attachment_next_sibling) {
            cursor = cursor->attachment_next_sibling;

        } else if (cursor->parent == root) {
            break;
          
        } else {
            cursor = cursor->parent->attachment_next_sibling;
        }
    }

    /* if we didn't find any child with MN_KEEP but we found
       at least one child with MN_SKIP_STORED_ATTACHMENT,
       return MN_SKIP_BUT_KEE_CHILDREN */
    if (found_stored_attachment && rv != MN_KEEP) {
        rv = MN_SKIP_BUT_KEEP_CHILDREN;
    }
    
    return rv;
}

/*
** flattens a message_node structure into a linked struct body list
** returns the pointer to the root of the body list as well as a pointer to the
** last element added.
*/ 
struct body *message_node_flatten(struct body **flattened_lp, struct message_node *root_message_node)
{
  struct message_node *cursor;
  struct body *cursor_lp;

  struct body *attachment_bp = NULL;
  struct body *attachment_lp = NULL;
  
  int rfc822_flag = FALSE; /* used for message/rfc822 attachments */
  struct hm_stack *s = NULL;  /* used for storing the list of attachments for message/rfc822
                                      attachments */
      
  struct body *root_flattened_bp = NULL;
  
  /* used to adjust the skip vlaue of a node so we can
     skip nodes that are of type multipart/ but that have no
     children, e.g. if all its children were stored attachments */
  message_node_skip_t skip_adjust;;

  cursor = root_message_node;

  root_flattened_bp = cursor->bp;
  cursor->bp = NULL;
  cursor_lp = find_last_bp (root_flattened_bp);

  /*
  ** we are only loooking at the first child but we should really check
  ** if there is at all even one descendant of this node that
  ** has something to display
  */

  skip_adjust = message_node_skip_adjust(cursor);
  cursor->skip = skip_adjust;
  
  if (root_message_node->attachment_child
      && root_message_node->skip != MN_SKIP_ALL) {
      
      while (cursor) {
        
          /*
          ** print current node unless we are skipping it
          */

          skip_adjust = message_node_skip_adjust(cursor);
          cursor->skip = skip_adjust;
     
          if (! (cursor->skip & MN_SKIP_ALL)) {
              
              /* if we have an rfc822 attachment, we're going to more or less merge it into
                 its parent, the message/rfc822 declaration */
              rfc822_flag = cursor->attachment_rfc822;
              
              /* push the current list of attachments and start with a new list */
              if (rfc822_flag) {
                  s = stack_attachment_list_push(s, attachment_bp, attachment_lp);
                  attachment_bp = attachment_lp = NULL;
              }
              
              /* is node a parent ? */
              if (cursor->attachment_child) {
                  if (cursor != root_message_node && !(cursor->skip & MN_SKIP_BUT_KEEP_CHILDREN)) {
                      /* add parent section and move cursor to child */
                      root_flattened_bp = addbody(root_flattened_bp, &cursor_lp,
                                                  NULL,
                                                  BODY_ATTACHMENT | BODY_ATTACHMENT_START
                                                  | ((rfc822_flag) ? BODY_ATTACHMENT_RFC822 : 0));
                      cursor_lp = quick_append_body(cursor_lp, cursor->bp);
                      cursor->bp = NULL;
                  }
                  cursor = cursor->attachment_child;
                  continue;
              }
          
              /*
              ** add current node, encapsulated by a section 
              */
              
              /* if this node is linked to a stored attachment, extract that info and put
                 it in a list we'll link at the end of the flattened message */
              if (cursor->skip & MN_SKIP_STORED_ATTACHMENT) {
                  message_node_attachment_list(&attachment_bp, &attachment_lp, cursor);
              } else {
                  root_flattened_bp = addbody(root_flattened_bp, &cursor_lp,
                                              NULL,
                                              BODY_ATTACHMENT | BODY_ATTACHMENT_START
                                              | ((rfc822_flag) ? BODY_ATTACHMENT_RFC822 : 0));
                  
                  cursor_lp = quick_append_body(cursor_lp, cursor->bp);
                  cursor->bp = NULL;

                  /* add the list of stored attachments associated
                   * with this message/rfc822 part */
                  if (rfc822_flag && attachment_bp) {
                      root_flattened_bp = addbody(root_flattened_bp, &cursor_lp,
                                                  NULL,
                                                  BODY_ATTACHMENT_LINKS_START);
                      cursor_lp = quick_append_body(cursor_lp, attachment_bp);
                      root_flattened_bp = addbody(root_flattened_bp, &cursor_lp,
                                                  NULL,
                                                  BODY_ATTACHMENT_LINKS_END);
                  }
                  
                  root_flattened_bp = addbody(root_flattened_bp, &cursor_lp,
                                              NULL,
                                              BODY_ATTACHMENT | BODY_ATTACHMENT_END
                                              | ((rfc822_flag) ? BODY_ATTACHMENT_RFC822 : 0));
                  /* pop the stored list of attachments */
                  if (rfc822_flag) {
                      s = stack_attachment_list_pop(s, &attachment_bp, &attachment_lp);
                  }
              }
          }
          
          /* go to next child if it exists */
          if (cursor->attachment_next_sibling) {
              cursor = cursor->attachment_next_sibling;
              continue;
          }
          
          /* find next child from parent, closing sections for each parent */
          while (cursor && cursor != root_message_node) {
              cursor = cursor->parent;
              if (cursor != root_message_node && !(cursor->skip & MN_SKIP_BUT_KEEP_CHILDREN)) {
                  /* we also want to know this is the end of the rfc822 attachment so we
                   can make markup decisions later on */
                  rfc822_flag = cursor->attachment_rfc822;

                  /* add the list of stored attachments associated
                   * with this message/rfc822 part */
                  if (rfc822_flag && attachment_bp) {
                      root_flattened_bp = addbody(root_flattened_bp, &cursor_lp,
                                                  NULL,
                                                  BODY_ATTACHMENT_LINKS_START);
                      cursor_lp = quick_append_body(cursor_lp, attachment_bp);
                      root_flattened_bp = addbody(root_flattened_bp, &cursor_lp,
                                                  NULL,
                                                  BODY_ATTACHMENT_LINKS_END);
                  }
                  
                  root_flattened_bp = addbody(root_flattened_bp, &cursor_lp,
                                              NULL,
                                              BODY_ATTACHMENT | BODY_ATTACHMENT_END
                                              | ((rfc822_flag) ? BODY_ATTACHMENT_RFC822 : 0));

                  /* pop the stored list of attachments */
                  if (rfc822_flag) {
                      s = stack_attachment_list_pop(s, &attachment_bp, &attachment_lp);
                  }
              }
              if (cursor->attachment_next_sibling) {
                  cursor = cursor->attachment_next_sibling;
                  break;
              }
          }
          if (cursor == root_message_node) {
              break;
          }
      }
  }

  /* add attachment list here */
  if (attachment_bp) {
      root_flattened_bp = addbody(root_flattened_bp, &cursor_lp,
                                  NULL,
                                  BODY_ATTACHMENT_LINKS_START);
      cursor_lp = quick_append_body(cursor_lp, attachment_bp);
      root_flattened_bp = addbody(root_flattened_bp, &cursor_lp,
                                  NULL,
                                  BODY_ATTACHMENT_LINKS_END);
  }
  
  *flattened_lp = cursor_lp;
  return root_flattened_bp;

}

/*
** This function is used for debugging purposes.
**
** Dumps a message_node structure and exits
** The content of each node is converted to UTF-8 before being output
*/ 
void message_node_dump(struct message_node *root_message_node)
{
  struct message_node *cursor;
  struct body *bp;
  int message_node_counter = 0;
  int parent_index = 0;
  int sibling_index = 1;
  char *node_separator = "------------------------";

  struct hm_stack *s = NULL;

#ifdef HAVE_PCRE2
  pcre2_code *re;
  PCRE2_SPTR pattern = (PCRE2_SPTR) "^.{0,40}"; /* extract the first 40 characters */
  int enumber;
  PCRE2_SIZE epos;
  PCRE2_SIZE *ovector;
  pcre2_match_data *match_data;
  int rc;
  int max_bytes;
  
  re = pcre2_compile(pattern, PCRE2_ZERO_TERMINATED, PCRE2_UTF|PCRE2_UCP, &enumber, &epos, NULL);

  if (re == NULL) {
      PCRE2_UCHAR buffer[256];
      pcre2_get_error_message(enumber, buffer, sizeof(buffer));
      printf("PCRE2 compilation failed at offset %d: %s\n", (int)epos,
             buffer);
      progerr("message_node_dump: PCRE2 compilation failed");
  }

  match_data = pcre2_match_data_create_from_pattern(re, NULL);
#endif /* HAVE_PRCRE2 */
  
  cursor = root_message_node;

/* the dump index guide is:
   node-count node-parent-index node-sibling-index
   node-counter increments by one for each node
   node-parent-index gives the node number of the parent, 
   -- if node has no parent
   node-sibling-count increments by one for each immediate sibling
*/
  
  while (cursor) {
      int convert_to_utf8;
      int header_detected;
      char *charset;
      
      message_node_counter++;

      if (cursor->charset && cursor->charsetsave) {
          if (!strncasecmp (cursor->charset, "us-ascii", 5)) {
              charset = cursor->charsetsave;
          } else {
              charset = cursor->charset;
          }
      } else if (cursor->charset) {
          charset = cursor->charset;
      } else if (cursor->charsetsave) {
          charset = cursor->charsetsave;
      } else {
          charset = NULL;
      }
          
      if (!charset
          || !strncasecmp (charset, "UTF-8", 5)
          || !strncasecmp (charset, "us-ascii", 5)) {
          convert_to_utf8 = FALSE;
      } else {
          convert_to_utf8 = TRUE;
      }

      if (message_node_counter == 1) {
          printf("[%02d -- %02d] %s\n", message_node_counter, sibling_index, node_separator);
      } else {
          printf("[%02d %02d %02d] %s\n", message_node_counter, parent_index, sibling_index,
                 node_separator);
      }

      if (cursor->boundary_part) {
          printf("boundary part:\t%s\n", cursor->boundary_part);
      }
      if (cursor->content_type) {
          printf("content-type:\t%s\n", cursor->content_type);
      }
      if (cursor->boundary_type) {
          printf("c-t boundary:\t%s\n", cursor->boundary_type);
      }
      if (cursor->charset) {
          printf("c-t charset:\t%s\n", cursor->charset);
      }
      if (cursor->charsetsave) {
          printf("header charset:\t%s\n", cursor->charsetsave);
      }      
      if (cursor->alternative) {
          printf("flag:       \talternative\n");
      }
      if (cursor->bin_filename) {
          printf("bin_filename:\t%s\n", cursor->bin_filename);
      }
      if (cursor->meta_filename) {
          printf("meta_filename:\t%s\n", cursor->meta_filename);
      }
      if (cursor->html_link) {
          printf("html_link:\t%s\n", strchomp(cursor->html_link));
      }
      if (cursor->comment_filename) {
          printf("comment_fname:\t%s\n", strchomp(cursor->comment_filename));
      }
      if (cursor->attachment_rfc822) {
          printf("flag:       \tmessage/rfc822\n");
      }

      if (cursor->skip) {
          char *value;
          switch (cursor->skip) {
          case MN_SKIP_BUT_KEEP_CHILDREN :
              value = "skip but keep children";
              break;
          case MN_SKIP_STORED_ATTACHMENT :
              value = "skip, stored attachment";
              break;
          default: value = "skip";
              break;
          }
          printf("skip flag:\t%s\n", value, cursor->skip);
      }
      printf("\n");

      if (set_debug_level == DEBUG_DUMP_ATT_VERBOSE) {
          bp = cursor->bp;
          header_detected = FALSE;
          while (bp) {
              char *line;
              size_t len;
              
              line = bp->line;
              if (!header_detected
                  && (line[0] == '\n' || (line[0] == '\r' && line[1] == '\n'))) {
                  header_detected = TRUE;
              }
              
              /* don't start the convertion until we hit the first header */
              if (header_detected && convert_to_utf8) {
                  line = i18n_convstring(bp->line, charset, "UTF-8", &len);
              }
#ifdef HAVE_PCRE2
              /* need to be able to only output 40 utf-8 chars here */
              rc = pcre2_match(
                  re,            /* the compiled pattern */
                  (PCRE2_SPTR) line, /* the subject string */
                  strlen(line),  /* the length of the subject */
                  0,             /* start at offset 0 in the subject */
                  0,             /* default options */
                  match_data,    /* block for storing the result */
                  NULL);
              
              if (rc <= 0) {
                  printf("%s", line);
              } else {
                  ovector = pcre2_get_ovector_pointer(match_data);
                  max_bytes=(int)ovector[1];
                  /* skip any \n */
                  if (max_bytes > 0 && line[max_bytes - 1] == '\n') {
                      max_bytes--;
                  }
                  line[max_bytes]='\0';
                  printf("%s\n", line);
              }
#else
              printf("%40s", line);
#endif
              
              if (convert_to_utf8) {
                  free(line);
              }
          
              bp = bp->next;
          }
      }

      if (cursor->attachment_child) {
          cursor = cursor->attachment_child;
          s = dump_index_stack_push(s, parent_index, sibling_index);
          parent_index = message_node_counter;
          sibling_index = 1;
          
      } else if (cursor->attachment_next_sibling) {
          sibling_index++;
          cursor = cursor->attachment_next_sibling;

      } else if (!cursor->parent) {
          break;
          
      } else {
          cursor = cursor->parent->attachment_next_sibling;
          s = dump_index_stack_pop(s, &parent_index, &sibling_index);
          sibling_index++;
      }
  }

#ifdef HAVE_PCRE2
  pcre2_match_data_free(match_data);   /* Release memory used for the match */
  pcre2_code_free(re);                 /* data and the compiled pattern. */
#endif
  
  printf ("Total message-nodes: %d\n", message_node_counter);
}

/* Uses simple heuristics to choose between two charsets
**
** if charset1 and charset 2 are both declared, returns
** charset1 unless it has value "us-ascii".
** If only one of them is declared, it returns that one.
** If none is declared, returns NULL
**
** caller must not free the returned value.
*/
char *choose_charset(char *charset1, char *charset2)
{
    char *rv;
    
    if (charset1 && *charset1 && charset2 && *charset2) {
        if (!strncasecmp(charset1, "us-ascii", 8))
            rv = charset2;
        else
            rv = charset1;
    } else {
        rv = (charset1 && *charset1) ? charset1 : charset2;
    }

    return rv;
}
  
/*
** Goes thru the message_node tree and selects the best charset
** message using these heuristics:
**
** use charset from Content-Type header if found
** otherwise, use the first MIME encoded charset from a header
** If the message is a multipart one, then give priority
** to the charset found in that part, if available
** If not, use the one from the root
**
** If no charset is found, it will use the default one
** stored in the set_default_charset config variable.
** 
** returns a copy of the selected charset; caller must
** free this string.
**
*/
char *message_node_get_charset(struct message_node *root)
{
  struct message_node *cursor;
  char *charset;
  
  char *root_ct_charset = NULL;
  char *root_mime_charset = NULL;
  char *mpart_ct_charset = NULL;
  char *mpart_mime_charset = NULL;
  int message_node_counter = 0;
  
  cursor = root;

  if (!cursor) {
      return NULL;
  }
  
  if (cursor->charset) {
      root_ct_charset = cursor->charset;
  }
  if (cursor->charsetsave) {
      root_mime_charset=cursor->charsetsave;
  }
  
  while (cursor) {

      message_node_counter++;
      
      if (!cursor->skip
          && cursor->bp
          && (cursor->charset || cursor->charsetsave)) {
          if (cursor->charset) {
              mpart_ct_charset = cursor->charset;
          }
          if (cursor->charsetsave) {
              mpart_mime_charset = cursor->charsetsave;
          }
          if (set_debug_level) {
              printf("message_node_prefered_charset: getting charsets from node %d\n",
                     message_node_counter);
          }
          break;
      }

      if (cursor->attachment_child) {
          cursor = cursor->attachment_child;
          
      } else if (cursor->attachment_next_sibling) {
          cursor = cursor->attachment_next_sibling;

      } else if (!cursor->parent) {
          break;
          
      } else {
          cursor = cursor->parent->attachment_next_sibling;
      }
  }

  /* heuristics, finetune as needed */
  {
      /* ct_charset has priority over mime_charset, unless it is us-ascii */
      
      char *root_charset = choose_charset(root_ct_charset, root_mime_charset);
      char *mpart_charset = choose_charset(mpart_ct_charset, mpart_mime_charset);

      /* giving priority to mpart, as it contains the main of the body, while
         root headers can be converted in principle to any charset*/
      if (mpart_charset) {
          charset = choose_charset(mpart_charset, root_charset);
      } else {
          charset = root_charset;
      }
  }
  
  if (!charset) {
      charset = set_default_charset;
  }
  
  if (set_debug_level) {
      printf("message_node_prefered_charset: mpart_ct_charset %s\n", mpart_ct_charset);
      printf("message_node_prefered_charset: mpart_mime_charset %s\n", mpart_mime_charset);      
      printf("message_node_prefered_charset: root_ct_charset %s\n", root_ct_charset);
      printf("message_node_prefered_charset: root_mime_charset %s\n", root_mime_charset);
      printf("message_node_prefered_charset: selected charset %s\n", charset);
  }

  return strsav(charset);
}

/*
** Add a line to a linked list that makes up an article's body.
*/

struct body *addbody(struct body *bp, struct body **lp,	/* points to the last pointer */
		     char *line, int flags)
{
    struct body *tempnode;
    struct body *newnode = NULL;
    char *unstuffed_line = line;
    int free_unstuffed_line = 0;

    /* delete both space stuffing and quotes where applicable for f=f */
    if (flags & BODY_DEL_SSQ) {
        if (flags & BODY_CONTINUE) {
            /* delete all quote levels, we're reusing those of the precedent line */
            while (*unstuffed_line == '>') {
                unstuffed_line++;
            }
        }
        /* deleting space-stuffing at beginning of line */
        if (unstuffed_line[0] == ' ') {
            unstuffed_line++;
        } 
        else if (unstuffed_line[0] == '>') {
            char *delsp_line = rfc3676_delsp_quotes(unstuffed_line);
            if (delsp_line) {
                unstuffed_line = delsp_line;
                free_unstuffed_line = 1;
            }
        }
    }
    
    if (!(flags & BODY_CONTINUE)) {
	newnode = (struct body *)emalloc(sizeof(struct body));
	memset(newnode, 0, sizeof(struct body));
        
        if (!(flags & BODY_ATTACHMENT || flags & BODY_ATTACHMENT_LINKS)
            || (flags & BODY_ATTACHMENT_RFC822)) {
            newnode->line = spamify(strsav(unstuffed_line));
        } else if (flags & BODY_ATTACHMENT_LINKS && line && *line) {
            newnode->line = strsav(line);
        }
    
        if (flags & BODY_ATTACHMENT) {
            newnode->attachment_flags = flags;
        }
        if (flags &  BODY_ATTACHMENT_LINKS_START || flags & BODY_ATTACHMENT_LINKS_END) {
            newnode->attachment_links_flags = flags;            
        }
	newnode->html = (flags & BODY_HTMLIZED) ? 1 : 0;
	newnode->header = (flags & BODY_HEADER) ? 1 : 0;
	newnode->attached = (flags & BODY_ATTACHED) ? 1 : 0;
	newnode->attachment_links = (flags & BODY_ATTACHMENT_LINKS) ? 1 : 0;
	newnode->attachment_rfc822 = (flags & BODY_ATTACHMENT_RFC822) ? 1 : 0;
        newnode->format_flowed = (flags & BODY_FORMAT_FLOWED) ? 1 : 0;
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
	    newlen = strlen(tempnode->line) + strlen(unstuffed_line) + 1;

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
		strcat(newbuf, unstuffed_line);

		/* point out the new buffer instead */
		tempnode->line = newbuf;
	    }
	}
	else {
	    tempnode->next = newnode;
	    *lp = newnode;
	}
    }

    if (free_unstuffed_line) {
        free(unstuffed_line);
    }
    
    return bp;
}

/*
** Remove the last empty lines, if any, from an article body's linked list.
** returns 1 if it removed any lines, 0 otherwise.
*/

int rmlastlines(struct body *bp)
{
    struct body *tempnode;

    if (NULL == bp) {
        return 0;
    }
    
    for (tempnode = bp; tempnode->next != NULL && tempnode->next->line
             && (tempnode->next->line)[0] != '\0'; tempnode = tempnode->next);
    if ((tempnode->line)[0] == '\n') {
        (tempnode->line)[0] = '\0';
        return 1;
    }
    return 0;
}

/* 
** copy bp on to the end of origbp, deallocate bp
*/
struct body *append_body(struct body *origbp, struct body **origlp, struct body *bp, bool free_body)
{
    while (bp) {
	struct body *next;
        int flags;

        flags = (bp->html ? BODY_HTMLIZED : 0)
            | (bp->header ? BODY_HEADER : 0)
            | (bp->attached ? BODY_ATTACHED : 0)
            | (bp->attachment_links ? BODY_ATTACHMENT_LINKS : 0)
            | (bp->attachment_rfc822 ? BODY_ATTACHMENT_RFC822 : 0)
            | (bp->format_flowed ? BODY_FORMAT_FLOWED : 0)
            | bp->attachment_flags
            | bp->attachment_links_flags;

        origbp = addbody(origbp, origlp, bp->line, flags);
	next = bp->next;
        
	if (free_body) {
            free(bp->line);
            free(bp);
	}
	bp = next;
    }
    return origbp;
}

/*
** finds the last element of a linked body structure 
*/
struct body *find_last_bp(struct body *bp)
{
    if (bp) {
        while (bp->next) {
            bp = bp->next;
        }
    }
    return bp;
}

/*
**  appends new_bp to target_bp and returns a pointer to the last element in new_bp
*/
struct body *quick_append_body(struct body *target_bp, struct body *new_bp)
{
    if (target_bp) {
        target_bp = find_last_bp (target_bp);
        target_bp->next = new_bp;
    }
    
    return find_last_bp (new_bp);
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

/* The following two are two functions used when debugging the code */
void dump_body(struct body *bp)
{
  struct body *cursor = bp;
  int lc = 0;
  while (cursor != NULL) {
      lc++;

      printf("%02d -----\n", lc);
      if (cursor->attachment_flags) {
          if (cursor->attachment_flags & BODY_ATTACHMENT_START)
              printf ("ATTACHMENT_START\n");
          if (cursor->attachment_flags & BODY_ATTACHMENT_END)
              printf ("ATTACHMENT_END\n");
          
          if (cursor->attachment_flags & BODY_ATTACHMENT_RFC822)
              printf ("ATTACHMENT_RFC822\n");
          
          if (cursor->attachment_flags & BODY_ATTACHMENT_LINKS_START)
              printf ("ATTACHMENT_LINKS_START\n");
          if (cursor->attachment_flags & BODY_ATTACHMENT_LINKS_END)
              printf ("ATTACHMENT_LINKS_END\n");
      }
      if (cursor->line)
          printf("%40s", cursor->line);
      /*
      printf("header %d\n", cursor->header);
      printf("parsed header %d\n", cursor->header);
      printf("attached %d\n", cursor->attached);
      printf("attachment_links %d\n", cursor->attachment_links);
      printf("attachment_rfc822 %d\n", cursor->attachment_links);
      printf("demimed %d\n", cursor->demimed);
      printf("format_flowed %d\n", cursor->format_flowed);
      printf("msgnum %d\n", cursor->msgnum);
      */
      cursor = cursor->next;
  }
}

void dump_mail(int startnum, int maxnum)
{
    int num;
    struct emailinfo *ep;
    struct body *bp;
    
    for (num = startnum; num < maxnum; num++) {
        
	if ((bp = hashnumlookup(num, &ep)) == NULL) {
	    continue;
	}

        printf("------------------------------\n");
        printf("msgnum %03d\n", ep->msgnum);
        printf("is deleted %d\n", ep->is_deleted);
        printf("from %s\n", ep->name);
        printf("emailaddr %s\n", ep->emailaddr);
        printf("fromdatestr %s\n", ep->fromdatestr);
        printf("msgid %s\n", ep->msgid);
        printf("unre_subject %40s\n", ep->unre_subject);
        printf("inreplyto %s\n", ep->inreplyto);
        printf("charset %s\n", ep->charset);
        
        /* @@print flags */
        dump_body(bp);
    }

    progerr("exiting");
    
} /* dump_mail */

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
** Returns true if a thread can be deleted,
** that is, it has no children or all of
** its children are also marked as deleted
*/

/* how deep threads can be */
#define MAX_THREAD_LEVELS 200

bool thread_can_be_deleted(struct emailinfo *data)
{
    struct reply *stack[MAX_THREAD_LEVELS + 1];
    int stack_index = 0;
    
    struct reply *rp;
    struct emailinfo *ep = data;
    
    bool rv = TRUE;

    if (!ep || !ep->is_deleted) {
        return FALSE;
    }
    
    rp = ep->replylist;
    while (rp) {
        
        ep = rp->data;
        
        if (!rp->data || (rp->msgnum != ep->msgnum)) {
            rp = rp->next;
            continue;
        }

        if (!ep->is_deleted) {
            rv = FALSE;
            break;
        }

        if (ep->replylist) {
            stack_index++;
            /* beyond MAXSTACK, we stop exploring and just return true */
            if (stack_index > MAX_THREAD_LEVELS) {
                rv = FALSE;
                break;
            }            
            stack[stack_index] = rp;
            rp = ep->replylist;
            
        } else {
            if (stack_index > 0) {
                rp = stack[stack_index--];
            }
            rp = rp->next;
        }
    }

  return rv;
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
#ifdef HAVE_PCRE2
	int rc;
        int enumber;
	PCRE2_SIZE epos;
	int index = regex_index(listname, i);
	static pcre2_code *p, **pcre_list;
	static pcre2_match_data *match_data, **match_data_list;
	if (!pcre_list) {
	    int n = regex_index(NULL, -1);
	    int i;
            pcre_list = (pcre2_code **) emalloc(n * sizeof(pcre2_code *));
			match_data_list = (pcre2_match_data **) emalloc(n * sizeof(pcre2_match_data *));
	    for (i = 0; i < n; ++i) {
	        pcre_list[i] = NULL;
	        match_data_list[i] = NULL;
	    }
	}
	if ((p = pcre_list[index]) == NULL) {
            p = pcre2_compile((PCRE2_SPTR8) tlist->val, PCRE2_ZERO_TERMINATED, 0, &enumber, &epos, NULL);
	    if (!p) {
                PCRE2_UCHAR buffer[256];
                pcre2_get_error_message(enumber, buffer, sizeof(buffer));
		trio_snprintf(errmsg, sizeof(errmsg), "Error at position %d of regular expression '%s': %s", epos, tlist->val, buffer);
		progerr(errmsg);
	    }
	    match_data = pcre2_match_data_create_from_pattern(p, NULL);
	    pcre_list[index] = p;
	    match_data_list[index] = match_data;
	}
	match_data = match_data_list[index];
	rc = pcre2_match(p, (PCRE2_SPTR8) str, strlen(str), 0, 0, match_data, 0);
	
	if (rc >= 0)
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
