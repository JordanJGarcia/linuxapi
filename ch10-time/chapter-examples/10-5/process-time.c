#include <sys/times.h>
#include <time.h>
#include "../../../lib/tlpi-hdr.h"

// display 'msg' and process times
static void
displayProcessTimes(const char * msg)
{
    struct tms t;
    clock_t clockTime;
    static long clockTicks = 0;

    if (msg != NULL)
        printf("%s", msg);

    // fetch clockTicks on first call
    if (clockTicks == 0)
    {
        clockTicks = sysconf(_SC_CLK_TCK);
        if (clockTicks == -1)
            errExit("sysconf");
    }

    clockTime = clock();
    if (clockTime == -1)
        errExit("clock");

    printf("\tclock() returns: %ld clocks-per-sec (%.2f secs)\n",
            (long)clockTime, (double)clockTime / CLOCKS_PER_SEC);

    if (times(&t) == -1)
        errExit("times");

    printf("\ttimes() yeilds: user CPU=%ld,%.2f; system CPU=%ld,%.2f\n",
            (long)t.tms_utime, (double) t.tms_utime / clockTicks,
            (long)t.tms_stime, (double) t.tms_stime / clockTicks);
}

int
main(int argc, char * argv[])
{
    int numCalls, j;

    printf("CLOCKS_PER_SEC=%ld\tsysconf(_SC_CLK_TCK) = %ld\n\n",
            (long) CLOCKS_PER_SEC, sysconf(_SC_CLK_TCK));

    displayProcessTimes("At program start:\n");

    numCalls = (argc > 1) ? getInt(argv[1], GN_GT_0, "num-calls") : 100000000;
    for(j = 0; j < numCalls; j++)
        (void) getppid();

    displayProcessTimes("After getppid() loop:\n");

    exit(EXIT_SUCCESS);
}
