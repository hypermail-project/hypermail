/** 
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
 **/

/**
 ** This file is part of the LIBCGI library
 **
 */

#ifndef CGI_H
#define CGI_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "../config.h"

#ifdef NO_MACRO
#undef isspace
#undef isdigit
#undef isalpha
#undef isupper
#undef islower
#undef isxdigit
#undef isalnum
#undef ispunct
#undef isprint
#undef isgraph
#undef iscntrl
#undef isascii
#endif



#define MCODE_GET 1
#define MCODE_POST 2
#define MCODE_PUT 3
#define MCODE_HEAD 4

typedef struct {
    char *server_software;
    char *server_name;
    char *gateway_interface;
    char *server_protocol;
    char *server_port;
    char *request_method;
    char *http_accept;
    char *path_info;
    char *path_translated;
    char *script_name;
    char *query_string;
    char *remote_host;
    char *remote_addr;
    char *remote_user;
    char *auth_type;
    char *remote_ident;
    char *content_type;
    int content_length;
} cgi_info;

typedef struct festruct {
    char *name;
    char *val;
    struct festruct *next;
} form_entry;


/* Prototypes */
/* ---------- */

int get_cgi_info(cgi_info *);
int syn_base_url(char *, cgi_info *);
int syn_mimeheader(char *, char *);
int mcode(cgi_info *);
char *trim(char *);
char *strmaxcpy(char *, char *, int);
char *sanitize(char *, char *);
char *parmval(form_entry *, char *);
int print_base_url(cgi_info *);
int print_mimeheader(const char *);
void print_doc_begin(char *);
void print_doc_end(char *);
void print_logo(void);
void print_sel_list(char *, char **, char *);
void print_submit(char *);
void print_input_blank(char *, unsigned int, unsigned int, char *);
form_entry *get_form_entries(cgi_info *);
void free_form_entries(form_entry *);
form_entry *get_fes_from_string(char *);
form_entry *get_fes_from_stream(int, FILE *);
unsigned char dd2c(char, char);
void dump_cgi_info(cgi_info *);

#ifdef lint
int isspace(int);
int isalnum(int);

extern int strcasecmp(const char *, const char *);
extern int strncasecmp(const char *, const char *, int);
#endif
#endif
