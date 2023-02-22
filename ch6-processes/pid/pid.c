#include <unistd.h>
#include "../../lib/tlpi-hdr.h"

int
main(int argc, char * argv[])
{
    printf("See GETPID(2) for more info\n");
    printf("\nThe Linux kernel limits process IDs to being 1 less than the value in\n");
    printf("`/proc/sys/kernel/pid_max`. It may be different for 32/64-bit systems.\n");
    printf("\nWhen a new process is created, it is assigned the next sequentially available process ID.\n");
    printf("Each time the limit in the mentioned file is reached, the kernel resets its process ID counter to 300.\n");

    printf("\nCurrent process ID: %ld\n", (long)getpid());
    printf("Current process's parent process ID: %ld\n", (long)getppid());

    exit(EXIT_SUCCESS);
}
