#include "../../lib/tlpi-hdr.h"

int
main(int argc, char * argv[])
{
    printf("See GETUID(2) & GETGID(2) for more info\n");

    printf("\nPrototypes:\n");
    printf("\tuid_t getuid(void);\n");
    printf("\tgid_t getgid(void);\n");

    printf("\nThese system calls return the real user ID and real group ID of the calling process\n");

    printf("\nPrototypes:\n");
    printf("\tuid_t geteuid(void);\n");
    printf("\tgid_t getegid(void);\n");

    printf("\nThese system calls return the effective user ID and effective group ID of the calling process\n");

    printf("\nAll of these system calls are always successful\n");

    printf("\nReal UID:\t%ld\n", (long)getuid());
    printf("Real GID:\t%ld\n", (long)getgid());
    printf("Effective UID:\t%ld\n", (long)geteuid());
    printf("Effective GID:\t%ld\n", (long)getegid());

    printf("\nYou can modify the IDs with their corresponding set functions:\n");
    printf("\tint setuid(uid_t uid);\n");
    printf("\tint seteuid(uid_t euid);\n");
    printf("\tint setgid(gid_t gid);\n");
    printf("\tint setegid(gid_t egid);\n");

    printf("\nThe rules about what changes a process can make to its credentials using\n");
    printf("setuid()/setgid() (REAL IDs) depend on whether the process is privelaged\n");

    printf("\nIf a privelaged process sets its credentials to something other than 0 (privelaged)\n");
    printf("then it ceases to be privelaged. It is a one-way trip\n");

    exit(EXIT_SUCCESS);
}
