#include "../../../lib/tlpi-hdr.h"

// globals
extern char ** environ;

// prototypes
int mySetenv(const char * name, const char * value, int overwrite);
int myUnsetenv(const char * name);

// main
int
main(int arg, char * argv[])
{
    char ** env;

    // print current env list
    puts("\n\n\n");
    for(env = environ; *env; env++)
        puts(*env);

    // set a new var
    if (mySetenv("MYNAME", "JORDAN", 0) == -1)
        fatal("mySetenv() 1");

    // print current env list
    puts("\n\n\n");
    for(env = environ; *env; env++)
        puts(*env);

    // overwrite that var
    if (mySetenv("MYNAME", "NONYA", 1) == -1)
        fatal("mySetenv() 2");

    // print current env list
    puts("\n\n\n");
    for(env = environ; *env; env++)
        puts(*env);

    // unset var
    if (myUnsetenv("MYNAME") == -1)
        fatal("myUnsetenv()");
    
    // print current env list
    puts("\n\n\n");
    for(env = environ; *env; env++)
        puts(*env);

    exit(EXIT_SUCCESS);
}


// function definitions
int
mySetenv(const char * name, const char * value, int overwrite)
{
    char * buffer = (char *)malloc((strlen(name) + strlen(value) + 2) * sizeof(char));
    if (!buffer)
        return -1;

    // copy name=value into buffer
    strcpy(buffer, name);
    strcat(buffer, "=");
    strcat(buffer, value);

    // check if env var already exists
    if (getenv(buffer) == NULL)
    {
        if (putenv(buffer) == 0)
            return 0;
        else
            return -1;
    }
    
    // check if we want to overwrite value
    if (overwrite != 0)
    {
        // remove var from env first
        if (putenv(name) != 0)
            return -1;

        // replace with new value
        if (putenv(buffer) == 0)
            return 0;
        else
            return -1;
    }

    return -1;
}

int
myUnsetenv(const char * name)
{
    if (name == NULL || strstr(name, "=") != NULL)
        return -1;

    putenv(name);

    return 0;
}
