#include "../../lib/tlpi-hdr.h"
#include <sys/times.h>
#include <time.h>


int
main(int argc, char * argv[])
{
    printf("See TIMES(2) for more info\n");
    
    printf("\nPrototype:\n");
    printf("\tclock_t times(struct tms *buf);\n");

    printf("\nThe times() system call retrieves process information,\n");
    printf("returning it in the same structure pointed to by <buf>\n");

    printf("\nThe tms structure pointed to by <buf> has the following form:\n");
    printf("\nstruct tms {\n");
    printf("\tclock_t tms_utime;  /* user time */\n");
    printf("\tclock_t tms_stime;  /* system time */\n");
    printf("\tclock_t tms_cutime; /* user time of children */\n");
    printf("\tclock_t tms_cstime; /* system time of children */\n");
    printf("};\n");

    printf("\nThe <clock_t> data type used by the 4 fields is an integer type\n");
    printf("that measures time in units called clock ticks.\n");
    printf("We can call sysconf(_SC_CLK_TCK) to obtain the number of clock ticks per second\n");
    printf("and then divide a clock_t value by this number to convert to seconds\n");

    struct tms tms;
    times(&tms);

    printf("\nclock_t result = times(&tms);\n");

    printf("\ntms.tms_utime: %ld\n", (long)tms.tms_utime);
    printf("tms.tms_stime: %ld\n", (long)tms.tms_stime);
    printf("tms.tms_cutime: %ld\n", (long)tms.tms_cutime);
    printf("tms.tms_cstime: %ld\n", (long)tms.tms_cstime);

    printf("\nAs you can see, this information is not very reliable\n");
    printf("On success, times() returns the elapsed (real) time in clock ticks since\n");
    printf("some arbitrary point in the past. SUSv3 deliberately does not specify what this point is,\n");
    printf("merely stating that it will be constant during the life of the calling process\n");

    printf("\nTherefore, the only portable use of this return value is to measure elapsed\n");
    printf("time in eh execution of the process by calculating the difference in the value\n");
    printf("returned by pairs of times() calls.\n");

    printf("\n----------------------------------------------------------------------------------------------\n");

    printf("\nSee CLOCK(3) for more info\n");
    
    printf("\nPrototype:\n");
    printf("\tclock_t clock(void);\n");

    printf("\nThe clock() function provides a simpler interface for retrieving the\n");
    printf("process time.\n");
    printf("It returns a single value that measures the total CPU time (user and system)\n");
    printf("used by the calling process.\n");

    printf("\nThe value returend by clock() is measured in units of CLOCKS_PER_SEC, so we must\n");
    printf("divide by this value to arrive art the number of seconds of CPU time used by the process\n");

    clock_t start = clock();
    
    int i;
    for(i = 0; i < 100000; i++)
        fprintf(stderr, "loop %d\n", i);

    clock_t end = clock();

    printf("\nPrinting \"loop <n>\" to stderr 100,000 times took (%.5f) seconds\n", (double)((end - start)/CLOCKS_PER_SEC));

    exit(EXIT_SUCCESS);
}
