#include "../../../lib/tlpi-hdr.h"
#include <sys/time.h>
#include <time.h>
#include <limits.h>

int
main(int argc, char * argv[])
{

    signed int maxClockTicks = (signed int)INT_MAX;
    signed int clockTicksPerSec = (signed int)sysconf(_SC_CLK_TCK);
    signed int result = maxClockTicks / clockTicksPerSec;

    printf("max clock_t value (if signed 32-bit int): %d\n", maxClockTicks);

    printf("\nsysconf(_SC_CLK_TCK): %d\n", clockTicksPerSec);
    printf("It will take %d seconds (%d min | %d hours | %d days) before the clock_t values overflow (times())\n",
           result, (result / 60), (result / 60 / 60), (result / 60 / 60 / 24));

    result = maxClockTicks / (signed int)CLOCKS_PER_SEC;

    printf("\nCLOCKS_PER_SEC: %d\n", (signed int)CLOCKS_PER_SEC);
    printf("It will take %d seconds (%d min | %d hours | %d days) before the clock_t values overflow (clock())\n",
            result, (result / 60), (result / 60 / 60), (result / 60 / 60 / 24));


    exit(EXIT_SUCCESS);
}
