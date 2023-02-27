#include <pwd.h>
#include "../../lib/tlpi-hdr.h"

struct passwd *pwd;

int
main(int argc, char * argv[])
{
    printf("See GETPWNAM(3) for more info\n");
    printf("\nThese functions are provided to retrieve information from the /etc/passwd file\n");
    printf("Prototypes:\n");
    printf("\tstruct passwd *getpwnam(const char *name);\n");
    printf("\tstruct passwd *getpwuid(uid_t uid);\n");

    printf("\nGiven a login name in <name>, the GETPWNAME(3) function returns a pointer to a\n");
    printf("structure of the following type, containing the corresponding information from\n");
    printf("password record:\n");

    printf("\nstruct passwd {\n");
    printf("\tchar *pw_name;    /* login name (username) */\n");
    printf("\tchar *pw_passwd;  /* Encrypted password (if password shadowing is not enabled) */\n");
    printf("\tuid_t pw_uid;     /* User ID */\n");
    printf("\tgid_t pw_gid;     /* Group ID */\n");
    printf("\tchar *pw_gecos;   /* Comment */\n");
    printf("\tchar *pw_dir;     /* Home directory */\n");
    printf("\tchar *pw_shell;   /* Login shell */\n");
    printf("};\n");

    printf("\nThe GETPWUID() function returns exactly the same info, but does a lookup\n");
    printf("using the numeric user ID supplied in the argument <uid>\n");
    printf("\nBoth of these functions return a pointer to a statically allocated structure\n");
    printf("\nNow collecting my info from /etc/passwd...\n");

    // get pwd record for myself
    errno = 0;
    pwd = getpwnam("jordan");
    if (pwd == NULL)
    {
        if (errno == 0)
            printf("\nNot found!\n"); // not found
        else
            printf("\nError!\n"); // error
    }

    printf("\nName: %s\n", pwd->pw_name);
    printf("Password: %s\n", pwd->pw_passwd);
    printf("User ID: %d\n", (int)pwd->pw_uid);
    printf("Group ID: %d\n", (int)pwd->pw_gid);
    printf("Comment: %s\n", pwd->pw_gecos);
    printf("Home Dir: %s\n", pwd->pw_dir);
    printf("Shell: %s\n", pwd->pw_shell);

    exit(EXIT_SUCCESS);
}
