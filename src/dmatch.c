/*
  The shortest ever? 88 characters...

  m(char*s,char*t){return*t-'*'?*s?*t=='?'|*s==*t&&m(s+1,t+1):!*t:m(s,t+1)||*s&&m(s+1,t);}

  Written by Staffan Ulfberg <staffanu@tcs07.nada.kth.se>

  What follows below is a rewrite of the above into readable C and a small
  patch to prevent really deep recursiveness on patterns featuring a long
  sequence of '*'. Done by Daniel Stenberg <Daniel.Stenberg@haxx.nu>
 */

#include "dmatch.h"
#ifdef __LCC__
#include "../lcc/config.h"
#else
#include "../config.h"
#endif

#ifdef HAVE_CTYPE_H
#include <ctype.h>
#endif

#undef FALSE
#define FALSE 0
#undef TRUE
#define TRUE 1

char Match(char *string, char *pattern)
{
    for (; '*'^*pattern; ++pattern, ++string) {
	if (!*string)
	    return (!*pattern);
	if (toupper(*string) ^ toupper(*pattern) && '?'^*pattern)
	     return (FALSE);
    }

    /* two-line patch to prevent *too* much recursiveness: */

    while ('*' == pattern[1])
	pattern ++;

    do {
	if (Match(string, pattern +1))
	     return (TRUE);
    } while (*string++);

    return (FALSE);
}
