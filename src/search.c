/*
** Copyright (C) 1997, Peter McCluskey (pcm@rahul.net)
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
#include <string.h>
#include <assert.h>
#include <ctype.h>
#include "proto.h"
#include "setup.h"
#include "struct.h"
#include "print.h"
#include "search.h"

static int bigram_count = 0;
static struct reply *replylist_tmp;

#if 1
typedef unsigned long BIGRAM_TYPE;
static const unsigned max_tokens = 400000; /* increase this if you have plenty of RAM */
#else				/* for systems with little RAM and archives less than 10 megs? */
typedef unsigned short BIGRAM_TYPE;
static const unsigned max_tokens = 0xfff0;
#endif

struct bigram_list {
    struct body *bp;
    short offset;
    struct bigram_list *next;
};

struct bigram_tree_entry {
    struct bigram_tree_entry *left;
    struct bigram_tree_entry *right;
    struct bigram_list list;
    BIGRAM_TYPE bigram1;
    BIGRAM_TYPE bigram2;
};
static struct bigram_tree_entry *bigram_tree = NULL;

#define MAXSEARCHTOKEN 26
struct search_text {
    struct search_text *left;
    struct search_text *right;
#ifdef BY_TOKEN_STRING
    char token[MAXSEARCHTOKEN];
#else
    int token_length;
    unsigned int token_crc32;
#endif				/* !BY_TOKEN_STRING */
    BIGRAM_TYPE itok;
#ifdef COUNT_TOKEN_FREQ
    int count;
#endif
};
static struct search_text *text_tree = NULL;
static BIGRAM_TYPE next_itoken = 1;

static int start_time;
static int tree_alloc = 0;

static void add_old_replies(void);
static void find_replyto_from_html(int num);

void set_alt_replylist(struct reply *r)
{
    replylist_tmp = r;
}

/* change the order of itok entries for more balanced tree */
static BIGRAM_TYPE reverse_bits(BIGRAM_TYPE i)
{
    BIGRAM_TYPE r = 0;
    int j;
    for (j = 0; j < 16; ++j)
	if (i & (1 << j))
	    r |= (1 << (16 - 1 - j));
    return r;
}

#ifdef BY_TOKEN_STRING
#define ENCODE_TOKEN(a)	encode_token(a)
#define COMPARE_TOKEN(a, b)	strcasecmp(a, (b)->token)
#else
#define ENCODE_TOKEN(a)	encode_token((a), strlen(a))
#define COMPARE_TOKEN(a, b)	token_digest_order(token_length, token_crc32, (b))

static int token_digest_order(const int token_length, const unsigned int token_crc32, const struct search_text *const p)
{
	return ((token_length > p->token_length) ? 1 : (token_length < p->token_length) ? -1 : (token_crc32 > p->token_crc32) ? 1 : (token_crc32 < p->token_crc32) ? -1 : 0);
}

/* crc32.c -- compute the CRC-32 of a data stream
 * Copyright (C) 1995 Mark Adler
 * For conditions of distribution and use, see copyright notice in zlib.h
 */
/* zlib.h is available in /usr/include on many systems. I also see a copy at:
http://sunsite.doc.ic.ac.uk/public/packages//NetBSD-current/src/lib/libz/zlib.h
*/

