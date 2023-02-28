#define _DEFAULT_SOURCE
#include <sys/types.h>
#include <pwd.h>
#include "../../../lib/tlpi-hdr.h"

// macros
#define ME "jordan"

// prototype
static struct passwd * my_getpwnam(const char * name);

// globals
static struct passwd * pwd;

int
main(int argc, char * argv[])
{
    errno = 0;
    pwd = my_getpwnam(ME);
    if (pwd == NULL)
    {
        if (errno != 0)
            errExit("my_getpwnam()");
        else
            fatal("Record not found");
    }

    // print record
    printf("\nName: %s\n", pwd->pw_name);
    printf("Password: %s\n", pwd->pw_passwd);
    printf("User ID: %d\n", (int)pwd->pw_uid);
    printf("Group ID: %d\n", (int)pwd->pw_gid);
    printf("Comment: %s\n", pwd->pw_gecos);
    printf("Home Dir: %s\n", pwd->pw_dir);
    printf("Shell: %s\n", pwd->pw_shell);

    exit(EXIT_SUCCESS);
}

struct passwd *
my_getpwnam(const char * name)
{
    // statically allocated struct
    static struct passwd * result = NULL;

    // loop through passwd recs
    while((result = getpwent()) != NULL)
    {
        if (strcmp(result->pw_name, name) == 0)
            break;
    }

    // close /etc/passwd
    endpwent();

    return result;
}
