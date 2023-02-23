#include "../../lib/tlpi-hdr.h"

#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif

// globals
extern char * program_invocation_name;
extern char * program_invocation_short_name;

// main
int
main(int argc, char * argv[])
{
    // usage
    if (argc != 1)
        usageErr("%s", argv[0]);

    // print info
    printf("The GNU C library provides 2 global variables that may be used anywhere in\n");
    printf("a program in order to obtain the name used to invoke the program\n");
    printf("This includes the complete pathname and a version with any directory\n");
    printf("prefix stripped off.\n\n");

    // program invocation names defined in <errno.h> under _GNU_SOURCE macro
    printf("program_invocation_name: %s\n", program_invocation_name);
    printf("program_invocation_short_name: %s\n", program_invocation_short_name);

    exit(EXIT_SUCCESS);
}