static unsigned int crc32_lower(register const unsigned char *buf,	/* pointer to bytes to pump through */
				int len)
{				/* number of bytes in buf[] */
    /* Run a set of bytes through the crc shift register.  If buf is a NULL
       pointer, then initialize the crc shift register contents instead.
       Return the current crc in either case. */
    register unsigned int crc = 0;	/* crc shift register */
#ifdef CRC32
#  undef CRC32
#endif
#define CRC32(c, b) (crc_table[((int)(c) ^ (b)) & 0xff] ^ ((c) >> 8))
#define DO1(buf)  crc = CRC32(crc, tolower(*buf++))

/* ========================================================================
 * Table of CRC-32's of all single-byte values (made by make_crc_table)
 */
    static const unsigned int crc_table[] = {
	0x00000000L, 0x77073096L, 0xee0e612cL, 0x990951baL, 0x076dc419L,
	0x706af48fL, 0xe963a535L, 0x9e6495a3L, 0x0edb8832L, 0x79dcb8a4L,
	0xe0d5e91eL, 0x97d2d988L, 0x09b64c2bL, 0x7eb17cbdL, 0xe7b82d07L,
	0x90bf1d91L, 0x1db71064L, 0x6ab020f2L, 0xf3b97148L, 0x84be41deL,
	0x1adad47dL, 0x6ddde4ebL, 0xf4d4b551L, 0x83d385c7L, 0x136c9856L,
	0x646ba8c0L, 0xfd62f97aL, 0x8a65c9ecL, 0x14015c4fL, 0x63066cd9L,
	0xfa0f3d63L, 0x8d080df5L, 0x3b6e20c8L, 0x4c69105eL, 0xd56041e4L,
	0xa2677172L, 0x3c03e4d1L, 0x4b04d447L, 0xd20d85fdL, 0xa50ab56bL,
	0x35b5a8faL, 0x42b2986cL, 0xdbbbc9d6L, 0xacbcf940L, 0x32d86ce3L,
	0x45df5c75L, 0xdcd60dcfL, 0xabd13d59L, 0x26d930acL, 0x51de003aL,
	0xc8d75180L, 0xbfd06116L, 0x21b4f4b5L, 0x56b3c423L, 0xcfba9599L,
	0xb8bda50fL, 0x2802b89eL, 0x5f058808L, 0xc60cd9b2L, 0xb10be924L,
	0x2f6f7c87L, 0x58684c11L, 0xc1611dabL, 0xb6662d3dL, 0x76dc4190L,
	0x01db7106L, 0x98d220bcL, 0xefd5102aL, 0x71b18589L, 0x06b6b51fL,
	0x9fbfe4a5L, 0xe8b8d433L, 0x7807c9a2L, 0x0f00f934L, 0x9609a88eL,
	0xe10e9818L, 0x7f6a0dbbL, 0x086d3d2dL, 0x91646c97L, 0xe6635c01L,
	0x6b6b51f4L, 0x1c6c6162L, 0x856530d8L, 0xf262004eL, 0x6c0695edL,
	0x1b01a57bL, 0x8208f4c1L, 0xf50fc457L, 0x65b0d9c6L, 0x12b7e950L,
	0x8bbeb8eaL, 0xfcb9887cL, 0x62dd1ddfL, 0x15da2d49L, 0x8cd37cf3L,
	0xfbd44c65L, 0x4db26158L, 0x3ab551ceL, 0xa3bc0074L, 0xd4bb30e2L,
	0x4adfa541L, 0x3dd895d7L, 0xa4d1c46dL, 0xd3d6f4fbL, 0x4369e96aL,
	0x346ed9fcL, 0xad678846L, 0xda60b8d0L, 0x44042d73L, 0x33031de5L,
	0xaa0a4c5fL, 0xdd0d7cc9L, 0x5005713cL, 0x270241aaL, 0xbe0b1010L,
	0xc90c2086L, 0x5768b525L, 0x206f85b3L, 0xb966d409L, 0xce61e49fL,
	0x5edef90eL, 0x29d9c998L, 0xb0d09822L, 0xc7d7a8b4L, 0x59b33d17L,
	0x2eb40d81L, 0xb7bd5c3bL, 0xc0ba6cadL, 0xedb88320L, 0x9abfb3b6L,
	0x03b6e20cL, 0x74b1d29aL, 0xead54739L, 0x9dd277afL, 0x04db2615L,
	0x73dc1683L, 0xe3630b12L, 0x94643b84L, 0x0d6d6a3eL, 0x7a6a5aa8L,
	0xe40ecf0bL, 0x9309ff9dL, 0x0a00ae27L, 0x7d079eb1L, 0xf00f9344L,
	0x8708a3d2L, 0x1e01f268L, 0x6906c2feL, 0xf762575dL, 0x806567cbL,
	0x196c3671L, 0x6e6b06e7L, 0xfed41b76L, 0x89d32be0L, 0x10da7a5aL,
	0x67dd4accL, 0xf9b9df6fL, 0x8ebeeff9L, 0x17b7be43L, 0x60b08ed5L,
	0xd6d6a3e8L, 0xa1d1937eL, 0x38d8c2c4L, 0x4fdff252L, 0xd1bb67f1L,
	0xa6bc5767L, 0x3fb506ddL, 0x48b2364bL, 0xd80d2bdaL, 0xaf0a1b4cL,
	0x36034af6L, 0x41047a60L, 0xdf60efc3L, 0xa867df55L, 0x316e8eefL,
	0x4669be79L, 0xcb61b38cL, 0xbc66831aL, 0x256fd2a0L, 0x5268e236L,
	0xcc0c7795L, 0xbb0b4703L, 0x220216b9L, 0x5505262fL, 0xc5ba3bbeL,
	0xb2bd0b28L, 0x2bb45a92L, 0x5cb36a04L, 0xc2d7ffa7L, 0xb5d0cf31L,
	0x2cd99e8bL, 0x5bdeae1dL, 0x9b64c2b0L, 0xec63f226L, 0x756aa39cL,
	0x026d930aL, 0x9c0906a9L, 0xeb0e363fL, 0x72076785L, 0x05005713L,
	0x95bf4a82L, 0xe2b87a14L, 0x7bb12baeL, 0x0cb61b38L, 0x92d28e9bL,
	0xe5d5be0dL, 0x7cdcefb7L, 0x0bdbdf21L, 0x86d3d2d4L, 0xf1d4e242L,
	0x68ddb3f8L, 0x1fda836eL, 0x81be16cdL, 0xf6b9265bL, 0x6fb077e1L,
	0x18b74777L, 0x88085ae6L, 0xff0f6a70L, 0x66063bcaL, 0x11010b5cL,
	0x8f659effL, 0xf862ae69L, 0x616bffd3L, 0x166ccf45L, 0xa00ae278L,
	0xd70dd2eeL, 0x4e048354L, 0x3903b3c2L, 0xa7672661L, 0xd06016f7L,
	0x4969474dL, 0x3e6e77dbL, 0xaed16a4aL, 0xd9d65adcL, 0x40df0b66L,
	0x37d83bf0L, 0xa9bcae53L, 0xdebb9ec5L, 0x47b2cf7fL, 0x30b5ffe9L,
	0xbdbdf21cL, 0xcabac28aL, 0x53b39330L, 0x24b4a3a6L, 0xbad03605L,
	0xcdd70693L, 0x54de5729L, 0x23d967bfL, 0xb3667a2eL, 0xc4614ab8L,
	0x5d681b02L, 0x2a6f2b94L, 0xb40bbe37L, 0xc30c8ea1L, 0x5a05df1bL,
	0x2d02ef8dL
    };

    if (buf == NULL)
	return 0L;

    crc = crc ^ 0xffffffffL;
    if (len)
	do {
	    DO1(buf);
	} while (--len);
    return crc ^ 0xffffffffL;	/* (instead of ~c for 64-bit machines) */
}
#endif				/* !BY_TOKEN_STRING */

