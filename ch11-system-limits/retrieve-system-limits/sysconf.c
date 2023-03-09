#include <unistd.h>
#include "../../lib/tlpi-hdr.h"

static void
sysconfPrint(const char * msg, int name)
{
    long lim;

    errno = 0;
    lim = sysconf(name);

    // call succeeded
    if (lim != -1)
        printf("%s %ld\n", msg, lim);
    else
    {
        // limit indeterminate
        if (errno == 0)
            printf("%s (indeterminate)\n", msg);
        else
            errExit("sysconf %s", msg);
    }
}


int
main(int argc, char * argv[])
{
    printf("See SYSCONF(3) for more info\n");
    
    printf("\nPrototype:\n");
    printf("\tlong sysconf(int name);\n");

    printf("\nThe sysconf() function allows an application to obtain the values of\n");
    printf("system limits at run time\n");

    printf("\nThe <name> argument is one of the _SC_* constants defined in <unistd.h>\n");
    
    printf("\nThe return value is either the value of the limit found for <name>, or\n");
    printf("-1 if limit not found or on error\n");

    printf("\nTo distinguish the case of an indeterminate limit from an error,\n");
    printf("we must set errno to 0 before the call, if the result is -1 and the value\n");
    printf("in errno has changed, an error has occured\n\n");

    sysconfPrint("_SC_ARG_MAX:          ", _SC_ARG_MAX);
    sysconfPrint("_SC_LOGIN_NAME_MAX:   ", _SC_LOGIN_NAME_MAX);
    sysconfPrint("_SC_OPEN_MAX:         ", _SC_OPEN_MAX);
    sysconfPrint("_SC_NGROUPS_MAX:      ", _SC_NGROUPS_MAX);
    sysconfPrint("_SC_PAGESIZE:         ", _SC_PAGESIZE);
    sysconfPrint("_SC_RTSIG_MAX:        ", _SC_RTSIG_MAX);

    exit(EXIT_SUCCESS);
}
