#include <sys/stat.h>
#include <fcntl.h>
#include "../../../lib/tlpi-hdr.h"


int
main(int argc, char * argv[])
{
    printf("One use of FCNTL(2) is to retrieve or modify the access mode and open file status flags of an open file\n");
    printf("These are the values set by the \"flags\" argument specified in the call to OPEN(2)\n\n");
    printf("See the source code of this program for details on how :)\n\n");

    int fd, flags, accessMode;

    // open file
    fd = open(argv[1], O_RDWR | O_CREAT | O_EXCL | O_SYNC, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
    
    // error check
    if (fd == -1)
    {
        if (errno == EEXIST)
            printf("O_CREAT | O_EXCL flags were used to open the file. It must not exist already\n");

        errExit("open");
    }

    printf("File: \"%s\"\n\n", argv[1]);

    // get flags
    flags = fcntl(fd, F_GETFL);
    if (flags == -1)
        errExit("fcntl");

    // test if file was opened for synchronizezd writes
    if (flags & O_SYNC)
        printf("flags & O_SYNC: PASS\n");

    // access mode
    printf("\nAccess mode checking is a bit more complex\n");
    printf("since the O_RDONLY(0), O_WRONLY(1), and O_RDWR(2) constants don't correspond\n");
    printf("to single bits in the open file status flags.\n");
    printf("\nTherefore, to make this check, we mask the flags value with the constant O_ACCMODE\n");
    printf("and then test for equality with one of the constants:\n\n");
    printf("accessMode = flags & O_ACCMODE;\n");

    accessMode = flags & O_ACCMODE;

    // test if file was opened for reading and writing
    if (accessMode & O_RDWR)
        printf("accessMode & O_RDWR: PASS\n");
    else
        printf("accessMode & O_RDWR: FAIL\n");

    // test if file was opened for writing
    if (accessMode & O_WRONLY)
        printf("accessMode & O_WRONLY: PASS\n");
    else
        printf("accessMode & O_WRONLY: FAIL\n");

    // test if file was opened for reading
    if (accessMode & O_RDONLY)
        printf("accessMode & O_RDONLY: PASS\n");
    else
        printf("accessMode & O_RDONLY: FAIL\n");

    // set the flags
    printf("\nWe can use FCNTL(2) to set some flags on an open file descriptor\n");
    printf("The flags we can set are O_APPEND, O_NONBLOCK, O_NOATIME, O_ASYNC, and O_DIRECT\n");
    printf("Attempts to modify other flags are ignored\n\n");

    if (flags & O_APPEND)
        printf("flags & O_APPEND: PASS\n");
    else
        printf("flags & O_APPEND: FAIL\n");

    printf("\nSetting O_APPEND flag (flags |= O_APPEND):\n");
    flags |= O_APPEND;
    printf("Use FCNTL(2) to set them on the file descriptor\n");
    printf("fcntl(fd, F_SETFL, flags)\n\n");

    // set on file descriptor
    if (fcntl(fd, F_SETFL, flags) == -1)
        errExit("fcntl");

    // re-retrieve flags from file descriptor
    flags = fcntl(fd, F_GETFL);

    if (flags & O_APPEND)
        printf("flags & O_APPEND: PASS\n");
    else
        printf("flags & O_APPEND: FAIL\n");

    // close file
    if (close(fd) == -1)
        errExit("close");

    exit(EXIT_SUCCESS);
}
