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
 */

/*
 * This file is part of the LIBCGI library
 *
 */

#include "cgi.h"

#define LF 10
#define CR 13

#define STARTSIZE 8

form_entry *get_form_entries(cgi_info *ci)
{
    if (ci && ci->request_method &&
	!strncasecmp(ci->request_method, "POST", 4) && ci->content_type &&
	!strncasecmp(ci->content_type, "application/x-www-form-urlencoded",
		     33)) return get_fes_from_stream(ci->content_length,
						     stdin);
    else if (ci && ci->request_method &&
	     !strncasecmp(ci->request_method, "GET", 3))
	return get_fes_from_string(ci->query_string);
    else
	return NULL;
}

void free_form_entries(form_entry *fe)
{
    form_entry *tempfe;
    while (fe) {
	if (fe->name)
	    free(fe->name);
	if (fe->val)
	    free(fe->val);
	tempfe = fe->next;
	free(fe);
	fe = tempfe;
    }
}

char *parmval(form_entry *fe, char *s)
{
    while (fe) {
	if (!strcasecmp(fe->name, s))
	    return fe->val;
	else
	    fe = fe->next;
    }
    return NULL;
}

form_entry *get_fes_from_string(char *s)
{
    form_entry *fe;
    int asize;
    int i;

    if (s == NULL)
	return NULL;
    while (isspace(*s) || *s == '&')
	s++;			/* some cases that shouldn't happen */
    if (*s == '\0')
	return NULL;
    fe = (form_entry *)malloc(sizeof(form_entry));
    if (fe == NULL)
	return NULL;
    fe->name = malloc((asize = STARTSIZE * sizeof(char)));
    if (fe->name == NULL) {
	free(fe);
	return NULL;
    }
    /* get form field name */
    for (i = 0; *s && *s != '&' && *s != '='; s++, i++) {
	switch (*s) {
	case '+':
	    fe->name[i] = ' ';
	    break;
	case '%':
	    fe->name[i] = dd2c(s[1], s[2]);
	    s += 2;
	    break;
	default:
	    fe->name[i] = *s;
	}
	if (i + 1 >= asize) {	/* try to double the buffer */
	    fe->name = realloc(fe->name, (asize *= 2));
	    if (fe->name == NULL)
		return NULL;
	}
    }
    fe->name[i] = '\0';
    switch (*s++) {
    case '&':
	fe->val = NULL;
	break;
    case '=':
	fe->val = malloc((asize = STARTSIZE * sizeof(char)));
	if (fe->val == NULL)
	    break;
	for (i = 0; *s && *s != '&'; s++, i++) {
	    switch (*s) {
	    case '+':
		fe->val[i] = ' ';
		break;
	    case '%':
		fe->val[i] = dd2c(s[1], s[2]);
		s += 2;
		break;
	    default:
		fe->val[i] = *s;
	    }
	    if (i + 1 >= asize) {	/* try to double the buffer */
		fe->val = realloc(fe->val, (asize *= 2));
		if (fe->val == NULL)
		    return NULL;
	    }
	}
	fe->val[i] = '\0';
	switch (*s++) {
	case '&':
	    fe->next = get_fes_from_string(s);
	    break;
	case '\0':
	default:
	    fe->next = NULL;
	}
	break;
    case '\0':
    default:
	fe->val = NULL;
	fe->next = NULL;
    }
    return fe;
}

#define getccl(s, l) (l-- ? getc(s) : EOF)