static int encode_token(const char *token
#ifndef BY_TOKEN_STRING
			, const int token_length
#endif
    )
{
    struct search_text *p = text_tree;
#ifndef BY_TOKEN_STRING
    unsigned int token_crc32 = crc32_lower((const unsigned char *)token, token_length);
#endif
    int r;
    while (1) {
	if (!p) {
	    return 0;
	}
#ifdef BY_TOKEN_STRING
	r = COMPARE_TOKEN(token, p);
#else
	r = token_length - p->token_length;
	if (!r)
			r = ((token_crc32 > p->token_crc32) ? 1 : (token_crc32 < p->token_crc32) ? -1 : 0);
#endif				/* !BY_TOKEN_STRING */
	if (!r)
	    return p->itok;
	if (r < 0)
	    p = p->left;
	else
	    p = p->right;
    }
}

static int b_times_entered;
static int b_loops_done;

static int addb(const char *token, struct body *bp)
{
    struct search_text **pp = &text_tree;
    struct search_text *p = *pp;
    int r;
#ifndef BY_TOKEN_STRING
    int token_length = strlen(token);
    unsigned int token_crc32 = crc32_lower((const unsigned char *)token, token_length);
#endif
    static struct search_text *next_token = NULL;
    if (!text_tree) {
	next_token = (struct search_text *)malloc(max_tokens * sizeof(struct search_text));
	if (!next_token) {
	    snprintf(errmsg, sizeof(errmsg), "Couldn't allocate %d bytes of memory.", max_tokens * sizeof(struct search_text));
	    progerr(errmsg);
	}
	memset(next_token, 0, max_tokens * sizeof(struct search_text));
    }
    ++b_times_entered;
    while (1) {
	++b_loops_done;
	p = *pp;
	if (!p) {
	    *pp = p = next_token++;
	    if (next_token >= text_tree + max_tokens) {
		snprintf(errmsg, sizeof(errmsg), "Too many distinct tokens(%d)", max_tokens);
		progerr(errmsg);
	    }
	    p->left = p->right = NULL;
#ifdef BY_TOKEN_STRING
	    strncpy(p->token, token, MAXSEARCHTOKEN);
	    p->token[MAXSEARCHTOKEN - 1] = 0;
#else
	    p->token_length = token_length;
	    p->token_crc32 = token_crc32;
#endif				/* !BY_TOKEN_STRING */
	    p->itok = reverse_bits(next_itoken++);
#ifdef COUNT_TOKEN_FREQ
	    p->count = 1;
#endif
	    if (!next_itoken >= max_tokens) {
		snprintf(errmsg, sizeof(errmsg), "Internal error - too many distinct tokens.");
		progerr(errmsg);
	    }
	    return p->itok;
	}
	r = COMPARE_TOKEN(token, p);
	if (!r) {
#ifdef COUNT_TOKEN_FREQ
	    ++p->count;
#endif
	    return p->itok;
	}
	if (r < 0)
	    pp = &p->left;
	else
	    pp = &p->right;
    }
}

