/* 
** Author:  Daniel Stenberg <Daniel.Stenberg@haxx.nu>
** Version: 0.1
**
** This is a Base64 encoder as defined in RFC 2045. If the output is gonna be
** used in a mail body: "The encoded output stream must be represented in
** lines of no more than 76 characters each."
**
** CHANGES by Daniel Stenberg. May 11, 1998:
**
** - Encoded strings that ended with more than one = caused the decode 
**   function+ to generate 3 extra zero bytes at the end of the output.
*/
#include <stdlib.h>
#include <strings.h>
#include "base64.h"

/*
 * base64_alloc()
 *	Allocate a new base64 decoder
 */
struct base64_state *
base64_alloc(void)
{
    struct base64_state *st = malloc(sizeof(struct base64_state));
    if (!st) {
	return(0);
    }
    bzero(st, sizeof(struct base64_state));
    return(st);
}

/*
 * base64_decode()
 *
 * Accept "intext", place resulting output in null-terminated "out".
 *  "st" is our state, which will be updated and can carry state between
 *  calls.
 */
int
base64_decode(struct base64_state *st, const char *intext, char *out)
{
    char ignore, ch;
    int length;

    /* Ignore trailing garbage */
    if (st->b_endtext) {
	*out = '\0';
	return(0);
    }

    length = 0;
    while ((ch = *intext++)) {
	ignore = 0;
	if ((ch >= 'A') && (ch <= 'Z')) {
	    ch = ch - 'A';
	} else if ((ch >= 'a') && (ch <= 'z')) {
	    ch = ch - 'a' + 26;
	} else if ((ch >= '0') && (ch <= '9')) {
	    ch = ch - '0' + 52;
	} else if (ch == '+') {
	    ch = 62;
	} else if (ch == '=') {	/* end of text */
	    if (st->b_endtext) {
		break;
	    }
	    st->b_endtext = 1;
	    st->b_lindex--;
	    if (st->b_lindex < 0) {
		st->b_lindex = 3;
	    }
	} else if (ch == '/') {
	    ch = 63;
	} else if (st->b_endtext) {
	    break;
	} else {
	    ignore = 1;
	}

	if (!ignore) {
	    if (!st->b_endtext) {
		st->b_ibuf[st->b_lindex] = ch;

		st->b_lindex++;
		st->b_lindex &= 3;	/* use bit arithmetic instead of remainder */
	    }
	    if ((0 == st->b_lindex) || st->b_endtext) {

		st->b_obuf[0] = (st->b_ibuf[0] << 2) | ((st->b_ibuf[1] & 0x30) >> 4);
		st->b_obuf[1] =
		    ((st->b_ibuf[1] & 0x0F) << 4) | ((st->b_ibuf[2] & 0x3C) >> 2);
		st->b_obuf[2] = ((st->b_ibuf[2] & 0x03) << 6) | (st->b_ibuf[3] & 0x3F);

		switch (st->b_lindex) {
		case 1:
		    *out++ = st->b_obuf[0];
		    length += 1;
		    break;
		case 2:
		    *out++ = st->b_obuf[0];
		    *out++ = st->b_obuf[1];
		    length += 2;
		    break;
		default:
		    *out++ = st->b_obuf[0];
		    *out++ = st->b_obuf[1];
		    *out++ = st->b_obuf[2];
		    length += 3;
		    break;
		}
		bzero(st->b_ibuf, sizeof(st->b_ibuf));
	    }
	}
    }
    *out = 0;
    return(length);
}

/*
 * base64_free()
 *	Release storage
 */
void
base64_free(struct base64_state *st)
{
    free(st);
}

/*
 * base64Decode()
 *	Convenience wrapper when decoding a single line
 */
void
base64Decode(const char *intext, char *out, int *length)
{
    struct base64_state *st = base64_alloc();

    if (!st) {
	*length = 0;
	return;
    }
    *length = base64_decode(st, intext, out);
    base64_free(st);
}
