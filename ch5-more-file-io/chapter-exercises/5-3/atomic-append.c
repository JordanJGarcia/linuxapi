#define _FILE_OFFSET_BITS 64

#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include "../../../lib/tlpi-hdr.h"

// macros
#define BUF_SIZE 1

// globals
char BUFFER[BUF_SIZE] = "x";

// prototypes
ssize_t writeOneByte(int fd);

int
main(int argc, char * argv[])
{
    // usage
    if (argc < 3 || argc > 4 || strcmp(argv[1], "--help") == 0 || strcmp(argv[1], "-h") == 0)
        usageErr("%s <filename> <num-bytes> [x]");

    // local variables
    int fd, flags;
    ssize_t numWritten = 0;
    long numBytes = atol(argv[2]);
    mode_t perms = S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH; // rw-r--r--

    // set file opening flags
    if (argc == 4)
    {
        if (strcmp(argv[3], "x") != 0)
            usageErr("3rd argument should be a literal \"x\"");
        else
            flags = O_RDWR | O_CREAT;
    }
    else
        flags = O_RDWR | O_CREAT | O_APPEND;

    // open file
    fd = open(argv[1], flags, perms);
    if (fd == -1)
        errExit("opening file %s", argv[1]);

    // write one byte at a time to file
    while(numWritten < numBytes)
        numWritten += writeOneByte(fd);

    // close file
    if (close(fd) == -1)
        errExit("close");
    
    // done
    exit(EXIT_SUCCESS);
}

// function definitions
ssize_t
writeOneByte(int fd)
{
    // local variables
    int flags;
    off_t offset;
    ssize_t numWritten = 0;

    // get flags for file descriptor fd
    flags = fcntl(fd, F_GETFL);
    if (flags == -1)
        errExit("fcntl");

    // check if opened with O_APPEND
    if (flags & O_APPEND)
    {
        // write byte
        numWritten = write(fd, BUFFER, BUF_SIZE);
        if (numWritten == -1)
            errExit("write - writeOneByte");

        return numWritten;
    }

    // otherwise lseek first
    offset = lseek(fd, 0, SEEK_END);
    if (offset == -1)
        errExit("lseek - writeOneByte");

    // write byte
    numWritten = write(fd, BUFFER, BUF_SIZE);
    if (numWritten == -1)
        errExit("write - writeOneByte");

    return numWritten;
}