static int bi_times_entered;


static void add_bigram(BIGRAM_TYPE b1, BIGRAM_TYPE b2, struct body *bp, char *ptr)
{
    static struct bigram_tree_entry *next_bigram = NULL;
    struct bigram_tree_entry **pp = &bigram_tree;
    struct bigram_tree_entry *p = *pp;
    int r;
    if (!bigram_tree) {
	if (0)
	    printf("bigram_count %d\n\n", bigram_count);
		bigram_tree = (struct bigram_tree_entry *)malloc(bigram_count * sizeof(struct bigram_tree_entry));
	if (!bigram_tree) {
			snprintf(errmsg, sizeof(errmsg), "Couldn't allocate %d bytes of memory.", bigram_count * sizeof(struct bigram_tree_entry));
	    progerr(errmsg);
	}
		memset(bigram_tree, 0, bigram_count * sizeof(struct bigram_tree_entry));
	next_bigram = bigram_tree;
    }
    ++bi_times_entered;
    while (1) {
	p = *pp;
	if (!p) {
	    *pp = p = next_bigram++;
	    tree_alloc += sizeof(struct bigram_tree_entry);
	    p->bigram1 = b1;
	    p->bigram2 = b2;
	    p->left = p->right = NULL;
	    p->list.offset = ptr - bp->line;
	    p->list.bp = bp;
	    p->list.next = NULL;
	    return;
	}
	r = p->bigram1 - b1;
	if (!r && !(r = p->bigram2 - b2)) {
	    struct bigram_list *old = p->list.next;
	    p->list.next = (struct bigram_list *)next_bigram++;
	    p->list.next->next = old;
	    p->list.next->offset = p->list.offset;
	    p->list.next->bp = p->list.bp;
	    p->list.offset = ptr - bp->line;
	    p->list.bp = bp;
	    return;
	}
	if (r < 0)
	    pp = &p->left;
	else
	    pp = &p->right;
    }
}

