#include "../../lib/tlpi-hdr.h"
#include <time.h>

int
main(int argc, char * argv[])
{
    printf("Note:\n");
    printf("\tSUSv4 marks ctime() and asctime() obsolete, because they do not return\n");
    printf("\tlocalized strings (and they are nonreentrant)\n");

    printf("\nSee CTIME(3) for more info\n");
    printf("\nPrototype:\n");
    printf("\tchar *ctime(const time_t *timep);\n");

    printf("\nThis returns a pointer to a statically allocated string terminated by newline\n");
    printf("and \\0 on success, or NULL on error\n");

    time_t currentTime;
    char timeCopy[30] = "\0";

    currentTime = time(NULL);
    if (currentTime == -1)
        errExit("time()");

    // make local copy of time
    strcpy(timeCopy, ctime(&currentTime));

    if (timeCopy == NULL || timeCopy[0] == '\0')
        errExit("ctime() or strcpy()");

    printf("\nctime(&currentTime): %s\n", timeCopy);

    printf("\n-----------------------------------------------------------------------------------\n");

    printf("\nSee CTIME(3) for more info\n");
    
    printf("\nPrototype:\n");
    printf("\tstruct tm *gmtime(const time_t *timep);\n");
    printf("\tstruct tm *localtime(const time_t *timep);\n");

    printf("\nThe gmtime() and localtime() functions convert time_t value into a\n");
    printf("so-called broken-down time\n");
    printf("The broken-down time is placed in a statically allocated structure whose\n");
    printf("address is returned as the function result\n");

    printf("\nThe gmtime() converts a calendar time into a broken-down time corresponding\n");
    printf("to UTC. (the letters gm derive from Greenwich Mean Time)\n");
    printf("\nlocaltime() takes into account timezone and DST settings to return a broken-down\n");
    printf("time corresponding to the system's local time.\n");

    printf("\nThe <tm> structure returned by these functions contains the date and time fields\n");
    printf("broken into individual parts. The structure has the following form:\n");
    printf("\nstruct tm {\n");
    printf("\tint tm_sec;    /* Seconds (0-60) */\n");
    printf("\tint tm_min;    /* Minutes (0-59) */\n");
    printf("\tint tm_hour;   /* Hours (0-23) */\n");
    printf("\tint tm_mday;   /* Day of the month (1-31) */\n");
    printf("\tint tm_mon;    /* Month (0-11) */\n");
    printf("\tint tm_year;   /* Year - 1900 */\n");
    printf("\tint tm_wday;   /* Day of the week (0-6, Sunday = 0) */\n");
    printf("\tint tm_yday;   /* Day in the year (0-365, 1 Jan = 0) */\n");
    printf("\tint tm_isdst;  /* Daylight saving time */\n");
    printf("};\n");

    struct tm * brokenTime = localtime(&currentTime);
    printf("\nstruct tm * brokenTime = localtime(&currentTime);\n");

    printf("\nbrokenTime->tm_sec: %d\n", brokenTime->tm_sec);
    printf("brokenTime->tm_min: %d\n", brokenTime->tm_min);
    printf("brokenTime->tm_hour: %d\n", brokenTime->tm_hour);
    printf("brokenTime->tm_mday: %d\n", brokenTime->tm_mday);
    printf("brokenTime->tm_mon: %d\n", brokenTime->tm_mon);
    printf("brokenTime->tm_year: %d\n", brokenTime->tm_year);
    printf("brokenTime->tm_wday: %d\n", brokenTime->tm_wday);
    printf("brokenTime->tm_yday: %d\n", brokenTime->tm_yday);
    printf("brokenTime->tm_isdst: %d\n", brokenTime->tm_isdst);

    printf("\n-----------------------------------------------------------------------------------\n");

    printf("\nSee CTIME(3) for more info\n");

    printf("\nPrototype:\n");
    printf("\ttime_t mktime(struct tm *tm);\n");

    printf("\nThe mktime() function translates a broken-down time, expressed as local time\n");
    printf("into a time_t value, which is returned as the function result\n");

    currentTime = mktime(brokenTime);

    printf("\nmktime(brokenTime): %ld\n", (long)mktime(brokenTime));

    printf("\n-----------------------------------------------------------------------------------\n");

    printf("\nSee CTIME(3) for more info\n");

    printf("\nPrototype:\n");
    printf("\tchar *asctime(const struct tm *tm);\n");

    printf("\nGiven a pointer to a broken-down time structure in the argument <tm>,\n");
    printf("asctime() returns a pointer to a statically allocated string containing the\n");
    printf("time in the same form as ctime()\n");

    printf("\nasctime(brokenTime): %s\n", asctime(brokenTime));
    exit(EXIT_SUCCESS);
}
