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
 * This file is part of the LIBCGI library
 *
 */

#include "cgi.h"

int get_cgi_info(cgi_info *ci)
{
    char *s;

    ci->content_length = (s = getenv("CONTENT_LENGTH")) ? atoi(s) : 0;
    ci->content_type = getenv("CONTENT_TYPE");
    ci->server_software = getenv("SERVER_SOFTWARE");
    ci->gateway_interface = getenv("GATEWAY_INTERFACE");
    ci->server_protocol = getenv("SERVER_PROTOCOL");
    ci->server_port = getenv("SERVER_PORT");
    ci->request_method = getenv("REQUEST_METHOD");
    ci->http_accept = getenv("HTTP_ACCEPT");
    ci->path_info = getenv("PATH_INFO");
    ci->path_translated = getenv("PATH_TRANSLATED");
    ci->script_name = getenv("SCRIPT_NAME");
    ci->query_string = getenv("QUERY_STRING");
    ci->remote_host = getenv("REMOTE_HOST");
    ci->remote_addr = getenv("REMOTE_ADDR");
    ci->remote_user = getenv("REMOTE_USER");
    ci->auth_type = getenv("AUTH_TYPE");
    ci->remote_user = getenv("REMOTE_USER");
    ci->remote_ident = getenv("REMOTE_IDENT");
    return (ci->server_name = getenv("SERVER_NAME")) != NULL;
}
