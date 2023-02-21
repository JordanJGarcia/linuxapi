#include <sys/uio.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "../../lib/tlpi-hdr.h"

#define STR_SIZE 25

int
main(int argc, char * argv[])
{
    // usage
    if (argc != 3 || strcmp(argv[1], "--help") == 0 || strcmp(argv[1], "-h") == 0)
        usageErr("%s input-file output-file\n", argv[0]);

    // system call information
    printf("See READV(2) for more info\n");
    printf("Prototypes:\n");
    printf("\tssize_t readv(int fd, const struct iovec *iov, int iovcnt);\n");
    printf("\tssize_t writev(int fd, const struct iovec *iov, int iovcnt);\n");

    printf("\nThese calls perform scatter-gather I/O\n");
    printf("Instead of accepting a single buffer of data to be read or written,\n");
    printf("these functions transfer multiple buffers of data in a single system call\n");
    printf("The set of buffers to be transferred is defined by the array <iov>\n");

    printf("\nThe READV(2) systemc all performs scatter input: \n");
    printf("It reads a contiguous sequence of bytes from the file referred to by <fd>\n");
    printf("and places (\"scatters\") these bytes into the buffers specified by <iov>\n\n");

    int fdIn, fdOut;
    struct iovec iov[3];
    struct stat buffer1;
    int buffer2;
    char buffer3[STR_SIZE];
    ssize_t numRead, numWritten, totRequired = 0;

    // open files
    fdIn = open(argv[1], O_RDONLY);
    if (fdIn == -1)
        errExit("opening input-file");

    fdOut = open(argv[2], O_RDWR | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
    if (fdOut == -1)
        errExit("opening output-file");

    // set up iovec elements
    iov[0].iov_base = &buffer1;
    iov[0].iov_len = sizeof(struct stat);
    totRequired += iov[0].iov_len;

    iov[1].iov_base = &buffer2;
    iov[1].iov_len = sizeof(buffer2);
    totRequired += iov[1].iov_len;

    iov[2].iov_base = buffer3;
    iov[2].iov_len = STR_SIZE;
    totRequired += iov[2].iov_len;

    // scatter input into iovec buffers
    numRead = readv(fdIn, iov, 3);
    if (numRead == -1)
        errExit("readv");

    if (numRead < totRequired)
        printf("Read fewer bytes than requested, garbage data may be output!!\n");

    printf("total bytes requested: %ld, bytes read: %ld\n", (long)totRequired, (long)numRead);
    printf("\nNow writing to output file \"%s\"", argv[2]);

    // write buffers to file
    numWritten = writev(fdOut, iov, 3);
    if (numWritten == -1)
        errExit("writev");

    if (numWritten != numRead)
        fatal("Not all that was read was written\n");

    // close files
    if (close(fdIn) == -1)
        errExit("closing input file");

    if (close(fdOut) == -1)
        errExit("closing output file");

    exit(EXIT_SUCCESS);
}
