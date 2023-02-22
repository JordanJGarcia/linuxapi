#include <sys/stat.h>
#include <fcntl.h>
#include "../../../lib/tlpi-hdr.h"

// this is stored as static global variable
// so that it is only visible to this file
// and it is initialized to zero's
#define BUF_SIZE 1024
static char BUFFER[BUF_SIZE];

int
main(int argc, char * argv[])
{
    // usage
    if (argc != 2 || strcmp(argv[1], "--help") == 0 || strcmp(argv[1], "-h") == 0)
        usageErr("%s <output-file>", argv[0]);

    // local vars
    off_t offset;
    ssize_t numRead, numWritten;
    int fd, flags = O_RDWR | O_APPEND;

    // open file
    fd = open(argv[1], flags);
    if (fd == -1)
        errExit("opening %s", argv[1]);

    // Request data to be written to the file
    printf("Enter some data you want to write to \"%s\" (<= 1024 chars): ", argv[1]);

    // have to flush stdout buffer so it prints before reading stdin
    fflush(stdout);

    // read from stdin
    numRead = read(STDIN_FILENO, BUFFER, BUF_SIZE);
    if (numRead == -1)
        errExit("could not read from STDIN");

    // seek to beginning of file
    offset = lseek(fd, 0, SEEK_SET);
    if (offset == -1)
        errExit("lseek");

    // write to output file
    numWritten = write(fd, BUFFER, numRead);
    if (numWritten == -1)
        errExit("unable to write buffer to \"%s\"", argv[1]);

    if (numWritten != numRead)
        fatal("Not all that was read was written");

    // close file
    if (close(fd) == -1)
        errExit("closing output file");

    exit(EXIT_SUCCESS);
}
