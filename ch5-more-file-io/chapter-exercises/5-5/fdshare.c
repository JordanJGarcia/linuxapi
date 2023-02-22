#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include "../../../lib/tlpi-hdr.h"

// prototypes
int checkFlags(int fd1, int fd2);
int checkOffsets(int fd1, int fd2);

// main
int
main(int argc, char * argv[])
{
    // usage
    if (argc != 2 || strcmp(argv[1], "--help") == 0 || strcmp(argv[1], "-h") == 0)
        usageErr("%s <pathname>", argv[0]);

    int fd1, fd2, flags = O_RDWR | O_CREAT | O_TRUNC;
    mode_t perms = S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH; // rw-r--r--

    // open file
    fd1 = open(argv[1], flags, perms);
    if (fd1 == -1)
        errExit("open()");

    printf("File \"%s\" opened with file descriptor (%d)\n", argv[1], fd1);

    // duplicate fd1
    fd2 = dup(fd1);
    if (fd2 == -1)
        errExit("dup()");

    printf("File \"%s\" duplicated with file descriptor (%d)\n", argv[1], fd2);
    printf("Now checking if they have the same flags & offsets...\n");

    if (checkFlags(fd1, fd2) == EXIT_SUCCESS)
        printf("Both file descriptors have the same flags!\n");
    else
        printf("Hmmm... both file descriptors have different flags\n");

    if (checkOffsets(fd1, fd2) == EXIT_SUCCESS)
        printf("Both file descriptors have the same offsets!\n");
    else
        printf("Hmmm... both file descriptors have different offsets\n");

    exit(EXIT_SUCCESS);
}

// function definitions
int
checkFlags(int fd1, int fd2)
{
    int fd1flags, fd2flags;

    // get flags for respective file descriptors
    fd1flags = fcntl(fd1, F_GETFL);
    if (fd1flags == -1)
        errExit("checkFlags() - fd1flags");

    fd2flags =  fcntl(fd2, F_GETFL);
    if (fd2flags == -1)
        errExit("checkFlags() - fd2flags");

    // compare
    if (fd1flags != fd2flags)
        return EXIT_FAILURE;

    return EXIT_SUCCESS;
}

int
checkOffsets(int fd1, int fd2)
{
    off_t fd1offset, fd2offset;

    // grab offsets for each file descriptor
    fd1offset = lseek(fd1, 0, SEEK_CUR);
    if (fd1offset == -1)
        errExit("checkOffsets() - fd1offset");

    fd2offset = lseek(fd2, 0, SEEK_CUR);
    if (fd2offset == -1)
        errExit("checkOffsets() - fd2offset");

    // compare
    if (fd1offset != fd2offset)
        return EXIT_FAILURE;

    return EXIT_SUCCESS;
}
