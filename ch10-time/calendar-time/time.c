#include "../../lib/tlpi-hdr.h"
#include <sys/time.h>
#include <time.h>


int
main(int argc, char * argv[])
{
    printf("See GETTIMEOFDAY(2) for more info\n");
    
    printf("\nPrototype:\n");
    printf("\tint gettimeofday(struct timeval *tv, struct timezone *tz);\n");

    printf("\nThis system call returns the calendar time in the buffer pointed to by <tv>\n");
    printf("The <tv> argument is a pointer to a structure of the following form:\n");
    printf("\nstruct timeval {\n");
    printf("\ttime_t      tv_sec;     /* seconds */\n");
    printf("\tsuseconds_t tv_usec;    /* microseconds */\n");
    printf("};\n");

    printf("\nThe <tz> argument is a historical artifact, and is now obsolete.\n");
    printf("It should always be specified as NULL\n");

    printf("\nAlso worth noting is that SUSv4 marks gettimeofday() obsolete.\n");
    printf("\nAnyway..\n");
    
    struct timeval tv;

    if (gettimeofday(&tv, NULL) == -1)
        errExit("gettimeofday()");
    
    printf("tv.tv_sec: %ld\n", (long)tv.tv_sec);
    printf("tv.tv_usec: %ld\n", (long)tv.tv_usec);

    printf("\nSee TIME(2) for more info\n");

    printf("\nPrototype:\n");
    printf("\ttime_t time(time_t *tloc);\n");

    printf("\nThis system call returns the number of seconds since the Epoch, which\n");
    printf("is the same value returned in the tv.tv_sec in the gettimeofday() call above\n");
    printf("If the <tloc> argument isn't NULL, the return value is also placed in the\n");
    printf("location to which <tloc> points\n");
    printf("\nAnyway..\n");

    printf("\ntime(NULL): %ld\n", (long)time(NULL));

    exit(EXIT_SUCCESS);
}
