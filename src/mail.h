/* mail.h
*/

#ifndef _MAIL_H
#define _MAIL_H

#include "../libcgi/cgi.h"
#include "../config.h"

#ifdef HAVE_SYS_TYPES_H 
#include <sys/types.h>
#endif

#ifdef HAVE_SYS_SOCKET_H 
#include <sys/socket.h>
#endif

#ifdef HAVE_NETINET_IN_H
#include <netinet/in.h>
#endif

#ifdef HAVE_ARPA_INET_H
#include <arpa/inet.h>
#endif

#ifdef HAVE_NETDB_H
#include <netdb.h>
#endif

#ifdef HAVE_STRING_H
#include <string.h>
#endif

#define CGINAME  "/cgi-bin/mail"
#define SENDMAIL "/usr/lib/sendmail"
#define ROWS     20
#define COLS     60
#define MAXLEN   80

char *strsav(char *);
char *lookupnumaddr(char *);
void cgi_main(cgi_info *);
void printform(char *, char *, char *, char *, char *, cgi_info *);

#endif
