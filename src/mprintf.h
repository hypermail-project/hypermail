/*************************************************************************
 *
 * $Id$
 *
 * The contents of this file are subject to the Mozilla Public License
 * Version 1.0 (the "License"); you may not use this file except in
 * compliance with the License. You may obtain a copy of the License at
 * http://www.mozilla.org/MPL/ 
 *
 * Software distributed under the License is distributed on an "AS IS"
 * basis, WITHOUT WARRANTY OF ANY KIND, either express or implied. See
 * the License for the specific language governing rights and limitations
 * under the License. 
 *
 * The Original Code is Triacle.
 *
 * The Initial Developers of the Original Code are Bjorn Reese and
 * Daniel Stenberg.
 *
 * Portions created by Initial Developers are
 *
 *   Copyright (C) 1998 Bjorn Reese and Daniel Stenberg.
 *   All Rights Reserved. 
 *
 * Contributor(s):
 *
 *************************************************************************
 *
 * Preliminary documentation
 *
 * printf conversions:
 *
 *  conversion ::= '%%' | '%' [position] ( number | float | string )
 *  position ::= digits '$'
 *  number ::= [number-flags] ( 'd' | 'i' | 'o' | 'x' | 'X' | 'u')
 *  number-flags ::= 'h' | 'l' | 'L' ...
 *  float ::= [float-flags] ( 'f' | 'e' | 'E' | 'g' | 'G' )
 *  string ::= [string-flags] 's'
 *  string-flags ::= padding | '#'
 *  digits ::= (digit)+
 *  digit ::= 0-9
 *
 *  c
 *  p
 *  n
 *
 * qualifiers
 *
 *  -     : left adjustment
 *  +     : show sign
 *  SPACE : padding
 *  #     : alterative
 *  .     : precision
 *  *     : width
 *  0     : padding / size
 *  1-9   : size
 *  h     : short
 *  l     : long
 *  ll    : longlong
 *  L     : long double
 *  Z     : long / longlong
 *  q     : longlong
 *
 ************************************************************************/

#ifndef H_MPRINTF
#define H_MPRINTF

#ifdef HAVE_SYS_TYPES_H
#include <sys/types.h>
#endif

#ifdef HAVE_STDARG_H
#include <stdarg.h>
#endif

int mprintf(const char *, ...);
int mfprintf(FILE *, const char *, ...);
int msprintf(char *, const char *, ...);
int msnprintf(char *, size_t, const char *, ...);
int mvprintf(const char *, va_list);
int mvfprintf(FILE *, const char *, va_list);
int mvsprintf(char *, const char *, va_list);
int mvsnprintf(char *, size_t, const char *, va_list);
char *maprintf(const char *, ...);
char *mvaprintf(const char *, va_list);

#ifdef _MPRINTF_REPLACE
# define printf mprintf
# define fprintf mfprintf
# define sprintf msprintf
# define snprintf msnprintf
# define vprintf mvprintf
# define vfprintf mvfprintf
# define vsprintf mvsprintf
# define vsnprintf mvsnprintf
#endif

#endif				/* H_MPRINTF */
