#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "../../lib/tlpi-hdr.h"

#define BUF_SIZE 256

static char DATA[BUF_SIZE] = "This is some\ndata\nthat will be written to the\nFILE!";
static char OUTFILE[] = "lseek-example-output.txt";

int
main(int argc, char * argv[])
{
    printf("This program demonstrates the use of the LSEEK(2) system call\n");
    printf("The LSEEK(2) system call adjusts the file offset of the open file\n");
    printf("referred to by the file descriptor <fd>, according to the values\n");
    printf("specified in OFFSET and WHENCE\n\n");
    printf("WHENCE indicates the base point from which OFFSET (a value in bytes)\n");
    printf("is to be interpreted, and is one of the following values:\n");
    printf("SEEK_SET: from beginning of the file\n");
    printf("SEEK_CUR: relative to current file offset\n");
    printf("SEEK_END: next byte after the last byte of the file\n\n");
    printf("Note: we cannot apply LSEEK() to all types of files. It does not work on\n");
    printf("pipes, FIFOs, sockets, or terminals\n\n");

    int fd, numRead;
    off_t current, end;
    char buf[20] = "";

    // open the file
    fd = open(OUTFILE, O_RDWR | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP);
    if (fd == -1)
        errExit("open");

    // write data to file
    if ((write(fd, DATA, (size_t)strlen(DATA))) != (int)strlen(DATA))
        errExit("Could not write DATA to file: %s\n", OUTFILE);

    // get size of file
    current = lseek(fd, 0, SEEK_SET);
    end = lseek(fd, 0, SEEK_END);

    printf("Size of file \"%s\": %d bytes\n", OUTFILE, (int)(end - current));

    // check current and end offset
    printf("current = %d\n", (int)current);
    printf("end = %d\n", (int)end);

    // traverse file
    current = lseek(fd, -1, SEEK_END); // last byte of file
    numRead = read(fd, buf, 1);
    if (numRead != 1)
        errExit("read");

    printf("Last byte of the file: %d (%c)\n", (int)current, buf[0]);

    // reset buffer
    memset(buf, 0, 20);

    // 10 bytes prior to current location
    current = lseek(fd, -10, SEEK_CUR); 
    numRead = read(fd, buf, 10);
    if (numRead != 10)
        errExit("read");

    printf("Current byte offset: %d (%s)\n", (int)current, buf);

    // close the file
    if (close(fd) == -1)
        errExit("close");

    exit(EXIT_SUCCESS);
}
