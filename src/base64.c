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
**
** CHANGES by Andy Valencia - May 15 2023:
**
** - Preserve decoding state between calls to the
**   base64_stream_decode() function to take into account UA that
**   output intermediate base64 lines that are not always multiple of
**   four.
*/

#include "hypermail.h"
#include "base64.h"

/*
 * base64_decoder_state_new()
 *	Allocate a new base64 decoder state
 */
struct base64_decoder_state *base64_decoder_state_new(void)
{
    struct base64_decoder_state *st = (struct base64_decoder_state *)emalloc(sizeof(struct base64_decoder_state));
    if (!st) {
	return(0);
    }
    memset(st, 0, sizeof(struct base64_decoder_state));
    return(st);
}

/*
 * base64_decoder_state_free()
 *	Release storage
 */
void base64_decoder_state_free(struct base64_decoder_state *st)
{
    free(st);
}

/*
 * base64_decode_stream()
 *
 * Accept base64 "intext", 
 * place resulting decoded output in a null-terminated "out".
 *
 *  "st" is our state, which will be updated and can carry state between
 *  calls.
 */
int base64_decode_stream(struct base64_decoder_state *st, const char *intext, char *out)
{
    char ignore;
    char ch;
    int length;
    
    /* Ignore trailing garbage */
    if (st->endtext) {
	*out = '\0';
	return(0);
    }

    length = 0;
    while (*intext) {
	ch = *intext;
	ignore = FALSE;
        
	if ((ch >= 'A') && (ch <= 'Z')) {
	    ch = ch - 'A';
        } else if ((ch >= 'a') && (ch <= 'z')) {
	    ch = ch - 'a' + 26;
        } else if ((ch >= '0') && (ch <= '9')) {
	    ch = ch - '0' + 52;
        } else if (ch == '+') {
	    ch = 62;
        } else if (ch == '=') {	/* end of text */
	    if (st->endtext) {
		break;
            }
	    st->endtext = TRUE;
	    st->lindex--;
	    if (st->lindex < 0) {
		st->lindex = 3;
            }
	} else if (ch == '/') {
	    ch = 63;
        } else if (st->endtext) {
	    break;
        } else {
	    ignore = TRUE;
        }
        
	if (!ignore) {
	    if (!st->endtext) {
		st->ibuf[st->lindex] = ch;

		st->lindex++;
		st->lindex &= 3;	/* use bit arithmetic instead of remainder */
	    }
            if ((0 == st->lindex) || st->endtext) {

		st->obuf[0] = (st->ibuf[0] << 2) | ((st->ibuf[1] & 0x30) >> 4);
		st->obuf[1] =
		    ((st->ibuf[1] & 0x0F) << 4) | ((st->ibuf[2] & 0x3C) >> 2);
		st->obuf[2] = ((st->ibuf[2] & 0x03) << 6) | (st->ibuf[3] & 0x3F);
                
		switch (st->lindex) {
		case 1:
		    *out++ = st->obuf[0];
		    length += 1;
		    break;
		case 2:
		    *out++ = st->obuf[0];
		    *out++ = st->obuf[1];
		    length += 2;
		    break;
		default:
		    *out++ = st->obuf[0];
		    *out++ = st->obuf[1];
		    *out++ = st->obuf[2];
		    length += 3;
		    break;
		}
		memset(st->ibuf, 0, sizeof(st->ibuf));
	    }
	}
	intext++;
    }
    *out = 0;
    return (length);
}

/*
 * base64_decode_string()
 *	Convenience wrapper when decoding a single string
 */
int base64_decode_string(const char *intext, char *out)
{
    struct base64_decoder_state *st = base64_decoder_state_new();
    int length = 0;
    
    if (!st) {
	return 0;
    }
    length = base64_decode_stream(st, intext, out);
    base64_decoder_state_free(st);

    return (length);
}
