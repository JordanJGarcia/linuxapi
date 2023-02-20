/* 
 * Problem 4-1:
 *
 * Implement tee using I/O system calls. By Default, tee overwrites
 * any existing file with the given name. Implement the -a command-line
 * option (tee -a file), which causes tee to append text to the end of
 * a file if it already exists.
 *
*/

#include <sys/stat.h>
#include <fcntl.h>
#include "../../../lib/tlpi-hdr.h"

#define BUF_SIZE 1024 // 1kb 

static char BUFFER[BUF_SIZE];

int
main(int argc, char * argv[])
{
    // usage check
    if (argc < 2  || argc > 3 || strcmp(argv[1], "--help") == 0)
        usageErr("%s [-a,--append] <file>\n", argv[0]);

    // local variables
    int fd, flags;
    ssize_t numRead, numWritten;
    mode_t perms = S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH; // rw-r--r--


    // check if appending to file
    if (strcmp(argv[1], "-a") == 0 || strcmp(argv[1], "--append") == 0)
    {
        if (argc == 2)
            usageErr("%s [-a,--append] <file>\n", argv[0]);

        flags = O_RDWR | O_CREAT | O_APPEND;
    }
    else if (argc > 2)
        usageErr("%s [-a,--append] <file>\n", argv[0]);
    else
        flags = O_RDWR | O_CREAT | O_TRUNC;

    // open the file
    fd = open(argv[argc-1], flags, perms);
    if (fd == -1)
        errExit("open");

    // read input
    while((numRead = read(STDIN_FILENO, BUFFER, BUF_SIZE)) > 0)
    {
        // write to stdout
        numWritten = write(STDOUT_FILENO, BUFFER, numRead);
        if (numWritten != numRead)
            errExit("STDOUT - Did not write all that was read");

        // write to file
        numWritten = write(fd, BUFFER, numRead);
        if (numWritten != numRead)
            errExit("FILE - Did not write all that was read");
    }

    // exit
    exit(EXIT_SUCCESS);
}
