#define _DEFAULT_SOURCE     /* Get getpass() declaration from <unistd.h> */
#define _XOPEN_SOURCE       /* get crypt() declaration from <unistd.h> */

#include <unistd.h>
#include <limits.h>
#include <pwd.h>
#include <shadow.h>
#include "../../../lib/tlpi-hdr.h"

int
main(int argc, char * argv[])
{
    char * username, * password, * encrypted, *p;
    struct passwd *pwd;
    struct spwd * spwd;
    Boolean authOk;
    size_t len;
    long lnmax;

    // if limit is indeterminate, make a guess
    lnmax = sysconf(_SC_LOGIN_NAME_MAX);
    if (lnmax == -1)
        lnmax = 256;

    username = malloc(lnmax);
    if (username == NULL)
        errExit("malloc");

    printf("Username: ");
    fflush(stdout);
    if (fgets(username, lnmax, stdin) == NULL)
        exit(EXIT_FAILURE); // exit on EOF

    len = strlen(username);
    if (username[len - 1] == '\n')
        username[len - 1] = '\0'; // remove trailing newline

    pwd = getpwnam(username);
    if (pwd == NULL)
        fatal("couldn't get password record");

    spwd = getspnam(username);
    if (spwd == NULL && errno == EACCES)
        fatal("no permission to read shadow password file");

    // if there is a shadow password record, use the shadow password
    if (spwd != NULL)
        pwd->pw_passwd = spwd->sp_pwdp;

    password = getpass("Password: ");

    /* Encrypt password and erase cleartext version immediately */
    encrypted = crypt(password, pwd->pw_passwd);
    for (p = password; *p != '\0'; )
        *p++ = '\0';

    if (encrypted == NULL)
        errExit("crypt");

    authOk = strcmp(encrypted, pwd->pw_passwd) == 0;
    if (!authOk)
    {
        printf("Incorrect password\n");
        exit(EXIT_FAILURE);
    }

    printf("Successfully authenticated: UID=%ld\n", (long) pwd->pw_uid);

    /* Now do authenticated work */

    exit(EXIT_SUCCESS);
}