static struct bigram_list *find_bigram(BIGRAM_TYPE b1, BIGRAM_TYPE b2)
{
    struct bigram_tree_entry *p = bigram_tree;
    int r;
    while (1) {
	if (!p) {
	    return NULL;
	}
	r = p->bigram1 - b1;
	if (!r)
	    r = p->bigram2 - b2;
	if (!r) {
	    return &p->list;
	}
	if (r < 0)
	    p = p->left;
	else
	    p = p->right;
    }
}

static struct body *next_body_pos(struct body *bp, char **ptr)
{
    ++*ptr;
    while (!**ptr) {
	bp = bp->next;
	if (!bp)
	    return NULL;
	*ptr = bp->line;
    }
    return bp;
}

struct body *tokenize_body(struct body *bp, char *token, char **ptr, int *bigram_index, int ignore)
{
    int i = 0;
    static const char *ignore_words[] = {
	"a",			/* 0 */
	"i",			/* 1 */
	NULL,			/* 2 */
	"of",			/* 3 */
	"is",			/* 4 */
	"in",			/* 5 */
	"it",			/* 6 */
	"to",			/* 7 */
	"be",			/* 8 */
	"or",			/* 9 */
	"on",			/* 10 */
	"at",			/* 11 */
	"by",			/* 12 */
	"as",			/* 13 */
	NULL,			/* 14 */
	"the",			/* 15 */
	"and",			/* 16 */
	"you",			/* 17 */
	"are",			/* 18 */
	"for",			/* 19 */
	NULL,			/* 20 */
	"that",			/* 21 */
	"with",			/* 22 */
	NULL
    };
#define MAX_IGNORE_LEN	4
    while (bp && **ptr && !isalnum(**ptr)) {
	bp = next_body_pos(bp, ptr);
	++*bigram_index;
    }
    if (!**ptr)
	return NULL;
    while (bp && isalnum(**ptr) && i < MAXLINE - 1) {
	token[i++] = **ptr;
	bp = next_body_pos(bp, ptr);
	++*bigram_index;
	if (bp && *ptr == bp->line)
	    break;		/* just passed end of line, treat as end of word */
	if (ignore && **ptr == '\'' && isalpha((*ptr)[1])) {
	    token[i++] = **ptr;	/* contraction - treat as 1 word */
	    bp = next_body_pos(bp, ptr);
	    ++*bigram_index;
	}
    }
    token[i] = 0;
    if (ignore) {
	switch (i) {
	case 1:
	    i = 0;
	    break;
	case 2:
	    i = 3;
	    break;
	case 3:
	    i = 15;
	    break;
	case 4:
	    i = 21;
	    break;
	default:
	    i = 23;
	    break;
	}
	for (; ignore_words[i]; ++i) {
	    if (!strcasecmp(token, ignore_words[i]))
		return tokenize_body(bp, token, ptr, bigram_index, ignore);
	}
    }
    return bp;
}

static void add_search_text(struct body *bp, int msgnum)
{
    char *ptr = bp->line;
    int bigram_index = 0;
    char token[MAXLINE];
    if (!start_time)
	start_time = time(NULL);
	while ((bp = tokenize_body(bp, token, &ptr, &bigram_index, TRUE)) != NULL) {
	addb(token, bp);
	++bigram_count;
    }
    if (0)
		printf("avg b %d avg bi %d (%d) msgnum %d %d allocated %d elapsed %ld\n", b_loops_done / b_times_entered, 0, bi_times_entered, msgnum, tree_alloc / 1024, time(NULL) - start_time, next_itoken);
}

