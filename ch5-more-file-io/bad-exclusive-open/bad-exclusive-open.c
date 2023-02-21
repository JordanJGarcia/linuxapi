#include <sys/stat.h>
#include <fcntl.h>
#include "../../lib/tlpi-hdr.h"


int
main(int argc, char * argv[])
{
    printf("This program details an example of a bad attempt at exclusively opening a file.\n");
    printf("Aside from the long-winded use of 2 calls to OPEN(2), the code also contains a bug.\n");
    printf("Suppose that when our process first called OPEN(2), the file did not exist\n");
    printf("but by the time of the second OPEN(2) call, some other process had created the file\n");
    printf("This could happen if the kernel scheduler cut this process's time slice to give control\n");
    printf("to another process, both executing the code in this file.\n\n");
    printf("While this is rare, it is still possible; rendering this code unreliable in some way :(\n\n");

    int fd;

    // open 1: check if file exists
    fd = open(argv[1], O_WRONLY);
    if (fd != -1)
    {
        printf("[PID %ld] File \"%s\" already exists\n", (long)getpid(), argv[1]);
 
        // also notice the lack of checks for proper close!
        close(fd);
    }
    else
    {
        // failed for unexpected reason
        if (errno != ENOENT)
            errExit("open");
        else
        {
            // WINDOW FOR FAILURE
            fd = open(argv[1], O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR);
            if (fd == -1)
                errExit("open");

            printf("[PID %ld] Created file \"%s\" exclusively\n", (long)getpid(), argv[1]);

            // close
            close(fd);
        }
    }

    // exit
    exit(EXIT_SUCCESS);
}
