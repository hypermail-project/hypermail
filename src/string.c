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

/*
** All the nasty string functions live here.
*/

#include <iconv.h>
#include <errno.h>
#include <sys/stat.h>

#include "hypermail.h"
#include "setup.h"
#include "parse.h"
#include "uconvert.h"

#ifdef HAVE_STRING_H
#include <string.h>
#endif


/*
** email address obfuscation
*/
char *obfuscate_email_address(char *address)
{
  char sbuf[10];
  struct Push buf;
  int at_mailbox=1;
  char *sentinel;
  
  if (!set_email_address_obfuscation){
    return address;
  }
  
  sentinel=strlen(address)+address;
  
  INIT_PUSH(buf);
  while (address<sentinel){
    if (*address=='@'){
      PushString(&buf,"&#x40;");
      at_mailbox=0;
    }else{
      if(!at_mailbox){
	sprintf(sbuf,"&#%04d;",*address);
	PushString(&buf,sbuf);
      }else{
	sbuf[0]=*address;
	sbuf[1]=0x0;
	PushString(&buf,sbuf);
      }
    }
    address++;
  }
  RETURN_PUSH(buf);
}

char *unobfuscate_email_address(char *address){

#define uea_sbufsize 6
  int flag=0; /* 1==obfuscated string */
  int on_error=0; /* 1==something was happen */
  int on_hex=0; /* 1==base 16 */
  int at_mailbox=1; /* 1==mailbox part */
  char *sentinel;
  
  char sbuf[uea_sbufsize+1];
  int scount=0;
  struct Push buf;
  long c;
  
  INIT_PUSH(buf);
  
  sentinel=strlen(address)+address;
  
  
  while (address<sentinel){
    switch (*address){
    case '&':
      if(strncmp(address,"&#x40;",6)==0){
	PushByte(&buf, '@');
	address+=5;
	flag=0;
	on_error=0;
	on_hex=0;
	at_mailbox=0;
	break;
      }
      if (at_mailbox){
	PushByte(&buf, '&');
	break;
      }
      if (flag==0){
	flag=1;
      }else{
	/* broken string */
	on_error=1;
      }
      break;
    case ';':
      if (at_mailbox){
	PushByte(&buf, ';');
	break;
      }
      if (flag==1){
	flag=0;
	sbuf[scount]=0;
	if(on_hex==1){
	  c=strtol(sbuf,NULL,16);
	}else{
	  c=strtol(sbuf,NULL,10);
	}
	if (on_error==0){
	  PushByte(&buf, (unsigned char)c); /* should be ASCII */
	} /* simply discard error */
	scount=0;
	on_error=0;
      }else{
	/* broken string */
	on_error=1;
      }
      break;
    case '#':
      if (at_mailbox){
	PushByte(&buf, '#');
	break;
      }
      if (flag==1){
	if (*(address+1)=='x'){
	  on_hex=1;
	  address++;
	}else{
	  on_hex=0;
	}
	break;
      }else{
	PushByte(&buf, *address);
      }
      break;
    default:
      if (at_mailbox || flag==0){
	PushByte(&buf, *address);
      }else{
	sbuf[scount++]=*address;
	if (scount>=uea_sbufsize){
	  /* broken string */
	  scount=0;
	  on_error=1;
	}
      }
    }
    address++;
  }
  RETURN_PUSH(buf);
}

/* I18N hack */
#ifdef HAVE_ICONV_H
#include <iconv.h>
#endif

#ifdef HAVE_ICONV
struct i18n_alt_charset_table {
  char *alt;
  char *formal;
};

struct i18n_alt_charset_table i18n_charsettable[] = {
  /* UTF-8 (RFC2279) */
  {"UNICODE-1-1-UTF-8","UTF-8"},
  {"UNICODE-2-0-UTF-8","UTF-8"},
  {"x-unicode-2-0-utf-8","UTF-8"},
  {"ANSI_X3.4-1968","US-ASCII"},

  /* US-ASCII (RFC1345) */
  {"iso-ir-6","US-ASCII"},
  {"ANSI_X3.4-1986","US-ASCII"},
  {"ISO_646.irv:1991","US-ASCII"},
  {"ASCII","US-ASCII"},
  {"ISO646-US","US-ASCII"},
  {"us","US-ASCII"},
  {"IBM367","US-ASCII"},
  {"cp367","US-ASCII"},
  {"csASCII","US-ASCII"},
  {"iso-ir-6us","US-ASCII"},

  /* Arabic (RFC1324) */
  {"ISO_8859-6:1987","ISO-8859-6"},
  {"iso-ir-127","ISO-8859-6"},
  {"ECMA-114","ISO-8859-6"},
  {"arabic","ISO-8859-6"},
  {"csISOLatinArabic","ISO-8859-6"},
  {"8859_6","ISO-8859-6"},
  {"ISO8859_6","ISO-8859-6"},
  {"iso8859-6","ISO-8859-6"},
  {"windows-1256","CP1256"},

  /* Korean (RFC1557) */
  {"KS_C_5601-1987","CP949"},
  {"KS_C_5601-1989","CP949"},

  /* Thai */
  {"windows-874","CP874"},

  /* Chinese */
  {"x-gbk","GB18030"},

  /* Cyrillic */
  {"x-mac-cyrillic","MacCyrillic"},
  {"ibm866","CP866"}
};
#define I18N_CHARSET_TABLE_SIZE (sizeof(i18n_charsettable)/sizeof(struct i18n_alt_charset_table))

char *i18n_canonicalize_charset(char *cs){

  int x=0;

  while(x<I18N_CHARSET_TABLE_SIZE){
    if(strcasecmp(cs,i18n_charsettable[x].alt)==0){
      return i18n_charsettable[x].formal;
    }
    x++;
  }
  return cs;
}


