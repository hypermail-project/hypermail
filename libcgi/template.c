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

void cgi_main(cgi_info *ci)
{
    char *parmval();
    form_entry *parms, *p;
    form_entry *get_form_entries();
    char *foo, *bar;

    print_mimeheader("text/html");

    puts("<title>Your Title Here</title>");
    puts("<h1>Your heading here</h1>");

    parms = get_form_entries(ci);
    if (parms) {
	/* extract specific form parameters */
	for (p = parms; p; p = p->next) {
	    if (strcasecmp(p->name, "foo"))
		foo = p->val;
	    else if (strcasecmp(p->name, "bar"))
		bar = p->val;
	}
    }

    switch (mcode(ci)) {

    case MCODE_HEAD:
	return;

    case MCODE_GET:
	puts("Your GET response here");
	printf("based on foo=%s and bar=%s.\n", foo, bar);
	break;

    case MCODE_POST:
	puts("Your POST response here");
	printf("based on foo=%s and bar=%s.\n", foo, bar);
	break;

    default:
	printf("Unrecognized method '%s'.\n", ci->request_method);
    }

    free_form_entries(parms);
}
