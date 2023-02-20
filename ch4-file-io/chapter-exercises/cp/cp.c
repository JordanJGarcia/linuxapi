/*
 * Problem 4-2:
 *
 * Write a program like cp that, when used to copy a regular file that contains
 * holes (sequences of null bytes), also creates corresponding holes in the target file.
 *
*/

#include <sys/stat.h>
#include <fcntl.h>
#include <libgen.h>
#include "../../../lib/tlpi-hdr.h"

// macros
#define BUF_SIZE 1024 // 1kb

// prototyes
int copy(char * inputPath, char * outputPath);

int
main(int argc, char * argv[])
{
    // usage
    if (argc < 3 || strcmp(argv[1], "--help") == 0 || strcmp(argv[1], "-h") == 0)
        usageErr("%s <input file(s)> <dest>\n", argv[0]);

    // local vars
    char * dest;
    int i, copyingToDir = 0;
    struct stat outputFileStats;

    // get inode stats for output path
    if (stat(argv[argc - 1], &outputFileStats) == -1)
    {
        if (errno != ENOENT)
            errExit("stat output file \"%s\"", argv[argc - 1]);
    }
    else
    {
        // check if output file is directory
        if (S_ISDIR(outputFileStats.st_mode))
            copyingToDir = 1;
    }

    if (!copyingToDir && argc > 3)
        usageErr("You cannot copy multiple input files to a single output file that is not a directory\n");

    for(i = 1; i < argc - 1; i++)
    {
        // will have to create filename to copy to, if 
        // destination is a directory
        if (copyingToDir)
        {
            dest = (char *)calloc(1, strlen(basename(argv[i])) + strlen(argv[argc - 1]) + 1);
            if (dest == NULL)
                errExit("allocating space for dest");

            strcpy(dest, argv[argc - 1]);
            if (dest[strlen(dest-1)] != '/')
                strcat(dest, "/");

            strcat(dest, basename(argv[i]));
            
            // copy input to output
            copy(argv[i], dest);

            // free allocated memory
            free(dest);
        }
        else
            copy(argv[i], argv[argc - 1]);

    }

    exit(EXIT_SUCCESS);
}


// functions
int copy(char * inputPath, char * outputPath)
{
    int inFd, outFd;
    ssize_t numRead, numWritten;
    struct stat inputFileStats;
    char buffer[BUF_SIZE];

    // get inode stats for input file
    if (stat(inputPath, &inputFileStats) == -1)
        errExit("stat input file \"%s\"", inputPath);

    // copy input file permissions for output file
    mode_t perms = inputFileStats.st_mode & 07777;

    // open output file
    outFd = open(outputPath, O_RDWR | O_CREAT | O_TRUNC, perms); 
    if (outFd == -1)
        errExit("opening output file");

    // open input file
    inFd = open(inputPath, O_RDONLY);
    if (inFd == -1)
        errExit("opening input file");

    // copy input to output, 1kb at a time
    while((numRead = read(inFd, buffer, BUF_SIZE)) > 0)
        if ((numWritten = write(outFd, buffer, numRead)) != numRead)
            errExit("Not all that was read, was written");

    // error check
    if (numRead == -1)
        errExit("read");

    // close files
    if (close(inFd) == -1)
        errExit("closing input file");

    if (close(outFd) == -1)
        errExit("closing output file");

    return EXIT_SUCCESS;
}