form_entry *get_fes_from_stream(int length, FILE *stream)
{
    form_entry *fe;
    int asize;
    int i;
    int c;
    int c1, c2;

    while (isspace(c = getccl(stream, length)) || c == '&');
    if (c == EOF)
	return NULL;
    fe = (form_entry *)malloc(sizeof(form_entry));
    if (fe == NULL)
	return NULL;
    fe->name = malloc((asize = STARTSIZE * sizeof(char)));
    if (fe->name == NULL) {
	free(fe);
	return NULL;
    }
    /* get form field name */
    for (i = 0; c != EOF && c != '&' && c != '=';
	 c = getccl(stream, length), i++) {
	switch (c) {
	case '+':
	    fe->name[i] = ' ';
	    break;
	case '%':
	    c1 = getccl(stream, length);
	    c2 = getccl(stream, length);
	    fe->name[i] = dd2c(c1, c2);
	    break;
	default:
	    fe->name[i] = c;
	}
	if (i + 1 >= asize) {	/* try to double the buffer */
	    fe->name = realloc(fe->name, (asize *= 2));
	    if (fe->name == NULL)
		return NULL;
	}
    }
    fe->name[i] = '\0';
    if (c == EOF) {
	fe->val = NULL;
	fe->next = NULL;
    }
    else
	switch (c) {
	case '&':
	    fe->val = NULL;
	    break;
	case '=':
	    fe->val = malloc((asize = STARTSIZE * sizeof(char)));
	    for (i = 0, c = getccl(stream, length); c != EOF && c != '&';
		 c = getccl(stream, length), i++) {
		switch (c) {
		case '+':
		    fe->val[i] = ' ';
		    break;
		case '%':
		    c1 = getccl(stream, length);
		    c2 = getccl(stream, length);
		    fe->val[i] = dd2c(c1, c2);
		    break;
		default:
		    fe->val[i] = c;
		}
		if (i + 1 >= asize) {	/* try to double the buffer */
		    fe->val = realloc(fe->val, (asize *= 2));
		    if (fe->val == NULL)
			return NULL;
		}
	    }
	    fe->val[i] = '\0';
	    if (c == '&') {
		fe->next = get_fes_from_stream(length, stream);
	    }
	    else
		fe->next = NULL;
	}
    return fe;
}

unsigned char dd2c(char d1, char d2)
{
    register unsigned char digit;

    digit = (d1 >= 'A' ? ((d1 & 0xdf) - 'A') + 10 : (d1 - '0'));
    digit *= 16;
    digit += (d2 >= 'A' ? ((d2 & 0xdf) - 'A') + 10 : (d2 - '0'));
    return (digit);
}


void dump_cgi_info(cgi_info *ci)
{
    printf("CONTENT_LENGTH:    %d\n", ci->content_length);
    if (ci->content_type != NULL)
	printf("<BR>CONTENT_TYPE:      %s\n", ci->content_type);
    if (ci->server_name != NULL)
	printf("<BR>SERVER_NAME:       %s\n", ci->server_name);
    if (ci->server_software != NULL)
	printf("<BR>SERVER_SOFTWARE:   %s\n", ci->server_software);
    if (ci->gateway_interface != NULL)
	printf("<BR>GATEWAY_INTERFACE: %s\n", ci->gateway_interface);
    if (ci->server_protocol != NULL)
	printf("<BR>SERVER_PROTOCOL:   %s\n", ci->server_protocol);
    if (ci->server_port != NULL)
	printf("<BR>SERVER_PORT:       %s\n", ci->server_port);
    if (ci->request_method != NULL)
	printf("<BR>REQUEST_METHOD:    %s\n", ci->request_method);
    if (ci->http_accept != NULL)
	printf("<BR>HTTP_ACCEPT:       %s\n", ci->http_accept);
    if (ci->path_info != NULL)
	printf("<BR>PATH_INFO:         %s\n", ci->path_info);
    if (ci->path_translated != NULL)
	printf("<BR>PATH_TRANSLATED:   %s\n", ci->path_translated);
    if (ci->script_name != NULL)
	printf("<BR>SCRIPT_NAME:       %s\n", ci->script_name);
    if (ci->query_string != NULL)
	printf("<BR>QUERY_STRING:      %s\n", ci->query_string);
    if (ci->remote_host != NULL)
	printf("<BR>REMOTE_HOST:       %s\n", ci->remote_host);
    if (ci->remote_addr != NULL)
	printf("<BR>REMOTE_ADDR:       %s\n", ci->remote_addr);
    if (ci->auth_type != NULL)
	printf("<BR>AUTH_TYPE:         %s\n", ci->auth_type);
    if (ci->remote_user != NULL)
	printf("<BR>REMOTE_USER:       %s\n", ci->remote_user);
    if (ci->remote_ident != NULL)
	printf("<BR>REMOTE_IDENT:      %s\n", ci->remote_ident);
    return;
}