char *i18n_convstring(char *string, char *fromcharset, char *tocharset, size_t *len){

  size_t origlen,strleft,bufleft;
  size_t origbuflen;
  char *convbuf,*origconvbuf;
  iconv_t iconvfd;
  size_t ret;
  int error;

  if (string){
    strleft=origlen=strlen(string);
  }else{
    strleft=origlen=0;
  }
  origbuflen = origlen*7;
  origconvbuf=convbuf=malloc(origbuflen+1);
  memset(origconvbuf,0,origbuflen);
  bufleft=origlen*7;

  if (!set_i18n || strcasecmp(fromcharset,tocharset)==0){
    /* we don't need to convert string here */
    *len=origlen;
    memcpy(origconvbuf,string,origlen);
    origconvbuf[origlen]=0x0;
    return origconvbuf;
  }

  iconvfd=iconv_open(i18n_canonicalize_charset(tocharset),i18n_canonicalize_charset(fromcharset));
  if(iconvfd==(iconv_t)(-1)){
    if(set_showprogress){
      if(errno==EINVAL){
        printf("I18N: unsupported encoding: charset=(from=%s, to=%s).\n",fromcharset,tocharset);
      }else{
        printf("I18N: libiconv open error.\n");
      }
    }
    origlen=snprintf(origconvbuf,origbuflen, "(unknown charset) %s",string);
    origconvbuf[origlen]=0x0;
    *len=origlen;
    return origconvbuf;
  }
  errno=0;
  ret=iconv(iconvfd, &string, &strleft, &convbuf, &bufleft);
  iconv_close(iconvfd);

  if (ret==(size_t)-1){
    error = 1;
    switch (errno){
    case E2BIG:
      if(set_showprogress){
	printf("I18N: buffer overflow.\n");
      }
      origlen=snprintf(origconvbuf, origbuflen,"(buffer overflow) %s",string);
      error = 1;
      break;
    case EILSEQ:
      if(set_showprogress){
	printf("I18N: invalid multibyte sequence, from %s to %s: %s.\n",fromcharset,tocharset,string);
      }
      origlen=snprintf(origconvbuf, origbuflen,"(wrong string) %s",string);
      error = 1;
      break;
    case EINVAL:
      if(set_showprogress){
	printf("I18N: incomplete multibyte sequence, from %s to %s: %s.\n",fromcharset,tocharset,string);
      }
      origlen=snprintf(origconvbuf, origbuflen,"(wrong string) %s",string);
      error = 1;
      break;
    }
  } else {
    error = 0;
  }

  if (error) {
    origconvbuf[origlen]=0x0;
    *len=origlen;
  } else {
    /* hmm... do we really need to do this? (daigo) */
    if (strncasecmp(tocharset,"ISO-2022-JP",11)==0){
      *len=origlen*7-bufleft;
      *(origconvbuf+*len)=0x1b;
      *(origconvbuf+*len+1)=0x28;
      *(origconvbuf+*len+2)=0x42;
      *len+=3;
    }else{
      *len=origlen*7-bufleft;
    }
    
    *(origconvbuf+*len)=0x0;
  }

  return origconvbuf;
}


/* convert text from UTF-8 to numeric reference */
/*  escape: 1==on 0==off */
char *i18n_utf2numref(char *instr,int escape){

  char *ucs,*headofucs;
  size_t len;
  struct Push buff;
  char strbuf[10];

  INIT_PUSH(buff);

  if (!set_i18n){
    PushString(&buff,instr);
    RETURN_PUSH(buff);
  }

  headofucs=ucs=i18n_convstring(instr, "UTF-8", "UCS-2BE", &len);

  unsigned int p;
  int i = (int) len;
  for(; i > 0; i-=2){
    p=(unsigned char)*ucs*256+(unsigned char)*(ucs+1);
    if (p<128){
      /* keep ASCII characters human readable */
      if (escape){
        switch (p){
        case '<':
          PushString(&buff, "&#0060;");
          break;
        case '>':
          PushString(&buff, "&#0062;");
          break;
        case '&':
          PushString(&buff, "&#0038;");
          break;
        case '\"':
          PushString(&buff, "&#0034;");
          break;
        case '\'':
          PushString(&buff, "&#0039;");
          break;
        default:
          PushByte(&buff,p);
        }
      }else{
        PushByte(&buff,p);
      }
    }else{
      snprintf(strbuf,10,"&#%04d;",p);
      PushString(&buff,strbuf);
    }
    ucs+=2;
  }
  free(headofucs);
  RETURN_PUSH(buff);
}


char *i18n_numref2utf(char *string){

  int status,len;
  unsigned short int utf;
  unsigned char convbuf[5],*utfstr,*headofutfstr;

  len=strlen(string);
  utfstr=headofutfstr=malloc(len);
  memset(headofutfstr,0,len);

  status=0;

  while (*string!=0x0){
    switch (*string){
    case '&':
      string++;
      status++;
      break;
    case '#':
      string++;
      status++;
      break;
    case ';':
      string++;
      status=0;
      break;
    default:
      if(status==2){
        convbuf[0]=*string++;
        convbuf[1]=*string++;
        convbuf[2]=*string++;
        convbuf[3]=*string++;
        convbuf[4]=0x0;
        utf=strtol(&convbuf[0], NULL,10);
        if(utf<256){
          *utfstr=(unsigned char)utf;
          utfstr++;
        }else{
          /* BE */
          *utfstr=(unsigned char)(utf>>8);
          *(utfstr+1)=(unsigned char)(utf%256);
          utfstr+=2;
        }
      }
    }
  }
  *utfstr=0x0;
  return headofutfstr;
}

#endif
/* end of I18N hack */

