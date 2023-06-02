#ifndef _HYPERMAIL_STRUCT_H
#define _HYPERMAIL_STRUCT_H
/*
** Copyright (C) 1994, 1995 Enterprise Integration Technologies Corp.
**         VeriFone Inc./Hewlett-Packard. All Rights Reserved.
** Kevin Hughes, kev@kevcom.com 3/11/94
** Kent Landfield, kent@landfield.com 4/6/97
** Hypermail Project 1998-2023
** 
** This program and library is free software; you can redistribute it and/or 
** modify it under the terms of the GNU (Library) General Public License 
** as published by the Free Software Foundation; either version 3
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

/*
** struct.c functions
*/

unsigned hash(char *);
void reinit_structs(void);
void fill_email_dates(struct emailinfo *, char *, char *, char *, char *);

struct emailinfo *addhash(int, char *, char *, char *, char *, char *, char *,
			  char *, char *, char *, char *, struct body *);

int insert_in_lists(struct emailinfo *, const bool *, int);

struct emailinfo *hashreplylookup(int, char *, char *, int *);

struct emailinfo *hashmsgidlookup(char *, int *);
int hashreplynumlookup(int, char *, char *, int *);
struct emailinfo *hashlookupbymsgid(char *);
int insert_older_msgs(int);

struct body *hashnumlookup(int, struct emailinfo **);
struct emailinfo *neighborlookup(int, int);

struct body *addbody(struct body *, struct body **, char *, int);
struct body *append_body(struct body *, struct body **, struct body *, bool);
struct body *find_last_bp(struct body *);
struct body *quick_append_body(struct body *, struct body *);
void free_body(struct body *);

struct message_node *message_node_new(void);
void message_node_clear(struct message_node *, message_node_release_details_t);
void message_node_free(struct message_node *);
void message_node_delete_attachments(struct message_node *);
struct message_node *message_node_init(struct message_node *,
				       struct body *, struct body *,
				       char *, char *, char *);
struct message_node *message_node_mimetest(struct message_node *,
                                           struct body *, struct body *,
                                           char *, char *,
                                           char *,
                                           char *, char *,
                                           char *, char *,
                                           char *, char *,
                                           char, message_node_skip_t);
struct message_node *message_node_get_parent(struct message_node *);
void message_node_append_child (struct message_node *, struct message_node *);
void message_node_attachment_list (struct body **, struct body **, struct message_node *);
struct body *message_node_flatten(struct body **, struct message_node *);
void message_node_dump(struct message_node *);
char *message_node_get_charset(struct message_node *);
char *choose_charset(char *, char *);

void dump_body(struct body *);
void dump_mail(int, int);

struct reply *addreply(struct reply *, int, struct emailinfo *, int,
		       struct reply **);
struct reply *addreply2(struct reply *, struct emailinfo *, struct emailinfo *,
			int, struct reply **);
bool thread_can_be_deleted(struct emailinfo *);
int rmlastlines(struct body *);

struct emailsubdir *new_subdir(char *, struct emailsubdir *, char *, time_t);

struct printed *markasprinted(struct printed *, int);
void printed_free(struct printed *pl);
int wasprinted(struct printed *, int);


struct header *addheader(struct header *, struct emailinfo *, int, int);

struct boundary_stack *boundary_stack_push(struct boundary_stack *, char *);
struct boundary_stack *boundary_stack_pop(struct boundary_stack *);
int boundary_stack_free(struct boundary_stack *);
char *boundary_stack_has_id(struct boundary_stack *, const char *boundary_id);
bool boundary_stack_pop_to_id(struct boundary_stack **boundaryp, const char *boundary_id);
    
struct hm_stack *multipart_stack_push(struct hm_stack *, char *);
struct hm_stack *multipart_stack_pop(struct hm_stack *);
int multipart_stack_free(struct hm_stack *);
bool multipart_stack_has_type(const struct hm_stack *, char *);
bool multipart_stack_top_has_type(const struct hm_stack *, char *);
const char *multipart_stack_top_type(const struct hm_stack *);
    
struct hm_stack *hm_stack_push(struct hm_stack *, void *);
struct hm_stack *hm_stack_pop(struct hm_stack *, void **);

void print_list(char *, struct hmlist *);
int inlist(struct hmlist *, char *);
int inlist_pos(struct hmlist *, char *);
int inlist_regex_pos(struct hmlist *, char *);
struct hmlist *add_2_list(struct hmlist *, char *);
struct hmlist *add_list(struct hmlist *, char *);
void hmlist_free(struct hmlist *);

#endif /* _HYPERMAIL_STRUCT_H */
