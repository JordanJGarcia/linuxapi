// convert all of the relevant 32-bit functions
// and data types into their 64-bit counterparts
#define _FILE_OFFSET_BITS 64

#include <sys/stat.h>
#include <fcntl.h>
#include "../../../lib/tlpi-hdr.h"

int
main(int argc, char * argv[])
{
    int fd;
    off_t off;

    if (argc != 3 || strcmp(argv[1], "--help") == 0 || strcmp(argv[1], "-h") == 0)
        usageErr("%s <pathname> <offset>\n*** This is meant to be used to create a large file (>2gb)", argv[0]);

    fd = open(argv[1], O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
    if (fd == -1)
        errExit("open");

    off = atoll(argv[2]);
    if (lseek(fd, off, SEEK_SET) == -1)
        errExit("lseek");

    if (write(fd, "\ntest", 5) == -1)
        errExit("write");

    exit(EXIT_SUCCESS);
}
