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

int mcode(cgi_info *ci)
{
    if (ci->request_method == NULL)
	return 0;
    else if (!strncasecmp(ci->request_method, "GET", 3))
	return MCODE_GET;
    else if (!strncasecmp(ci->request_method, "POST", 4))
	return MCODE_POST;
    else if (!strncasecmp(ci->request_method, "PUT", 3))
	return MCODE_PUT;
    else if (!strncasecmp(ci->request_method, "HEAD", 4))
	return MCODE_HEAD;
    else
	return 0;
}
