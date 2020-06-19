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