/*
** Push byte onto a buffer realloc the buffer if needed.
**
** Returns the (new) buffer pointer.
*/
char *PushByte(struct Push *push, char byte)
{				/* byte to append */
#define PUSH_DEFAULT 32		/* default strings are this big */
    if (!push)
	return NULL;		/* this is a sever error */
    if (!push->string) {
	push->string = (char *)malloc(PUSH_DEFAULT);
	if (!push->string)
	    return NULL;	/* error again */
	push->alloc = PUSH_DEFAULT;
	push->len = 0;
#ifdef DEBUG_PUSH
	fprintf(stderr, "PUSH: INIT at index 0 alloc %d\n", PUSH_DEFAULT);
#endif
    }
    else if ((push->len + 2) >= push->alloc) {
	char *newptr;
	newptr = (char *)realloc(push->string, push->alloc * 2);	/* double the size */
	if (!newptr) {
	    return push->string;	/* live on the old one! */
	}
	push->alloc *= 2;	/* enlarge the alloc size */
	push->string = newptr;	/* use the new buffer */
#ifdef DEBUG_PUSH
	fprintf(stderr,
		"PUSH: REALLOC at index %d to alloc %d\n", push->len,
		push->alloc);
#endif
    }
#ifdef DEBUG_PUSH
    fprintf(stderr, "PUSH: WRITE '%c' at index %d\n", byte, push->len);
#endif
    push->string[push->len++] = byte;
    push->string[push->len] = 0;	/* zero terminate */

    return push->string;	/* current buffer */
}

/*
** Push a string onto a buffer, and realloc the buffer if needed.
**
** Returns the (new) buffer pointer.
*/
char *PushString(struct Push *push, const char *append)
{				/* string to append */
    char *string = NULL;

#if 1
    return PushNString(push, append, strlen(append));
#else
    while (*append) {		/* continue until zero termination */
	string = PushByte(push, *append);
	append++;		/* get next character */
    }
#endif
    return string;		/* this is the new buffer */
}

/*
** Push a limited string onto a buffer, and realloc the buffer if needed.
**
** Returns the (new) buffer pointer.
*/
char *PushNString(struct Push *push, const char *append,/* string to append */
		  int size)
{				/* maximum number of bytes to copy */
    char *string = NULL;
#if 1
    if (!push)
	return NULL;		/* this is a sever error */
    if (!push->string) {
	push->string = (char *)malloc(PUSH_DEFAULT + size);
	if (!push->string)
	    return NULL;	/* error again */
	push->alloc = PUSH_DEFAULT + size;
	push->len = 0;
#ifdef DEBUG_PUSH
	fprintf(stderr, "PUSH: INIT at index 0 alloc %d\n", PUSH_DEFAULT);
#endif
    }
    else if ((push->len + size + 1) >= push->alloc) {
	char *newptr;
	push->alloc = 2*push->alloc + size;	/* enlarge the alloc size */
	newptr = (char *)realloc(push->string, push->alloc);	/* double the size */
	if (!newptr) {
	    return push->string;	/* live on the old one! */
	}
	push->string = newptr;	/* use the new buffer */
#ifdef DEBUG_PUSH
	fprintf(stderr,
		"PUSH: REALLOC at index %d to alloc %d\n", push->len,
		push->alloc);
#endif
    }
#ifdef DEBUG_PUSH
    fprintf(stderr, "PUSH: WRITE '%s' at index %d\n", append, push->len);
#endif
    strncpy(push->string + push->len, append, size);
    push->len += size;
    push->string[push->len] = 0;

    string = push->string;	/* current buffer */

#else
    while (*append && size--) {	/* continue until zero termination */
	string = PushByte(push, *append);
	append++;		/* get next character */
    }
#endif

    return string;		/* this is the new buffer */
}


/*
** Malloc() out a string, give it to whoever asked for it.
*/

char *strsav(const char *s)
{
    char *p;

    if (NULL == s)
	s = "";			/* Daniel's kludge to survive better */

    p = (char *)emalloc(strlen(s) + 1);
    strcpy(p, s);
    return p;
}

char *strreplace(char *present, char *new)
{
    char *retval;
    int len;
    if (new == NULL) {
	free(present);
	return NULL;
    }
    len = strlen(new) + 1;
    if (present == NULL)
	retval = (char *)malloc(len);
    else
	retval = (char *)realloc(present, len);
    return strcpy(retval, new);	/* CCC is safe, buffer allocated for it
					   by length of string 'new' */
}

/*
** strcpymax() - copies a string, but max N bytes. It guarantees the
** destination string to be zero terminated. That is, if the destination
** buffer is X bytes, set N to X.
*/

void strcpymax(char *dest, const char *src, int n)
{
    int i;
    if (n) {
	n--;			/* decrease one to allow for the termination byte */
	for (i = 0; *src && (i < n); i++)
	    *dest++ = *src++;
    }
    *dest = 0;
}

/*
** strtolower - converts a string to lower case.
*/

void strtolower(char *string)
{
  char *ptr;

  if (!string)
    return;

  ptr = string;
  while (*ptr) {
    if (isalpha (*ptr))
      *ptr = tolower (*ptr);
    ptr++;
  }
}

#ifndef HAVE_STRCASESTR
/*
** strcasestr() - case insensitive strstr()
*/

/* Stolen-- stolen!-- from glibc 2.1. Please don't sue me. */

char *strcasestr (const char *phaystack, const char *pneedle)
{
  register unsigned char *haystack;
  register const unsigned char *needle;
  register unsigned b, c;

  haystack = (unsigned char *) phaystack;
  needle = (const unsigned char *) pneedle;

  b = tolower (*needle);
  if (b != '\0')
    {
      haystack--;				/* possible ANSI violation */
      do
	{
	  c = *++haystack;
	  if (c == '\0')
	    goto ret0;
	}
      while (tolower (c) != b);

      c = tolower (*++needle);
      if (c == '\0')
	goto foundneedle;
      ++needle;
      goto jin;

      for (;;)
        {
          register unsigned a;
	  register unsigned char *rhaystack;
	  register const unsigned char *rneedle;

	  do
	    {
	      a = *++haystack;
	      if (a == '\0')
		goto ret0;
	      if (tolower (a) == b)
		break;
	      a = *++haystack;
	      if (a == '\0')
		goto ret0;
shloop:	    ;
    	 }
          while (tolower (a) != b);

jin:	  a = *++haystack;
	  if (a == '\0')
	    goto ret0;

	  if (tolower (a) != c)
	    goto shloop;

	  rhaystack = haystack-- + 1;
	  rneedle = needle;
	  a = tolower (*rneedle);

	  if (tolower (*rhaystack) == a)
	    do
	      {
		if (a == '\0')
		  goto foundneedle;
		++rhaystack;
		a = tolower (*++needle);
		if (tolower (*rhaystack) != a)
		  break;
		if (a == '\0')
		  goto foundneedle;
		++rhaystack;
		a = tolower (*++needle);
	      }
	    while (tolower (*rhaystack) == a);

	  needle = rneedle;		/* took the register-poor approach */

	  if (a == '\0')
	    break;
        }
    }
foundneedle:
  return (char*) haystack;
ret0:
  return 0;
}
#endif

