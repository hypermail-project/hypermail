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

    for (ccptr = domain_codes; ccptr->domain != NULL; ccptr++) {
        if (strcasecmp(name_to_check, ccptr->domain) == 0)
            return (1);
    }
    return (0);
}
