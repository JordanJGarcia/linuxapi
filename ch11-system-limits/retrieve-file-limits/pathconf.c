#include "../../lib/tlpi-hdr.h"
#include <unistd.h>

static void
fpathconfPrint(const char * msg, int fd, int name)
{
    long lim;

    errno = 0;
    lim = fpathconf(fd, name);

    // call succeeded
    if (lim != -1)
        printf("%s %ld\n", msg, lim);
    else
    {
        // limit indeterminate
        if (errno == 0)
            printf("%s (indeterminate)\n", msg);
        else
            errExit("fpathconf %s", msg);
    }
}


int
main(int argc, char * argv[])
{
    printf("See FPATHCONF(3) for more info\n");

    printf("\nPrototypes:\n");
    printf("\tlong fpathconf(int fd, int name);\n");
    printf("\tlong pathconf(const char *path, int name);\n");

    printf("\nThe only difference between these 2 functions is that one uses a pathname,\n");
    printf("and the other uses a file descriptor\n");

    printf("\nThe <name> argument is one of hte _PC_* constants defined in <unistd.h>\n");

    printf("\nReturn value is same as sysconf() and we can distinguish between an indeterminate\n");
    printf("value and error the same way as we do for sysconf() (by setting errno to 0 prior)\n\n");

    fpathconfPrint("_PC_NAME_MAX:   ", STDIN_FILENO, _PC_NAME_MAX);
    fpathconfPrint("_PC_PATH_MAX:   ", STDIN_FILENO, _PC_PATH_MAX);
    fpathconfPrint("_PC_PIPE_BUF:   ", STDIN_FILENO, _PC_PIPE_BUF);

    exit(EXIT_SUCCESS);
}
