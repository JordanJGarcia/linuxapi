#define _GNU_SOURCE

#include <sys/statvfs.h>
#include "../../lib/tlpi-hdr.h"
#include "../../lib/error.h"

/* static functions */
static void
info(void)
{
    printf("The statvfs() and fstatvfs() library functions obtain information about a mounted \
            file system.\n");
    printf("\nSee STATVFS(3) for more info.\n");
    printf("\nThe only difference between these two functions is how the file system is identified (path or fd).\n");
    printf("\nBoth functions return a statvfs structure containing information about the file system \
            \nin the buffer pointed to by statvfsbuf. The structure has the following form:\n");
    printf("\nstruct statvfs {\n \
   \tunsigned long f_bsize;    /* File-system block size (in bytes) */ \n \
   \tunsigned long f_frsize;   /* Fundamental file-system block size (in bytes) */ \n \
   \tfsblkcnt_t f_blocks;      /* Total number of blocks in file system (in units of 'f_frsize') */\n \
   \tfsblkcnt_t f_bfree;       /* Total number of free blocks */\n \
   \tfsblkcnt_t f_bavail;      /* Number of free blocks available to unprivileged process */\n \
   \tfsfilcnt_t f_files;       /* Total number of i-nodes */\n \
   \tfsfilcnt_t f_ffree;       /* Total number of free i-nodes */\n \
   \tfsfilcnt_t f_favail;      /* Number of i-nodes available to unprivileged process (set to 'f_ffree' on Linux) */\n \
   \tunsigned long f_fsid;     /* File-system ID */\n \
   \tunsigned long f_flag;     /* Mount flags */\n \
   \tunsinged long f_namemax;  /* Maximum length of filenames on this file system */\n \
};\n\n");
}

static int
getfsinfo(char * pathname)
{
    struct statvfs buffer;

    if (statvfs(pathname, &buffer) == -1)
    {
        LOG_ERROR(stderr, "In getfsinfo(), statvfs() call failed\n");
        return EXIT_FAILURE;
    }

    /* print file system stats */
    printf("File-system block size (in bytes): %lu\n", buffer.f_bsize);
    printf("Fundamental file-system block size (in bytes): %lu\n", buffer.f_frsize);
    printf("Size of file-system in 'f_frsize' blocks: %lu\n", (unsigned long)buffer.f_blocks);
    printf("Total number of free blocks: %lu\n", (unsigned long)buffer.f_bfree);
    printf("Free blocks available to unprivileged process: %lu\n", (unsigned long)buffer.f_bavail);
    printf("Total number of i-nodes: %lu\n", (unsigned long)buffer.f_files);
    printf("Total number of free i-nodes: %lu\n", (unsigned long)buffer.f_ffree);
    printf("Number of free i-nodes available to unprivileged process: %lu\n", (unsigned long)buffer.f_favail);
    printf("File-system ID: %lu\n", buffer.f_fsid);
    printf("Mount flags:\n");
#ifdef _GNU_SOURCE
    if (buffer.f_flag & ST_MANDLOCK)
        printf("\tST_MANDLOCK\n");
    if (buffer.f_flag & ST_NOATIME)
        printf("\tST_NOATIME\n");
    if (buffer.f_flag & ST_NODEV)
        printf("\tST_NODEV\n");
    if (buffer.f_flag & ST_NODIRATIME)
        printf("\tST_NODIRATIME\n");
    if (buffer.f_flag & ST_NOEXEC)
        printf("\tST_NOEXEC\n");
    if (buffer.f_flag & ST_RELATIME)
        printf("\tST_RELATIME\n");
    if (buffer.f_flag & ST_SYNCHRONOUS)
        printf("\tST_SYNCHRONOUS\n");
#endif
    if (buffer.f_flag & ST_NOSUID)
        printf("\tST_NOSUID\n");
    if (buffer.f_flag & ST_RDONLY)
        printf("\tST_RDONLY\n");
    printf("Max length of filenames: %lu\n", buffer.f_namemax);

    return EXIT_SUCCESS;
}


/* main */
int
main(int argc, char * argv[])
{
    info();
    getfsinfo("/");
    exit(EXIT_SUCCESS);
}


