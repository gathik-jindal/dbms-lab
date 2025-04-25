// IMT2023089

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "contact.h"

void print_contact(struct Contact *c)
{
    printf("%d,%s,%s\n", c->contact_id, c->contact_name, c->phone);
}

/* Return 0 if name of the contact matches with name parameter */
/* Return 1 if name of the contact does NOT match */
/* Return > 1 in case of any other error */
int contact_name_matcher(void *rec, void *search_name)
{
    // uninitialized pointers
    if (rec == NULL || search_name == NULL)
    {
        return 2;
    }

    int out = strcmp(((struct Contact *)rec)->contact_name, (char *)search_name);

    if (out == 0)
    {
        return 0;
    }

    // names didn't match
    return 1;
}
