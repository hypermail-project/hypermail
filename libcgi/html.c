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

void print_doc_begin(char *title)
{
    printf("<head><title>%s</title></head>\n", title);
    printf("<h1>%s</h1>\n<img src=\"http://www.hypermail.org/images/stripe.bottom.gif alt=\"\"><br>",
	 title);
}

void print_doc_end(char *text)
{
    char *w;

    puts("<p><hr>");
    if (text && *text)
	puts(text);
    else if ((w = getenv("WEBMASTER")))
	printf("<address>%s</address>\n", w);
}

void print_logo(void)
{
    printf("<a href=\"http://www.hypermail.org/\"><img src=\"http://www.hypermail.org/images/hypermail-small.gif alt=\"HYPERMAIL\" align=bottom></a>");
}
