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

#include "hypermail.h"
#include "base64.h"

void base64Decode(char *intext, char *out, int *length)
{
    unsigned char ibuf[4];
    unsigned char obuf[3];
    char ignore;
    char endtext = FALSE;
    char ch;
    int lindex = 0;
    *length = 0;

    memset(ibuf, 0, sizeof(ibuf));

    while (*intext) {
	ch = *intext;

	ignore = FALSE;
	if ((ch >= 'A') && (ch <= 'Z'))
	    ch = ch - 'A';
	else if ((ch >= 'a') && (ch <= 'z'))
	    ch = ch - 'a' + 26;
	else if ((ch >= '0') && (ch <= '9'))
	    ch = ch - '0' + 52;
	else if (ch == '+')
	    ch = 62;
	else if (ch == '=') {	/* end of text */
	    if (endtext)
		break;
	    endtext = TRUE;
	    lindex--;
	    if (lindex < 0)
		lindex = 3;
	}
	else if (ch == '/')
	    ch = 63;
	else if (endtext)
	    break;
	else
	    ignore = TRUE;

	if (!ignore) {
	    if (!endtext) {
		ibuf[lindex] = ch;

		lindex++;
		lindex &= 3;	/* use bit arithmetic instead of remainder */
	    }
	    if ((0 == lindex) || endtext) {

		obuf[0] = (ibuf[0] << 2) | ((ibuf[1] & 0x30) >> 4);
		obuf[1] =
		    ((ibuf[1] & 0x0F) << 4) | ((ibuf[2] & 0x3C) >> 2);
		obuf[2] = ((ibuf[2] & 0x03) << 6) | (ibuf[3] & 0x3F);

		switch (lindex) {
		case 1:
		    sprintf(out, "%c", obuf[0]);
		    out++;
		    (*length)++;
		    break;
		case 2:
		    sprintf(out, "%c%c", obuf[0], obuf[1]);
		    out += 2;
		    (*length) += 2;
		    break;
		default:
		    sprintf(out, "%c%c%c", obuf[0], obuf[1], obuf[2]);
		    out += 3;
		    (*length) += 3;
		    break;
		}
		memset(ibuf, 0, sizeof(ibuf));
	    }
	}
	intext++;
    }
    *out = 0;
}
