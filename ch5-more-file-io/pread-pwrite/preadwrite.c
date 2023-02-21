#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include "../../lib/tlpi-hdr.h"

#define BUF_SIZE 1024

static char BUFFER[BUF_SIZE];

int
main(int argc, char * argv[])
{
    // usage
    if (argc != 3 || strcmp(argv[1], "--help") == 0 || strcmp(argv[1], "-h") == 0)
        usageErr("%s input-file output-file\n", argv[0]);

    printf("See PREAD(2) for more info\n");
    printf("Prototypes:\n");
    printf("\tssize_t pread(int fd, void *buf, size_t count, off_t offset);\n");
    printf("\tssize_t pwrite(int fd, const void *buf, size_t count, off_t offset);\n");

    printf("\nThe PREAD() nad PWRITE() system calls operate just like READ(2) and WRITE(2),\n");
    printf("except that the file I/O is performed at the location specified by <offset>, rather\n");
    printf("than at the current file offset. And the file offset is left unchanged by these calls\n");

    printf("Calling PREAD(2) is equivalent to atomically performing an LSEEK() to change the offset,\n"); 
    printf("then a READ(), then another LSEEK() to reset the offset back to the original pos\n\n");

    ssize_t numRead, numWritten;
    int fdIn, fdOut, flags = O_RDWR | O_CREAT | O_TRUNC;
    mode_t perms = S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH; //rw-r--r--

    // open input file
    fdIn = open(argv[1], O_RDONLY);
    if (fdIn == -1)
        errExit("opening input-file");

    // open output file
    fdOut = open(argv[2], flags, perms); 
    if (fdOut == -1)
        errExit("opening output-file");

    printf("Reading from \"%s\", writing to \"%s\"\n", argv[1], argv[2]);

    // read data 10 bytes in
    numRead = pread(fdIn, BUFFER, BUF_SIZE, 10); 
    if (numRead == -1)
        errExit("pread");

    // write data to output-file from the beginning
    numWritten = pwrite(fdOut, BUFFER, numRead, 0);
    if (numWritten != numRead)
        errExit("Not all that was read was written");

    // write data again to output-file, at offset 100
    // note that this will not create file holes
    numWritten = pwrite(fdOut, BUFFER, numRead, 100);
    if (numWritten != numRead)
        errExit("Not all that was read was written");

    // close files
    if (close(fdIn) == -1)
        errExit("closing input file");

    if (close(fdOut) == -1)
        errExit("closing output file");

    exit(EXIT_SUCCESS);
}
