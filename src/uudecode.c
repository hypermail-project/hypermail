/*
** This file was written by Daniel Stenberg. 
** Parts of it were stolen from the BSD uudecode command.
*/

/*
** Copyright (c) 1983 Regents of the University of California.
** All rights reserved.
**
** Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions
** are met:
** 1. Redistributions of source code must retain the above copyright
**    notice, this list of conditions and the following disclaimer.
** 2. Redistributions in binary form must reproduce the above copyright
**    notice, this list of conditions and the following disclaimer in the
**    documentation and/or other materials provided with the distribution.
** 3. Neither the name of the University nor the names of its contributors
**    may be used to endorse or promote products derived from this software
**    without specific prior written permission.
**
** THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
** ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
** IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
** ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
** FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
** DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
** OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
** HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
** LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
** OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
** SUCH DAMAGE.
*/

#include "hypermail.h"
#include "setup.h"
#include "uudecode.h"

#ifdef HAVE_SYS_PARAM_H
#include <sys/param.h>
#endif

/* sco/isc */
#ifndef MAXPATHLEN
#ifdef PATH_MAX
#define MAXPATHLEN PATH_MAX
#else
#define MAXPATHLEN 1024
#endif
#endif

#define DEC(c) (((c) - ' ') & 077)	/* single character decode */
#define IS_DEC(c) ( (((c) - ' ') >= 0) &&  (((c) - ' ') <= 077 + 1) )

/*
** uudecode returns non-zero on error 
*/

int uudecode(FILE *input,	/* get file data from (if needed) */
	     char *iptr,	/* input string from where we are right now */
	     char *output,	/* write result to, must be at least 80 bytes */
	     int *length,	/* output size */
	     struct Push *init)
{				/* set to non-zero when initing */
    register int n;
    register char ch, *p;
    int mode;
    char buf[MAXPATHLEN] = { 0 };
    char scanfstring[MAXPATHLEN] = { 0 };
    int outlen = 0;

    if (length)
	*length = 0;		/* in case we abort early */
    
    if (init) {
	/* search for header line */
	/* AUDIT biege: BOF in buf! */
	sprintf(scanfstring, "begin %%o %%%us", sizeof(buf));
	while (2 != sscanf(iptr, scanfstring, &mode, buf)) {
	    if (!fgets(buf, MAXPATHLEN, input)) {
		return 2;
	    }
	    PushString(init, buf);
	    iptr = buf + set_ietf_mbox;
	}
#if 0
	fprintf(stderr, "FIRST %s", iptr);
#endif
	/* ignore file mode    */
	/* ignore file name too for now */
    }
    else {
	/* for each input line */
	p = iptr;

	/* first, just make sure this isn't the end line: */

		if (!strcmp(buf, "end") || !strcmp(buf, "end\n") || !strcmp(buf, "end\r\n")) {
	    /* muu->state = UU_HASENDED; */
	    if (length)
		*length = 0;
	    return 0;
	}

	n = DEC(*p);
	for (++p; n > 0; p += 4, n -= 3) {
	    if (n >= 3) {

		if (!(IS_DEC(*p) && IS_DEC(*(p + 1)) && IS_DEC(*(p + 2)) && IS_DEC(*(p + 3))))
		    return 33;

		ch = DEC(p[0]) << 2 | DEC(p[1]) >> 4;
		output[outlen++] = ch;
		ch = DEC(p[1]) << 4 | DEC(p[2]) >> 2;
		output[outlen++] = ch;
		ch = DEC(p[2]) << 6 | DEC(p[3]);
		output[outlen++] = ch;

	    }
	    else {
		if (n >= 1) {
		    if (!(IS_DEC(*p) && IS_DEC(*(p + 1))))
			return 34;
		    ch = DEC(p[0]) << 2 | DEC(p[1]) >> 4;
		    output[outlen++] = ch;
		}
		if (n >= 2) {
		    if (!(IS_DEC(*(p + 1)) && IS_DEC(*(p + 2))))
			return 35;
		    ch = DEC(p[1]) << 4 | DEC(p[2]) >> 2;
		    output[outlen++] = ch;
		}
		if (n >= 3) {
		    if (!(IS_DEC(*(p + 2)) && IS_DEC(*(p + 3))))
			return 36;
		    ch = DEC(p[2]) << 6 | DEC(p[3]);
		    output[outlen++] = ch;
		}
	    }
	}
    }
    if (output)
	output[outlen] = 0;	/* zero terminate    */

    if (length)
	*length = outlen;	/* send back size    */

    return 0;
}