/*
** Strips the timezone information from long date strings, so more correct
** comparisons can be made between dates when looking for article replies.
** Y2K ok.
*/

char *stripzone(char *date)
{
    int num;
    static char tmpdate[DATESTRLEN];

    if (!strcmp(date, NODATE))
	return (date);

    if (!strchr(date, ':'))
	return (date);

    strcpymax(tmpdate, date, DATESTRLEN);
    num = strlen(tmpdate);
    while (tmpdate[num] != ':')
	num--;

    num += 3;
    while (tmpdate[num])
	tmpdate[num++] = '\0';

    return (tmpdate);
}

/*
** How many times does the character c appear in string s?
*/

int numstrchr(char *s, char c)
{
    int i;

    for (i = 0; *s != '\0'; s++) {
	if (*s == c)
	    i++;
    }
    return i;
}


/*
** Grabs whatever happens to be between the outermost double quotes in a line.
** This is for grabbing the values in comments.
*/

char *getvalue(char *line)
{
    int i;
    int len;
    char *c, *d;
    struct Push buff;

    INIT_PUSH(buff);

    c = strchr(line, '\"');
    d = strrchr(line, '\"');
    if (c == NULL)
	return "";
    for (c++, i = 0, len = MAXLINE - 1; *c && c != d && i < len; c++)
	PushByte(&buff, *c);

    RETURN_PUSH(buff);
}


/* 
** Get rid of Re:'s in a subject and strips spaces at the end
** of subjects. Make the subject index much less cluttered.
**
** Returns an ALLOCATED string.
*/

char *unre(char *subject)
{
    int ws = 0;
    char *c, *s;

    struct Push buff;
    INIT_PUSH(buff);

    s = subject;

    while (findre(s, &c)) {
	s = c;			/* get position after next re-string */
    }

    while (*s && isspace(*s))
	s++;

    c = s;			/* the first non-space position after the last re: */

    while (*c) {
	if (c && isspace(*c))
	    ws++;
	else {
	    if (ws) {
		PushByte(&buff, ' ');
		ws = 0;
	    }
	    PushByte(&buff, *c);
	}
	c++;
    }
    if (!PUSH_STRLEN(buff)) {
	PushByte(&buff, '\0');
    }

    RETURN_PUSH(buff);
}

/*
** Only gets rid of one re: in a subject, so messages the subject is a reply to
** can be guessed.
*/

char *oneunre(char *subject)
{
    char *c;
    struct Push buff;

    INIT_PUSH(buff);

    if (isre(subject, &c)) {
	if (*c && isspace(*c))
	    c++;
	subject = c;
	PushString(&buff, subject);
	RETURN_PUSH(buff);
    }
    return NULL;
}

/*
** Is a line in an article body part of a quoted passage?
*/

int isquote(const char *line)
{
    const char *lp;
    const char *quote_prefix = get_quote_prefix();

    if (!line)
	return (0);

    if (*quote_prefix) {
	return !strncmp(quote_prefix, line, strlen(quote_prefix));
    }
    if (*line == '>')
	return 1;

    lp = line;

#ifdef RECOGNIZE_SUPERCITE_QUOTES
    /*
       ** If there is a ":" in the first column, 
       ** it means the text is quoted. 
     */
    if (*lp == ':') {
	const char *cp;
	/* 
	   ** Check to make sure that smileys are not
	   ** intrepreted as Supercite Quotes.
	 */
	cp = lp + 1;
	if (*cp && *cp != '-' && *cp != ']' &&
	    *cp != '>' && *cp != '(' && *cp != ')' && *cp != '^')
	    return 1;
    }
#endif

    while (*lp && (*lp == ' ' || *lp == '\t'))
	lp++;

    if (!(*lp))
	return 0;

#ifdef RECOGNIZE_SUPERCITE_QUOTES
    /*
       ** recognize citations in the form "  Jane>" 
     */

    while (*lp &&
	   ((*lp >= 'A' && *lp <= 'Z') ||
	    (*lp >= 'a' && *lp <= 'z'))) lp++;
#endif

    if (*lp == '>')
	return 1;

    return 0;
}

/*
** A ripoff from convchars, for a special case.
** Converts - to &ndash;. Used for storing strings
** in XML comments.
**
** Returns an ALLOCATED string!
*/
char *convdash (char *line)
{
    struct Push buff;

    INIT_PUSH(buff);		/* init macro */

    /* avoid strlen() for speed */

    for (; *line; line++) {

      if (*line == '-')
	PushString(&buff, "&ndash;");
      else
	PushByte(&buff, *line);
    }
    RETURN_PUSH(buff);
} /* end convdash() */

/*
** Converts <, >, and & to &lt;, &gt; and &amp;.
** It was ugly. Now its better. And probably faster.
**
** Returns an ALLOCATED string!
*/

