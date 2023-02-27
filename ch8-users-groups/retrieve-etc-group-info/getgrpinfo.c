#include <grp.h>
#include "../../lib/tlpi-hdr.h"

struct group * grp;
static gid_t mygid = 1000;

int
main(int argc, char * argv[])
{
    printf("See GETGRNAM(3) for more info\n");
    printf("\nThese functions retrieve records from the /etc/group file\n");
    printf("One by the group name, and one by the group ID\n");
    printf("\nPrototoypes:\n");
    printf("\tstruct group *getgrnam(const char *name);\n");
    printf("\tstruct group *getgrgid(gid_t gid);\n");
    
    printf("\nBoth functions return a pointer to a statically allocated struct of the following type:\n");
    printf("struct group {\n");
    printf("\tchar *gr_name;    /* Group name */\n");
    printf("\tchar *gr_passwd;  /* Group password (if not password shadowing) */\n");
    printf("\tgid_t gr_gid;     /* Group ID */\n");
    printf("\tchar **gr_mem;    /* NULL-terminated array of pointers to names of members listed in /etc/group */\n");
    
    printf("\nLets try with my own group ID (%d):\n", (int)mygid);

    errno = 0;
    grp = getgrgid(mygid);
    if (grp == NULL)
    {
        if (errno == 0)
            printf("\nGroup not found!\n");
        else
        {
            printf("\nError!\n");
            exit(EXIT_FAILURE);
        }
    }

    printf("\nGroup name: %s\n", grp->gr_name);
    printf("Group password: %s\n", grp->gr_passwd);
    printf("Group ID: %d\n", (int)grp->gr_gid);
    printf("Members: ");

    char ** member = grp->gr_mem;
    for(; *member != NULL; member++)
        printf("%s ", *member);

    exit(EXIT_SUCCESS);
}
