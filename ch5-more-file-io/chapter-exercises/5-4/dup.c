#include <sys/stat.h>
#include <fcntl.h>
#include "../../../lib/tlpi-hdr.h"

// prototypes
int dup(int oldfd);
int dup2(int oldfd, int newfd);


// main
int
main(int argc, char * argv[])
{
    printf("This program attempts to implement dup() and dup2()\n");
    printf("No user input required for this one, main() will just test it\n");

    int dupfd, dup2fd;

    printf("\ndupfd = dup(STDOUT_FILENO);\n");
    dupfd = dup(STDOUT_FILENO);
    printf("dupfd = %d\n", dupfd);
    if (dupfd == -1)
        errExit("dup()");

    printf("\ndup2fd = dup2(STDOUT_FILENO, 4);\n");
    dup2fd = dup2(STDOUT_FILENO, 4);
    printf("dup2fd = %d\n", dup2fd);
    if (dup2fd == -1)
        errExit("dup2()");

    printf("\ndup2fd = dup2(STDOUT_FILENO, 2);\n");
    dup2fd = dup2(STDOUT_FILENO, 2);
    printf("dup2fd = %d\n", dup2fd);
    if (dup2fd == -1)
        errExit("dup2()");

    printf("\ndup2fd = dup2(STDOUT_FILENO, 3);\n");
    dup2fd = dup2(STDOUT_FILENO, 3);
    printf("dup2fd = %d\n", dup2fd);
    if (dup2fd == -1)
        errExit("dup2()");

    printf("\ndup2fd = dup2(STDOUT_FILENO, STDOUT_FILENO); // this should do nothing and return 1\n");
    dup2fd = dup2(STDOUT_FILENO, STDOUT_FILENO);
    printf("dup2fd = %d\n", dup2fd);
    if (dup2fd == -1)
        errExit("dup2()");

    printf("\ndup2fd = dup2(10, 3); // this should fail with errno set to EBADF\n");
    dup2fd = dup2(10, 3);
    printf("dup2fd = %d\n", dup2fd);
    if (dup2fd == -1)
        errExit("dup2()");

    exit(EXIT_SUCCESS);
}


// function defintions
int
dup(int oldfd)
{
    int newfd = -1;

    // verify oldfd is valid
    if (fcntl(oldfd, F_GETFL) == -1)
        return newfd;

    // duplicate oldfd (smallest >= 0)
    newfd = fcntl(oldfd, F_DUPFD, 0);

    // return newfd
    return newfd;
}


int
dup2(int oldfd, int newfd)
{
    int res = -1;

    // check if oldfd is valid
    if (fcntl(oldfd, F_GETFL) == -1)
    {
        // ensure errno is set properly
        errno = EBADF;
        return -1;
    }

    // check if oldfd = newfd
    if (oldfd == newfd)
        return newfd;

    // close newfd if already open
    if (fcntl(newfd, F_GETFL) != -1)
        if (close(newfd) == -1)
            return -1;

    // duplicate oldfd to newfd
    res = fcntl(oldfd, F_DUPFD, newfd);
    if (res != newfd)
        return -1;
    
    return res;
}
