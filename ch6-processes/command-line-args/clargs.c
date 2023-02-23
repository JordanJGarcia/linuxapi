#include "../../lib/tlpi-hdr.h"

int
main(int argc, char * argv[])
{
    printf("This program prints out the command-line arguments\n");
    printf("stored in `char * argv[]`. So you can pass any arguments\n");
    printf("you'd like to see that happen.\n\n");

    char ** arg;

    for(arg = argv; *arg; arg++)
        puts(*arg);

    exit(EXIT_SUCCESS);
}
