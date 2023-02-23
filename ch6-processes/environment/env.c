#include "../../lib/tlpi-hdr.h"

// globals
extern char ** environ;

int
main(int argc, char * argv[])
{
    printf("Within a C program, the environment list can be accessed using the global\n");
    printf("variable `char ** environ`.\n");
    printf("The C run-time startup code defines this variable and assigns the location\n");
    printf("of the environment list to it.\n\n");

    char ** var;

    for(var = environ; *var; var++)
        puts(*var);

    exit(EXIT_SUCCESS);
}
