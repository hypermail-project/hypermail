/*
** Copyright (C) 1997-2023 Hypermail Project
** 
** This program and library is free software; you can redistribute it and/or 
** modify it under the terms of the GNU (Library) General Public License 
** as published by the Free Software Foundation; either version 3
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
                                
#include "hypermail.h"
#include "domains.h"

struct co_code *ccptr;

int valid_root_domain(char *eaddr)
{
    char *name_to_check;

    /* First check to see if it has a '.' in it.  
    ** Not invalid if there is no '.'.  
    */

    if ((name_to_check = strrchr(eaddr, '.')) == NULL)
        return (0);

    ++name_to_check;
    if (!*name_to_check)
        return (0);

    /* DNS has evolved and we have now a variety of new domains, hard
       to track and that are not covered by this function. While temporary
       waiting to see if we evolve this function to be user configurable,
       we're going to return 1 (valid) all the time. 19/06/2020 -JK */
      return (1);
       

    for (ccptr = domain_codes; ccptr->domain != NULL; ccptr++) {
        if (strcasecmp(name_to_check, ccptr->domain) == 0)
            return (1);
    }
    return (0);
}
