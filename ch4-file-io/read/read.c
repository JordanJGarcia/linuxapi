#include <unistd.h>
#include "../../lib/tlpi-hdr.h"

#define MAX_READ 20

int main(int argc, char * argv[])
{
    printf("See READ(2) for more info on this system call\n\n");
    printf("Prototype - ssize_t read(int fd, void *buf, size_t count);\n\n");
    printf("It is important to note, this reads binary format, so no null-terminator is automatically appended\n");
    printf("If we want the string to be null-terminated, we must do so explicitly\n\n");

    char buf[MAX_READ + 1];
    ssize_t numRead;

    numRead = read(STDIN_FILENO, buf, MAX_READ);
    if (numRead == -1)
        errExit("read");

    buf[numRead] = '\0';
    printf("The input data was: %s\n", buf);

    exit(EXIT_SUCCESS);
}