char *convcharsreal(char *line, char *charset, int spamprotect)
{
    struct Push buff;
    int in_ascii = TRUE, esclen = 0;
    int seen_at = FALSE;
    bool is_iso_8859_1;

    if (charset && !strcasecmp ("iso-8859-1", charset))
      is_iso_8859_1 = TRUE;
    else
      is_iso_8859_1 = FALSE;

    INIT_PUSH(buff);		/* init macro */

    /* avoid strlen() for speed */

    for (; *line; line++) {

	if (set_iso2022jp) {
		iso2022_state(line, &in_ascii, &esclen);
		if (esclen && in_ascii == FALSE) {
			for (; in_ascii == FALSE && *line; line++) {
				PushByte(&buff, *line);
				iso2022_state(line, &in_ascii, &esclen);
			}
			line--;
			continue;
		}
	}

	/* @@ JK : try to convert from the WinLatin1 code */
	if (is_iso_8859_1
	    && (unsigned char) (*line) >= 0x80 && (unsigned char) (*line) <= 0x9f) {
	  char *unicode_entity;
	  trio_asprintf (&unicode_entity, 
			 "&#x%x;", WIN1252CP[(unsigned char) (*line) - WIN1252CP_length]);

	  if (unicode_entity) {
	    PushString(&buff, (const char *) unicode_entity);
	    free (unicode_entity);
	  }
	  continue;
	}

	switch (*line) {
	case '<':
	    PushString(&buff, "&lt;");
	    break;
	case '>':
	    PushString(&buff, "&gt;");
	    break;
	case '&':
	    PushString(&buff, "&amp;");
	    break;
	case '\"':
	    PushString(&buff, "&quot;");
	    break;
	case '@': /* pkn added: simple "antispam" measure */
	    PushString(&buff, "&#64;");
	    seen_at = TRUE;
	    break;
	case '.': /* pkn added */
	    if (seen_at && spamprotect)
	    {
	    	PushString(&buff, "&#46;<!--nospam-->");
	    	seen_at = FALSE;
	    	break;
	    }
	    /* fall through */
	default:
	    PushByte(&buff, *line);
	}
    }
    RETURN_PUSH(buff);
} /* end convchars() */

char *convcharsnospamprotect(char *line, char *charset)
{
    return convcharsreal(line, charset, FALSE);
}
char *convchars(char *line, char *charset)
{
    return convcharsreal(line, charset, set_spamprotect);
}

/*
** Converts from an Unicode entity to the equivalent winlatin charset
*/
static bool unconvwinlatin1 (char *entity, char *conv_char)
{
  int i;
  int value = 0;

  if (!entity || *entity == '\0' || *entity != '#')
    return FALSE;

  if (sscanf (entity, "#x%x;", &value) != 1 || value == 0)
    return FALSE;

  for (i = 0; i <  WIN1252CP_length; i++) {
    if (WIN1252CP[i] == value)
      break;
  }

  if (i == WIN1252CP_length)
    return FALSE;

  *conv_char =  (char) (i + 0x80);

  return TRUE;
} /* end unconvwinlatin1 */

/*
** Just the opposite of convchars().
** Completely rewritten 17th Nov 1998 by Daniel.
*/

char *unconvchars(char *line)
{
    char conv_char;
    struct Push buff;
    INIT_PUSH(buff);

    for (; *line; line++) {
	if (*line == '&') {
	    if (!strncmp("lt;", line + 1, 3)) {
		PushByte(&buff, '<');
		line += 3;
	    }
	    else if (!strncmp("gt;", line + 1, 3)) {
		PushByte(&buff, '>');
		line += 3;
	    }
	    else if (!strncmp("amp;", line + 1, 4)) {
		PushByte(&buff, '&');
		line += 4;
	    }
	    else if (!strncmp("&#46;<!--nospam-->", line + 1, 18)) {
	        PushByte(&buff, '.');
	        line += 18;
	    }
	    else if (!strncmp("quot;", line + 1, 5)) {
		PushByte(&buff, '\"');
		line += 5;
	    }
	    else if (!strncmp("#64;", line + 1, 4)) {
		PushByte(&buff, '@');
		line += 4;
	    }
	    else if (!strncmp("ndash;", line + 1, 6)) {
	        PushByte(&buff, '-');
		line += 6;
	    }
	    else if (unconvwinlatin1 (line + 1, &conv_char)) {
		PushByte(&buff, conv_char);
	    }
	    else
	        PushByte(&buff, *line);
	}
	else
	  PushByte(&buff, *line);
    }
    RETURN_PUSH(buff);
}

/*
 * translatechars() is just a ripoff of convertchars() 
 * with a different argument list.
 */

static void translatechars(char *start, char *end, struct Push *buff)
{
    char *p;
    int in_ascii = TRUE, esclen = 0;

    for (p = start; p <= end; p++) {

	if (set_iso2022jp) {
		iso2022_state(p, &in_ascii, &esclen);
		if (esclen && in_ascii == FALSE) {
			for (; in_ascii == FALSE && p <= end; p++) {
				PushByte(buff, *p);
				iso2022_state(p, &in_ascii, &esclen);
			}
			p--;
			continue;
		}
	}

	switch (*p) {
	case '<':
	    PushString(buff, "&lt;");
	    break;
	case '>':
	    PushString(buff, "&gt;");
	    break;
	case '&':
	    PushString(buff, "&amp;");
	    break;
	case '\"':
	    PushString(buff, "&quot;");
	    break;
	default:
	    PushByte(buff, *p);
	}
    }
}


/*
 * translateurl(), to escape URI strings only.
 *  this should be divided from convchars().
 *
 *   in_mailcommand: line is MAILCOMMAND, if 1
 *
 */
static char *translateurl(char *line, int in_mailcommand)
{

  int hexbuflen;
  char hexbuf[16];
  struct Push buff;
  INIT_PUSH(buff);		/* init macro */
  
  for(; *line; line++){
    if(isalnum((int)*line)){
      PushByte(&buff,*line);
    }else{
      switch (*line){
	/* we can use unreserved characters, others should be escaped */
	/*  RFC2396: */
	/*  unreserved = alphanum | mark */
	/*  mark = "-" | "_" | "." | "!" | "~" | "*" | "'" | "(" | ")" */
      case '-':
	PushByte(&buff,*line);
	break;
      case '_':
	PushByte(&buff,*line);
	break;
      case '.':
	PushByte(&buff,*line);
	break;
      case '!':
	PushByte(&buff,*line);
	break;
      case '~':
	PushByte(&buff,*line);
	break;
      case '*':
	PushByte(&buff,*line);
	break;
      case '\'':
	PushByte(&buff,*line);
	break;
      case '(':
	PushByte(&buff,*line);
	break;
      case ')':
	PushByte(&buff,*line);
	break;

	/*
	 * following characters are reserved, but used in MAILCOMMAND
	 */

      case ':':
	if (in_mailcommand){
	  PushByte(&buff,*line);
	  break;
	}
      case '?':
	if (in_mailcommand){
	  PushByte(&buff,*line);
	  break;
	}
      case '&':
	if (in_mailcommand){
	  /* ...to reduce CPU cycles */
	  PushString(&buff,"&amp;");
	  break;
	}
      case '=':
	if (in_mailcommand){
	  PushByte(&buff,*line);
	  break;
	}
      case '$':
	if (in_mailcommand){
	  PushByte(&buff,*line);
	  break;
	}

      default:
	/* URIs MUST NOT have non-ascii characters */
	/* otherwise, we must use IRI */
	hexbuflen=snprintf(hexbuf,4,"%%%02X",*line);
	PushString(&buff,hexbuf);
	break;
      }
    }
  } 
  RETURN_PUSH(buff);
}

