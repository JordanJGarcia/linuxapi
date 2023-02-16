#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "../../lib/tlpi-hdr.h"

#define BUF_SIZE 1024 // 1 kb

int
main(int argc, char * argv[])
{
    char buffer[BUF_SIZE + 1];
    ssize_t numRead;
    char * outFile = "what-you-wrote.txt";
    int fd;
    mode_t filePerms = S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH; //rw-r--r--

    printf("Welcome!\nYou may type anything you wish to be printed to the file \"%s\"\n", outFile);
    printf("Type \"exit\" to end the program\n\n");
    printf("*** Note that each line should be 1024 characters or less in length ***\n\n");

    // open output file
    fd = open(outFile, O_WRONLY | O_CREAT | O_TRUNC, filePerms);
    if (fd == -1)
        errExit("opening file %s", outFile);

    // read input from user
    while((numRead = read(STDIN_FILENO, buffer, BUF_SIZE - 1)) > 0)
    {
        // add null-terminator for strcmp()
        buffer[numRead] = '\0';

        // check for exit
        // user should enter <return> after typing exit (or any line)
        // so include newline in this
        if (strcmp(buffer, "exit\n") == 0)
            break;

        // write to output file
        if(write(fd, buffer, numRead) != numRead)
            fatal("write() returned error or partial write occured");
    }

    // error check read
    if (numRead == -1)
        errExit("read");

    // close output file
    if (close(fd) == -1)
        errExit("close output file %s", outFile);


    exit(EXIT_SUCCESS);
}
