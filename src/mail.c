/* 
** Copyright (C) 1994, 1995 Enterprise Integration Technologies Corp.
**         VeriFone Inc./Hewlett-Packard. All Rights Reserved.
** Kevin Hughes, kev@kevcom.com 3/11/94
** Jay Weber, weber@eit.com
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
#include "../libcgi/cgi.h"
#include "../config.h"

void cgi_main(cgi_info *ci)
{
    /* This program has been disabled because it
     * was probably easy for spammers to use as an open relay. It also
     * had problems with enabling malicious use of JavaScript and
     * CRLF Injection.
     */
    printf("<html>\n");
    printf("<body bgcolor=\"#ffffff\" text=\"#000000\">\n");
    printf("This page has been disabled due to potential abuse by spammers.\n");
    printf("</body>\n</html>\n");
}