/* Given a string, replaces all instances of "oldpiece" with "newpiece".
**
** Modified this routine to eliminate recursion and to avoid infinite
** expansion of string when newpiece contains oldpiece.  --Byron Darrah
**
** 1998-11-17 (Daniel) modified to deal with any length strings and dynamic
** buffers.
*/

char *replace(char *string, char *oldpiece, char *newpiece)
{
    int str_index, newstr_index, oldpiece_index, end,
	new_len, old_len, cpy_len;
    char *c;

    struct Push buff;

    INIT_PUSH(buff);

    if ((c = (char *)strstr(string, oldpiece)) == NULL) {
	/* push the whole thing */
	PushString(&buff, string);
	RETURN_PUSH(buff);
    }

    new_len = strlen(newpiece);
    old_len = strlen(oldpiece);
    end = strlen(string) - old_len;
    oldpiece_index = c - string;

    newstr_index = 0;
    str_index = 0;
    while (str_index <= end && c != NULL) {
	/* Copy characters from the left of matched pattern occurence */
	cpy_len = oldpiece_index - str_index;
	PushNString(&buff, string + str_index, cpy_len);

	newstr_index += cpy_len;
	str_index += cpy_len;

	/* Copy replacement characters instead of matched pattern */

	PushString(&buff, newpiece);

	newstr_index += new_len;
	str_index += old_len;

	/* Check for another pattern match */
	if ((c = (char *)strstr(string + str_index, oldpiece)) != NULL)
	    oldpiece_index = c - string;
    }
    /* Copy remaining characters from the right of last matched pattern */
    PushString(&buff, string + str_index);

    RETURN_PUSH(buff);
}

char *replacechar(char *string, char old, char *new)
{
    struct Push buff;
    int in_ascii = TRUE, esclen = 0;

    INIT_PUSH(buff);

    for (; *string; string++) {
	if (set_iso2022jp) iso2022_state(string, &in_ascii, &esclen);
	if (in_ascii == TRUE && *string == old) {
	    PushString(&buff, new);
	}
	else
	    PushByte(&buff, *string);
    }

    RETURN_PUSH(buff);
}

/*
** Generates the mail command to use from the default mail command,
** the current recipient's email address, the current ID of the
** message, and the current subject.
**
** Returns an ALLOCATED string!
*/

char *makemailcommand(char *mailcommand, char *email, char *id, char *subject)
{
  char *newcmd = NULL, *newcmd2=NULL, *cp;
  char *tmpsubject=NULL;
  char *convsubj=NULL,*convemail=NULL,*convid=NULL;
   
  if ((cp = strrchr(email, ' ')) != NULL)
    *cp = '\0';

  /* prepare "Re: " string, should this be localized? */
  if(subject && isre(subject,NULL)){
    trio_asprintf(&tmpsubject, "%s",subject);
  }else{
    trio_asprintf(&tmpsubject, "Re: %s",subject);
  }

  /* escape strings */
  convsubj=translateurl(tmpsubject,0);
  free(tmpsubject);
  if(set_email_address_obfuscation){
    convemail=obfuscate_email_address(email);
  }else{
    convemail=translateurl(email,0);
  }
  convid=translateurl(id,0);
   
  /* escape mailcommand, with keeping some delimiters */
  newcmd = translateurl(mailcommand,1);

  /* put email address */
  if (strlen(email)>0){
    newcmd2 = replace(newcmd, "$TO", convemail);
  }else{
    newcmd2 = replace(newcmd, "$TO", "");
  }
  free(convemail);
  free(newcmd);

  /* put message-id */
  if (strlen(id)>0){
    newcmd = replace(newcmd2, "$ID", convid);
  }else{
    newcmd = replace(newcmd2, "$ID", "");
  }
  free(convid);
  free(newcmd2);
   
  /* put subject */
  if (subject && strlen(subject)>0){
    newcmd2 = replace(newcmd, "$SUBJECT", convsubj);
  }else{
    newcmd2 = replace(newcmd, "$SUBJECT", "");
  }
  free(newcmd);
  free(convsubj);

  return newcmd2;
}

/*
** Generates the inreplyto command to use from the 
** default inreplyto command and the the current ID of the
** message,
**
** Returns an ALLOCATED string!
*/

char *makeinreplytocommand(char *inreplytocommand, char *id)
{
  char *newcmd = NULL;
  char *convid = NULL;

  /* escape id */
  if (set_email_address_obfuscation){
    convid = obfuscate_email_address (id);
  } else {
    convid = translateurl (id, 0);
  }

  /* replace message-id */
  if (strlen (id)>0) {
    newcmd = replace (inreplytocommand, "$ID", convid);
  } else {
    newcmd = replace (inreplytocommand, "$ID", "");
  }
  free (convid);
   
  return newcmd;
}

char *unspamify(char *s)
{
    char *p;
    if (!s)
	return s;
    if (!strchr(s, '@') && ((p = strstr(s, set_antispam_at)) != NULL)) {
	struct Push buff;
	INIT_PUSH(buff);
	PushNString(&buff, s, p - s);
	PushByte(&buff, '@');
	PushString(&buff, p + strlen(set_antispam_at));
	return PUSH_STRING(buff);
    }
    return strsav(s);
}

