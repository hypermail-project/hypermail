/*
** Copyright (C) 1997-2023 Hypermail Project
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
