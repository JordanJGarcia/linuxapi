#include <shadow.h>
#include "../../lib/tlpi-hdr.h"

struct spwd *pwd;

int
main(int argc, char * argv[])
{
    printf("See GETSPWNAM(3) for more info\n");
    printf("These functions are provided to retrieve information from the /etc/shadow file\n");
    printf("\nPrototypes:\n");
    printf("\tstruct spwd *getspnam(const char *name);\n");
    printf("\tstruct spwd *getspent(void);\n");
    printf("\tvoid setspent(void);\n");
    printf("\tvoid endspent(void);\n");

    printf("\nGiven a login name in <name>, the GETSPWNAME(3) function returns a pointer to a\n");
    printf("structure of the following type, containing the corresponding information from\n");
    printf("the shadow password record:\n");

    printf("\nstruct spwd {\n");
    printf("\tchar *sp_namp;    /* login name (username) */\n");
    printf("\tchar *sp_pwdp;    /* Encrypted password */\n");
    printf("\n\t/* Remaining fields support \"password aging\" */\n");
    printf("\n\tlong sp_lstchg;   /* Time of last password change */\n");
    printf("\tlong sp_min;      /* Min. number of days between pw changes */\n");
    printf("\tlong sp_max;      /* Max number of days before change required */\n");    
    printf("\tlong sp_warn;     /* Number of days beforehand that user is warned of upcoming pw expiration */\n");
    printf("\tlong sp_inact;    /* Number of days after expiration that account is considered inactive */\n");
    printf("\tlong sp_expire;   /* Date when account expires (days since 1 Jan 1970 */\n");
    printf("\tunsigned long sp_flag; /* Reserved for future use */\n");
    printf("};\n");

    printf("\nThe GETSPENT() function returns records from the shadow password file one by one,\n");
    printf("returning NULL when there are no more records (or an error occurs)\n");

    printf("\nThe SETSPENT() function allows you to restart from the beginning of the file if you're already partially through it\n");
    printf("\nThe ENDSPENT() function closes the /etc/shadow file once you are done using it via the above 2 functions\n");

    printf("\nNow collecting my info from /etc/shadow...\n");

    // get pwd record for myself
    errno = 0;
    pwd = getspnam("jordan");
    if (pwd == NULL)
    {
        puts(" ");

        // this is because, if errno does not get reset, the record was simply not found. No errors occured
        if (errno == 0)
        {
            printf("Not found!\n"); // not found
            exit(EXIT_FAILURE);
        }
        else
            errExit("getspnam()"); 
    }

    printf("\nName: %s\n", pwd->sp_namp);
    printf("Password: %s\n", pwd->sp_pwdp);
    printf("Time of last password change: %ld\n", pwd->sp_lstchg);
    printf("Min number of days between pw changes: %ld\n", pwd->sp_min);
    printf("Max number of days before change required: %ld\n", pwd->sp_max);
    printf("Number of days warned beforehand: %ld\n", pwd->sp_warn);
    printf("Number of days after exp till inactive: %ld\n", pwd->sp_inact);
    printf("Date when account expires: %ld\n", pwd->sp_expire);

    exit(EXIT_SUCCESS);
}