/*
** RFC 1738
** Thus, only alphanumerics, the special characters "$-_.+!*'(),", and
** reserved characters used for their reserved purposes may be used
** unencoded within a URL.
**
**
**        //<user>:<password>@<host>:<port>/<url-path>
** 
** Some or all of the parts "<user>:<password>@", ":<password>",
** ":<port>", and "/<url-path>" may be excluded.  The scheme specific
** data start with a double slash "//" to indicate that it complies with
** the common Internet scheme syntax.
**
*/

char *parseemail(char *input,	/* string to parse */
		 char *mid,	/* message ID */
		 char *msubject)
{				/* message subject */
    char mailbuff[256];
    char mailaddr[MAILSTRLEN];
    char tempbuff[MAXLINE];
    char *ptr;
    char *lastpos = input;
    struct Push buff;
    
    char *at;
    int at_len;

    int in_ascii = TRUE, esclen = 0;

    if(set_spamprotect)
      at=set_antispam_at;
    else
      at="@";

    INIT_PUSH(buff);

    while (*input) {
      if ((ptr = strchr (input, '@')))
	at_len = 1;
      else if ((ptr = strstr (input, "&#64;")))
	at_len = 5;
      if (ptr) {
	    /* found a @ */
	    char *email = ptr - 1;
	    char content[2];
	    int backoff = ptr - input;	/* max */

#define VALID_IN_EMAIL_USERNAME   "a-zA-Z0-9.!#$%&'*+-/=?^_`{|}~"
#define VALID_IN_EMAIL_DOMAINNAME "a-zA-Z0-9.-"

	    if (set_iso2022jp) {
		    for (; ptr > input; input++) {
			    iso2022_state(input, &in_ascii, &esclen);
			    if (!esclen) continue;
			    input += esclen;
			    if (in_ascii == TRUE)
				    backoff = ptr - input;
		    }
	    }

	    /* check left side */
	    while (backoff) {
		if (sscanf
		    (email, "%1[" VALID_IN_EMAIL_USERNAME "]", content) == 1) {
		    email--;
		    backoff--;
		}
		else
		    break;
	    }
	    if (backoff > 2 && email[0] == '/' && email[-1] == '/'
		&& email[-2] == ':') {
	        ;		/* part of a url such as ftp://user@host.com */
	    }
	    else if (email != ptr - 1) { /* bigger chance this is an address */
		email++;
		if (sscanf
		    (ptr + at_len, "%255[" VALID_IN_EMAIL_DOMAINNAME "]",
		     mailbuff) == 1) {

		    /* a valid mail right-end */
		    if (lastpos < email) {
			PushNString(&buff, lastpos, email - lastpos);
		    }

                    trio_snprintf(mailaddr, sizeof(mailaddr),"%.*s%s%s", 
				  ptr-email, email, at, mailbuff);

		    if (valid_root_domain(mailaddr)) {
			char *mailcmd = makemailcommand(set_mailcommand,
							mailaddr, mid,
							msubject);
			trio_snprintf(tempbuff, sizeof(tempbuff),
				      "<a href=\"%s\">%s</a>", mailcmd,
				      obfuscate_email_address(mailaddr));

			free(mailcmd);

			PushString(&buff, tempbuff);

			input = ptr + strlen(mailbuff) + at_len;
			lastpos = input;
			continue;
		    }
		    else {	/* bad address */
			PushString(&buff, mailaddr);
			input = ptr + strlen(mailbuff) + at_len;
			lastpos = input;
			continue;
		    }
		}
	    }
	    /* no address, continue from here */
	    input = ptr + 1;
	    continue;
      }
      else
	input = strchr(input, '\0');
    }
    if (lastpos < input) {
	PushNString(&buff, lastpos, input - lastpos);
    }
    RETURN_PUSH(buff);
}

/*
** Returns the allocated and converted string.
**
** This function is run on each and every body line, so it pays to make it
** run quickly.
**/

/* jk 8/03/2013: commented all the URLs that don't end with :// as the
   current hypermail convurl code doesn't know how to handle them,
   which results in sigsevs in some cases */
static char *url[] = {
    "http://",
    "https://",
    "news:",
    "ftp://",
#if 0
    "file://",/* can expose private files outside the archive in some cases? */
#endif
    "gopher://",
    "nntp://",
    /* "wais://", */ /* deprecated */
    "telnet://",
    "prospero://", /* deprecated */
/* "mailto:", *//* Can't have mailto: as it will be converted twice */
    /*"tel:", */
    /* "fax:", */
    "rtsp://",
    /* "im:", */
    /* some non RFC or experimental or de-facto ones */
    "cap://",
    "feed://",
    "webcal://",
    "irc://",
    /* "callto:", */
    NULL
};

