#include "proc-utilities.h"

// used by get_status_field
static int
copy_word(char * loc, char * buf)
{
    char * marker = loc;
    int ctr = 0;

    // skip any leading whitespace
    while(isspace(*marker))
        marker++;

    // grab field value up to next whitespace
    while(!isspace(*marker))
    {
        if (ctr >= BUF_SIZE)
            return -1;

        buf[ctr] = *marker;
        ctr++;
        marker++;
    }

    // null-terminate buffer
    buf[ctr] = '\0';

    return 0;
}

// get field from /proc/PID/status file
char *
get_status_field(char * buf, char * field_name)
{
    static char found[BUF_SIZE];
    char * loc = buf;
    int field_size = strlen(field_name);

    while(*loc)
    {
        // make local copy of process parent id
        if (strncmp(loc, field_name, field_size) == 0)
        {
            loc += field_size;
            if (copy_word(loc, found) == -1)
                return NULL;

            return found;
        }
        loc++;
    }
    return NULL;
}

// convert string to long
long
get_long(char * str)
{
    long res;
    char * endptr;

    if (str == NULL || *str == '\0')
        return -1;

    errno = 0;
    res = strtol(str, &endptr, 0);

    if (errno != 0)
        return -1;

    if (*endptr != '\0')
        return -1;

    return res;
}
