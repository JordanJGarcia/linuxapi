#ifdef __linux__
#define _GNU_SOURCE
#endif

#include <sys/utsname.h>
#include "../../lib/tlpi-hdr.h"

int
main(int argc, char * argv[])
{
    printf("See UNAME(2) for more info\n");
    
    printf("\nPrototype:\n");
    printf("\tint uname(struct utsname *buf);\n");

    printf("\nThe uname() system call returns a range of identifying information about\n");
    printf("the host system on which an application is running, in the structure pointed to by <utsbuf>\n");

    printf("\nThe utsbuf argument is a pointer to a utsname structure, of the following form:\n");
    printf("\nstruct utsname {\n");
    printf("\tchar sysname[];    /* Operating system name (e.g., \"Linux\") */\n");
    printf("\tchar nodename[];   /* Name within \"some implementation-defined network\" */\n");
    printf("\tchar release[];    /* Operating system release (e.g., \"2.6.28\") */\n");
    printf("\tchar version[];    /* Operating system version */\n");
    printf("\tchar machine[];    /* Hardware identifier */\n");
    printf("#ifdef _GNU_SOURCE\n");
    printf("\tchar domainname[]; /* NIS or YP domain name */\n");
    printf("#endif\n");
    printf("};\n\n");

    struct utsname uts;

    if (uname(&uts) == -1)
        errExit("uname()");

    printf("Node name:      %s\n", uts.nodename);
    printf("System name:    %s\n", uts.sysname);
    printf("Release:        %s\n", uts.release);
    printf("Version:        %s\n", uts.version);
    printf("Machine:        %s\n", uts.machine);
#ifdef _GNU_SOURCE
    printf("Domain name:    %s\n", uts.domainname);
#endif

    exit(EXIT_SUCCESS);
}