char *parseurl(char *input, char *charset)
{
    struct Push buff;		/* output buffer */
    char urlbuff[256];
    char tempbuff[MAXLINE];
    char *inputp;
    char *match[sizeof(url) / sizeof(char **)];
    int first;

    if (!input || !*input)
	return NULL;

    if (!strstr(input, "://")) {
	/*
	 * All our protocol prefixes have this "//:" substring in them. Most
	 * of the lines we process don't have any URL. Let's not spend any
	 * time looking for URLs in lines that we can prove cheaply don't
	 * have any; it will be a big win for average input if we follow
	 * this trivial heuristic. 
	 */
	return convchars(input, charset);
    }
    INIT_PUSH(buff);

    /*
     * Iterate on possible URLs in the input string. There might 
     * be more than one! 
     */

    inputp = input;
    first = TRUE;

    while (1) {
	char *leftmost = NULL;
	char **up;
	char thisprotocol[256];
	char *p;

	/* Loop on protocol prefixes, searching for the leftmost. */

	for (up = url; *up; up++) {
	    int i;

	    if (first) {
		/* 
		 * we haven't looked for this one yet, so do so and remember
		 * any match we find 
		 */
		match[up - url] = p = strcasestr(inputp, *up);
	    }
	    else if (match[i = up - url]) {
		/*
                 * we looked for it before, and we found it before, so if the
		 * match is to the right of the input we have processed so
		 * far, we can simply reuse it; else, search again. 
		 */
		if (match[i] <= inputp)
		    match[i] = p = strcasestr(inputp, *up);
		else
		    p = match[i];
	    }
	    else {
		/*
                 * we looked for it before, and we didn't find it before, so
		 * we aren't bloody likely to find it this time; don't waste
		 * time looking again 
                 */
		continue;
	    }

	    if (p) {
		/*
                 * Found a protocol prefix. We want the leftmost such, 
                 * so note the match and keep looking for other protocols. 
                 */
		if (!leftmost || p < leftmost) {
		    char *endp;
		    int len;
		    leftmost = p;
		    memset(thisprotocol, 0, sizeof(thisprotocol));
		    endp = strstr(p, "://");
		    if (endp) {
			len = endp - p + 3;
			strncpy(thisprotocol, p, len);
			thisprotocol[len] = '\0';
		    }
		}
	    }
	}

	first = FALSE;

	if (leftmost) { /* we found at least one protocol prefix */
	    int accepted = FALSE;
	    int urlscan = FALSE;

	    /* 
	     * all the charaters between the position where we started
             * looking for a protocol prefix and the protocol prefix
             * need to be checked for character translations 
	     */

	    translatechars(inputp, leftmost-1, &buff);
	    inputp = leftmost + strlen(thisprotocol);

	    if (set_iso2022jp)
		    urlscan = sscanf(inputp, "%255[^] \033)>\"\'\n[\t\\]", urlbuff);
	    else
		    urlscan = sscanf(inputp, "%255[^] )<>\"\'\n[\t\\]", urlbuff);
	    if (urlscan == 1) {
	        char *r;
	
		/* 
		 * A valid url: up to 255 characters in a run containing legal
		 * URL characters. But let's nibble off any punctuation other
		 * than slashes at the end, because they are not likely part
		 * of the URL. E.g. a trailing comma. 
		 */

		for(r = strchr(urlbuff, '\0') - 1; 
		    *r != '/' && ispunct(*r) && r > urlbuff;
		    r--);
		if(r++ > urlbuff) {

		    /* 
		     * there should be something left in the URL after we chew
		     * away the trailing punctuation if we are going to call it
		     * valid 
		     */

		    accepted = TRUE;
		    if(*r) {
		        *r = '\0';
		    }
		}
	    }
	    if(accepted) {
	      /* string is already escaped in URI context */
	        char *urlbuff2 = convcharsnospamprotect(urlbuff,"us-ascii");

		trio_snprintf(tempbuff, sizeof(tempbuff),
			      "<a href=\"%s%s\">%s%s</a>",
			      thisprotocol, urlbuff2, thisprotocol, urlbuff2);
		PushString(&buff, tempbuff); /* append the tag buffer */
		inputp += strlen(urlbuff);
		free(urlbuff2);
	    } else {
	        PushString(&buff, thisprotocol);
	    }
	} else {
	    /*
	     * no prospects found; translate the characters in the rest of
	     * the string and return 
	     */
	    translatechars(inputp, strchr(inputp, '\0') - 1, &buff);
	    break;
	}
    }
    RETURN_PUSH(buff);
} /* end parseurl() */

/*
 * Support RFC1468 (and RFC1554, 94 character sets)
 *
 * reference
 * - RFC1468: Japanese Character Encoding for Internet Messages (ISO-2022-JP)
 * - RFC1554: ISO-2022-JP-2: Multilingual Extension of ISO-2022-JP
 * - RFC1557: Korean Character Encoding for Internet Messages
 * - RFC2234: Japanese Character Encoding for Internet Messages
 */

/*
 * state
 *	TRUE: ascii (default)
 *	FALSE: non-ascii
 * esclen
 *	n: escape sequence length
 */
void
iso2022_state(const char *str, int *state, int *esclen)
{
	if (*state != TRUE && *state != FALSE)
		*state = TRUE;

	if (*str != '\033') {
		*esclen = 0;
		return;
	}

	switch (*(str+1)) {
	case '$':	
		if (*(str+2) == 'B' || *(str+2) == '@' || *(str+2) == 'A') {
			/*
			 * ESC $ B	JIS X 0208-1983 to G0
			 * ESC $ @	JIS X 0208-1976 to G0
			 * ESC $ A	GB2312-1980 to G0
			 */
			*state = FALSE;
			*esclen = 3;
		} else if ((*(str+2) == '(' && *(str+3) == 'C') ||
			   (*(str+2) == '(' && *(str+3) == 'D')) {
			/*
			 * ESC $ ) C	KSC 5601-1987 to G0
			 * ESC $ ( D	JIS X 0212-1990 to G0
			 */
			*state = FALSE;
			*esclen = 4;
		} else {
			/* keep state */
			*esclen = 1;
		}
		break;
	case '(':
		if (*(str+2) == 'B' || *(str+2) == 'J') {
			/*
			 * ESC ( B	ASCII to G0
			 * ESC ( J	JIS X 0201-Roman to G0
			 */
			*state = TRUE;
			*esclen = 3;
		} else {
			/* keep state */
			*esclen = 1;
		}
		break;
	default:
		/* keep state */
		*esclen = 1;
	}
}

char *
hm_strchr(const char *str, int ch)
{
	if (!set_iso2022jp) {
		return(strchr(str, ch));
	} else {
		int in_ascii = TRUE, esclen = 0;
	
		for (; *str; str++) {
			iso2022_state(str, &in_ascii, &esclen);
			if (esclen) str += esclen;
			if (in_ascii == TRUE) {
				if (*str == ch)
					return((char *)str);
			}
		}
		return((char *)NULL);
	}
}

#if ! HAVE_STRERROR
/*************************************************
*     Provide strerror() for non-ANSI libraries  *
*************************************************/

/* Some old-fashioned systems still around (e.g. SunOS4) don't have strerror()
in their libraries, but can provide the same facility by this simple
alternative function. */

extern int   sys_nerr;
extern char *sys_errlist[];

char *
strerror(int n)
{
if (n < 0 || n >= sys_nerr) return "unknown error number";
return sys_errlist[n];
}
#endif /* HAVE_STRERROR */