static void add_bigrams(struct body *bp, int msgnum)
{
    int last_itok = 0;
    static int bigram_index = 0;
#if 0
    struct body *bp = hashnum_set_index(msgnum, bigram_index);
#endif
    char *ptr = bp->line;
    char token[MAXLINE];
    int itok;
    if (!start_time)
	start_time = time(NULL);
	while ((bp = tokenize_body(bp, token, &ptr, &bigram_index, TRUE)) != NULL) {
	itok = ENCODE_TOKEN(token);
	if (last_itok)
	    add_bigram(last_itok, itok, bp, ptr);
	bp->msgnum = msgnum;
	last_itok = itok;
    }
    if (0 && b_times_entered && bi_times_entered)
		printf("avg b %d msgnum %d %d allocated %d elapsed %ld\n", b_loops_done / b_times_entered, msgnum, tree_alloc / 1024, time(NULL) - start_time, next_itoken);
}

static void add_old_replies()
{
    struct reply *rp;
    struct reply *rp2;
    for (rp2 = replylist_tmp; rp2 != NULL; rp2 = rp2->next) {
	for (rp = replylist; rp != NULL; rp = rp->next) {	/* get rid of old guesses for where this links */
	    if (rp->frommsgnum == rp2->frommsgnum) {
		rp->msgnum = rp2->msgnum;
		rp->maybereply = 0;
		break;
	    }
	}
	if (!rp) {
#ifdef FASTREPLYCODE
	    struct emailinfo *email2;
	    hashnumlookup(rp2->frommsgnum, &email2);
			replylist = addreply2(replylist, email2, rp2->data, 0, &replylist_end);
#else
			replylist = addreply(replylist, rp2->frommsgnum, rp2->data, 0, &replylist_end);
#endif
	}
    }
}

static void find_replyto_from_html(int num)
{
    char *filename;
    char line[MAXLINE];
    FILE *fp;
    char *ptr;
    static const char *href_str = "<a href=\"";
    struct emailinfo *ep;
    if (!hashnumlookup(num, &ep))
	return;
    filename = articlehtmlfilename(ep);
    if ((fp = fopen(filename, "r")) != NULL) {
	while (fgets(line, MAXLINE, fp)) {
	    if ((ptr = strcasestr(line, lang[MSG_IN_REPLY_TO])) != NULL) {
		const char *ptr2 = strcasestr(ptr, href_str);
		if (ptr2 != NULL) {
		    int msgn = atoi(ptr2 + strlen(href_str));
#ifdef FASTREPLYCODE
		    struct emailinfo *email2;
		    if (hashnumlookup(msgn, &email2))
						replylist_tmp = addreply2(replylist_tmp, email2, ep, 0, NULL);
#else
		    replylist_tmp = addreply(replylist_tmp, msgn, ep, 0, NULL);
#endif
		}
	    }
	    if (!strcmp(line, "<!-- lnextthread=\"start\" -->\n"))
		break;
	}
	fclose(fp);
    }
    free(filename);
}

void analyze_headers(int max_num)
{
    int i;
    int min_search_msgnum = 0;
    int num = max_num;

	if (set_searchbackmsgnum > 0 && set_increment && num - set_searchbackmsgnum > min_search_msgnum)
	min_search_msgnum = num - set_searchbackmsgnum;

    for (i = 0; i < num; ++i)
	find_replyto_from_html(i);
    if (set_showprogress)
	printf("\nparsing bodies for later search.\n");
    for (i = min_search_msgnum; i < num; ++i) {
	struct emailinfo *ep;
	if (hashnumlookup(i, &ep) && ep->bodylist) {
	    add_search_text(ep->bodylist, i);
	}
    }
    for (i = min_search_msgnum; i < num; ++i) {
	struct emailinfo *ep;
	if (hashnumlookup(i, &ep) && ep->bodylist)
	    add_bigrams(ep->bodylist, i);
	if (set_showprogress)
	    printf("\b\b\b\b%4d articles.\n", i);
    }
    add_old_replies();
}

static void print_count(struct search_text *t)
{
    if (t->left)
	print_count(t->left);
#ifdef COUNT_TOKEN_FREQ
    printf("%d\t%s\n", t->count, t->token);
#endif
    if (t->right)
	print_count(t->right);
}

