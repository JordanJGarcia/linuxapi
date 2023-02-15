#include <sys/stat.h>
#include <fcntl.h>
#include "../../lib/tlpi-hdr.h"

#define MAX_FLAGS 23

int
main(int argc, char * argv[])
{
    char * file = "startup";
    int fd, i = 0;

    char * flags[MAX_FLAGS] = { 
        "*** file access mode flags ***",
        "O_RDONLY       - open for reading only",
        "O_WRONLY       - open for writing only",
        "O_RDWR         - open for reading and writing",
        " ",
        "*** file creation flags ***",
        "O_CLOEXEC      - set the close-on-exec flag (since Linux 2.6.23)",
        "O_CREAT        - create file if it doesn't already exist",
        "O_DIRECTORY    - fail if pathname is not a directory",
        "O_EXCL         - with O_CREAT: create file exclusively",
        "O_LARGEFILE    - used on 32-bit systems to open large files",
        "O_NOCTTY       - don't let pathname become the controlling terminal",
        "O_NOFOLLOW     - don't dereference symbolic links",
        "O_TRUNC        - truncate existing file to zero length",
        " ",
        "*** open file status flags ***",
        "O_APPEND       - writes are always appended to end of file",
        "O_ASYNC        - generate a signal when I/O is possible",
        "O_DIRECT       - file I/O bypasses buffer cache",
        "O_DSYNC        - provide synchronized I/O data integrity (since Linux 2.6.33)",
        "O_NOATIME      - don't update file last access time on read() (since Linux 2.6.8)",
        "O_NONBLOCK     - open in nonblocking mode",
        "O_SYNC         - make file writes synchronous"
    };

    printf("See \"open.c\" for the source code\n\n");
    printf("See OPEN(2) for more info on this system call\n\n");
    printf("Prototype - int open(const char *pathname, int flags, mode_t mode);\n\n");
    printf("Valid flags for \"flags\" parameter: \n");
    for(; i < MAX_FLAGS; i++)
        printf("\t%s\n", flags[i]);

    // open existing file for reading
    fd = open(file, O_RDONLY);
    if (fd == -1)
        errExit("open");

    printf("\n\"%s\" opened for reading\n", file);

    if (close(fd) == -1)
        errExit("close");

    // open a new or existing file for reading and writing, truncating to zero
    // bytes; file permissions read+write for owner, nothing for all others
    file = "myfile";
    fd = open(file, O_RDWR | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
    if (fd == -1)
        errExit("open");

    printf("\"%s\" opened for reading & writing, truncated to zero bytes, with perms \"rw-------\"\n", file);

    if (close(fd) == -1)
        errExit("close");

    // open a new or existing file for writing; writes should always 
    // append to end of file
    file = "w.log";
    fd = open(file, O_WRONLY | O_CREAT | O_APPEND, S_IRUSR | S_IWUSR);
    if (fd == -1)
        errExit("open");

    printf("\"%s\" opened for writing, writes append to end of file, with perms \"rw-------\"\n", file);

    if (close(fd) == -1)
        errExit("close");

    exit(EXIT_SUCCESS);
}
