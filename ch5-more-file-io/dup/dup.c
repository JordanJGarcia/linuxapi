#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include "../../lib/tlpi-hdr.h"

int
main(int argc, char * argv[])
{
    printf("See DUP(2) for more information.\n");
    printf("Prototype:\tint dup(int oldfd);\n");
    printf("DUP(2) takes <oldfd>, an open file descriptor, and returns a\n");
    printf("new descriptor that refers to the same open file description\n");
    printf("the file description means the system-wide open file table. See pg 95 of the textbook.\n\n");

    printf("DUP(2) will use the lowest-available file descriptor.\n");
    printf("So if we call `newfd = dup(STDOUT_FILENO)`, and we assume the only open\n");
    printf("file descriptors are the standard ones 0,1,2 opened by the shell; then newfd should equal 3\n");
    printf("Lets verify...\n\n");

    int newfd;

    // duplicate stdout
    printf("newfd = dup(STDOUT_FILENO);\n");
    newfd = dup(STDOUT_FILENO);

    // check it value
    printf("newfd: %d\n", newfd);

    // troubleshoot message
    printf("\nIf newfd is not 3, then run `ls -l /proc/$$/fd` in your current shell to see\n");
    printf("which file descriptors are currently opened\n");

    exit(EXIT_SUCCESS);
}