static int better_match(struct body *bp, const char *matched_string, const char *last_matched_string)
{
    int i;
    const char *ptr = bp->line;
    for (i = 0; 1; ++i) {
	int last_matches = (last_matched_string[i] == *ptr);
	int new_matches = (matched_string[i] == *ptr);
	if (0)
			printf("better_match? %d,%d @ %d\n%.70s\nVS:\n%.70s\n", new_matches, last_matches, i, last_matched_string + i, matched_string + i);
	if (!new_matches)
	    return 0;
	if (!last_matches)
	    return 1;
	if (!*++ptr) {
	    bp = bp->next;
	    if (!bp)
		break;
	    ptr = bp->line;
			if (last_matched_string[i] == '\n' || matched_string[i] == '\n')
				++i;
	}
    }
    return 0;
}

static void check_match(struct bigram_list *bigram, struct body *bp, char *ptr, int max_msgnum, String_Match * match_info, const char *match_start_ptr, const char *exact_line)
{
    int match_len = 1;
    int alloc_len = 0;
    int match_len_bytes;
    struct body *bp2 = bp;
    struct body *bp3 = NULL;
    char *ptr2 = ptr;
    const char *last_ptr = ptr;
    char *ptr3;
    char token2[MAXLINE];
    char token3[MAXLINE];
    int b2_index = 0;
    int b_index = 0;
    int msgnum = bigram->bp->msgnum;
    bp3 = bigram->bp;
    if (msgnum < max_msgnum && bp3) {
	ptr3 = bp3->line + bigram->offset;
	while (1) {
	    bp2 = tokenize_body(bp2, token2, &ptr2, &b2_index, TRUE);
	    bp3 = tokenize_body(bp3, token3, &ptr3, &b_index, TRUE);
	    if (0)
				printf("compare_match: %d %s, %20.20s\n", match_len, token3, ptr3);
	    if (!bp2 || !bp3)
		break;
	    if (ENCODE_TOKEN(token2) != ENCODE_TOKEN(token3))
		break;
	    ++match_len;
	    last_ptr = ptr2;
	}
	{
	    const char *p = match_start_ptr;
	    match_len_bytes = 0;
	    while (p != last_ptr) {
		++match_len_bytes;
		++p;
		if (!*p && p != last_ptr) {
		    bp = bp->next;
		    p = bp->line;
		}
	    }
	    ++match_len_bytes;
	}
	if (0)
			printf("compare_match: %d %d msgnum %d\n", match_len, match_info->match_len_tokens, msgnum);
		if (match_len > match_info->match_len_tokens || (match_len == match_info->match_len_tokens && better_match(bp, exact_line, match_info->last_matched_string))) {
	    match_info->match_len_tokens = match_len;
	    match_info->match_len_bytes = match_len_bytes;
	    match_info->msgnum = msgnum;
	    match_info->start_match = bigram->bp->line + bigram->offset;
	    match_info->stop_match = ptr3;
	    if (match_info->last_matched_string)
		free(match_info->last_matched_string);
	    match_len = strlen(bigram->bp->line);
	    alloc_len = match_len + 1000;
	    match_info->last_matched_string = (char *)emalloc(alloc_len);
	    strcpy(match_info->last_matched_string, bigram->bp->line);
	    if (!strchr(match_info->last_matched_string, '\n')) {
		strcat(match_info->last_matched_string + match_len, "\n");
		++match_len;
	    }
	    for (bp3 = bigram->bp->next; bp3; bp3 = bp3->next) {
		char *p = match_info->last_matched_string;
		int add_len = strlen(bp3->line);
				if (match_len + add_len + 2 > alloc_len) {
					alloc_len = 2 * (match_len + add_len + 2);
					match_info->last_matched_string = (char *)realloc(p, alloc_len);
		}
		strcat(match_info->last_matched_string + match_len, bp3->line);
		match_len += add_len;
				if (add_len > 0 && bp3->line[add_len - 1] != '\n')
					strcat(match_info->last_matched_string + match_len++, "\n");
	    }
	    if (0)
				printf("%d +++ %s; %s\nbp->line %s\n", bigram->bp->msgnum, match_info->last_matched_string, match_info->stop_match, bp->line);
	}
    }
}

