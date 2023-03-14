#include "../proc-utilities.h"
#include "../../../lib/tlpi-hdr.h"
#include "../../../lib/error.h"
#include "../../../ch8-users-groups/chapter-examples/8-1/ugid-functions.h"
#include <fcntl.h>
#include <sys/stat.h>

struct pid_info {
    long pid;
    char pname[BUF_SIZE];
} p_info = {
    -1,
    "N/A"
};


static int
uid_match(char * buf, uid_t uid, struct pid_info * info)
{
    // if UID matches user-supplied one, fill in struct
    if (get_long(get_status_field(buf, "Uid:")) == (long)uid)
    {
        strcpy(info->pname, get_status_field(buf, "Name:"));
        return 0;
    }

    // otherwise, empty struct
    strcpy(info->pname, "");
    info->pid = 0;
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

        // read file contents (up to 2500 bytes) into buffer
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
