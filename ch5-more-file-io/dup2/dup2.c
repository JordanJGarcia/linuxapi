#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "../../lib/tlpi-hdr.h"


int
main(int argc, char * argv[])
{
    printf("See DUP(2) for more info.\n");
    printf("Prototype:\tint dup2(int oldfd, int newfd);\n");
    printf("the DUP2() system call makes a duplicate of the file descriptor given in <oldfd>\n");
    printf("using the descriptor number supplied in <newfd>\n");
    printf("\nIf the file descriptor specified in <newfd> is already open, dup2() closes it first\n");
    printf("Any error that occurs during this close is silently ignored.\n");

    int fd;

    // duplicate stdout to stderr
    printf("\nLets duplicate STDOUT to STDERR:\n");
    printf("fd = dup2(STDOUT_FILENO, STDERR_FILENO);\n");
    fd = dup2(STDOUT_FILENO, STDERR_FILENO);
    if (fd == -1)
        errExit("dup2");

    printf("fd = %d\n", fd);

    exit(EXIT_SUCCESS);
}