/*
** Find the best match for a line from the bodies of prior messages  
*/

int search_for_quote(char *search_line, char *exact_line, int max_msgnum, String_Match * match_info)
{
    char *ptr = search_line;
    char token[MAXLINE];
    int last_itok = 0;
    int search_len = strlen(search_line);
	const char *stop_ptr = search_line + (search_len >= 80 ? 40 : (search_len + 1) / 2);
    static int count_tokens = 0;
    static int count_matches = 0;
    static int count_searched = 0;
    const char *match_start_ptr = ptr;
    const char *next_match_start_ptr;
    char *next_exact_ptr;
    int len;
    int dummy = 0;
    struct body *bp;
    struct body b;
    b.line = search_line;
    b.next = NULL;
    match_info->match_len_tokens = 0;
    match_info->match_len_bytes = 0;
    match_info->msgnum = -1;
    match_info->last_matched_string = NULL;
    bp = tokenize_body(&b, token, &ptr, &dummy, TRUE);
    if (!bp)
	return -1;
    ++count_searched;
    last_itok = ENCODE_TOKEN(token);
    next_match_start_ptr = ptr;
    next_exact_ptr = exact_line;

    while ((bp = tokenize_body(bp, token, &ptr, &dummy, TRUE)) != NULL) {
	int itok = ENCODE_TOKEN(token);
	struct bigram_list *bigram;
	bigram = find_bigram(last_itok, itok);
	if (!bigram)
			printf("Warning, internal inconsistency in search_for_quote:\n(%d,%d) %s %d best %d, msg %d %s || %s\n", last_itok, itok, token, dummy, match_info->match_len_tokens, max_msgnum, ptr, search_line);
	++count_tokens;
	while (bigram) {
	    ++count_matches;
			check_match(bigram, bp, ptr, max_msgnum, match_info, match_start_ptr, exact_line);
	    if (match_info->match_len_bytes == search_len)
		break;
	    bigram = bigram->next;
	}
		if (match_info->last_matched_string != NULL && strlen(match_info->last_matched_string) > search_len / 2)
	    break;
	if (ptr > stop_ptr)	/* very little chance of improving match */
	    break;		/* in 2nd half of string */
	last_itok = itok;
	match_start_ptr = next_match_start_ptr;
	next_match_start_ptr = ptr;

	exact_line = next_exact_ptr;
	tokenize_body(bp, token, &next_exact_ptr, &dummy, TRUE);
    }
    if (0)
		printf("%d times %d searches %d tokens %d matches tries %f\n", max_msgnum, count_searched, count_tokens, count_matches, (float)count_matches / count_tokens);
    len = match_info->match_len_bytes;
    if (max_msgnum == -1)
		printf("best_match_len %d (%d) len %d search_len %d %d; %s.\n", match_info->match_len_tokens, match_info->msgnum, len, search_len, match_info->match_len_bytes, match_info->last_matched_string);
	if (match_info->match_len_tokens > 1 && (len > search_len / 2 || len > 40)) {
	if ((ptr = strchr(match_info->last_matched_string, '\n')) != NULL)
	    *ptr = 0;		/* multi-line used for compares in check_match, but would screw up line-by-line compare outside */
	return TRUE;
    }
	if (match_info->match_len_tokens > 1 && (len > search_len / 2 || len > 40)
	&& len > match_info->match_len_bytes / 2)
	if (0)
			printf("#almost %d best_match_len %d len %d search_len %d %d.\n", max_msgnum, match_info->match_len_tokens, len, search_len, match_info->match_len_bytes);

    if (match_info->last_matched_string != NULL)
	free(match_info->last_matched_string);
    match_info->last_matched_string = NULL;
    match_info->msgnum = -1;
    return FALSE;
}
