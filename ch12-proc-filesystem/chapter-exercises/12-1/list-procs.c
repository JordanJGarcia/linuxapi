#include <fcntl.h>
#include <sys/types.h>
#include <dirent.h>
#include <ctype.h>
#include <sys/stat.h>
#include "../../../lib/tlpi-hdr.h"
#include "../../../lib/error.h"
#include "../../../ch8-users-groups/chapter-examples/8-1/ugid-functions.h"

#define BUF_SIZE 100
#define READ_LIM 2500

struct pid_info {
    long pid;
    char pname[BUF_SIZE];
} p_info = {
    -1,
    "N/A"
};

static long
get_long(char * str)
{
    long res;
    char * endptr;

    if (str == NULL || *str == '\0')
        return -1;

    errno = 0;
    res = strtol(str, &endptr, 0);

    if (errno != 0)
        return -1;

    if (*endptr != '\0')
        return -1;

    return res;
}


static void
collect_info(char * loc, char * buf)
{
    char * marker = loc;
    int ctr = 0;

    // skip any leading whitespace
    while(isspace(*marker))
        marker++;

    // grab field value up to next whitespace
    while(!isspace(*marker))
    {
        buf[ctr] = *marker;
        ctr++;
        marker++;
    }
    
    // null-terminate buffer
    buf[ctr] = '\0';
}

static int
uid_match(char * buf, uid_t uid, struct pid_info * info)
{
    char found_name[BUF_SIZE], found_uid[BUF_SIZE], * loc = buf;

    while(*loc)
    {
        // make local copy of process program name
        if (strncmp(loc, "Name:", 5) == 0)
        {
            loc += 5;
            collect_info(loc, found_name);
        }

        // make local copy of process uid
        if (strncmp(loc, "Uid:", 4) == 0)
        {
            loc += 4;
            collect_info(loc, found_uid);

            // if UID matches user-supplied one, fill in struct
            if (get_long(found_uid) == uid)
            {
                strcpy(info->pname, found_name);
                return 0;
            }

            // otherwise, set struct data to "N/A"
            strcpy(info->pname, "N/A");
            return -1;
        }

        loc++;
    }
    return -1;
}


int
main(int argc, char * argv[])
{
    if (argc != 2)
        usageErr("%s <username>", argv[0]);

    char status_file[BUF_SIZE], buffer[READ_LIM];
    int fd;
    uid_t uid; 
    ssize_t num_read;
    DIR * proc_dir;
    struct dirent * proc_dirent;

    // determine Uid
    uid = userIdFromName(argv[1]);
    if (uid == -1)
        fatal("Invalid username");

    // open /proc/ dir
    proc_dir = opendir("/proc/");
    if (proc_dir == NULL)
        errExit("opendir()");

    // read through files in /proc/
    while((proc_dirent = readdir(proc_dir)) != NULL)
    {
        // convert filename to long
        // if conversion doesn't work, its not a PID
        // so continue past it
        p_info.pid = get_long(proc_dirent->d_name);
        if (p_info.pid == -1)
            continue;

        // set status file
        sprintf(status_file, "/proc/%ld/status", p_info.pid);

        // open status file
        fd = open(status_file, O_RDONLY);
        if (fd == -1)
        {
            LOG_ERROR(stderr, "cannot open file: process [%ld] may have terminated", p_info.pid);
            continue;
        }

        // read file contents into buffer
        num_read = read(fd, buffer, READ_LIM);
        if (num_read == -1)
        {
            LOG_ERROR(stderr, "cannot read file: process [%ld] may have terminated", p_info.pid);
            continue;
        }

        // null-terminate buffer
        buffer[num_read - 1] = '\0';

        // if matching uid, display proccess info
        if (uid_match(buffer, uid, &p_info) != -1)
            printf("Process [%ld]: %s\n", p_info.pid, p_info.pname);

        // close file
        if (close(fd) == -1)
        {
            LOG_ERROR(stderr, "cannot close file: process [%ld] may have terminated", p_info.pid);
            continue;
        }
    }

    exit(EXIT_SUCCESS);
}
