#include "../../lib/tlpi-hdr.h"

// unitialized data segment
// remember, system initializes all memory in this segment to 0's
char globBuf[65536];
int primes[] = { 2, 3, 5, 7 };

// static function
// remember, one stack frame is allocated for each
// currently called function
static int
square(int x) // allocated in frame for square()
{
    // allocated in frame for square()
    int result;

    result = x * x;

    // return value passed via register
    return result;
}

static void
doCalc(int val) // allocated in frame for doCalc()
{
    printf("The square of %d is %d\n", val, square(val));

    if (val < 1000)
    {
        // allocated in frame for doCalc()
        int t;

        t = val * val * val;
        printf("The cube of %d is %d\n", val, t);
    }
}


// main
int
main(int argc, char * argv[]) // allocated in frame for main()
{
    printf("You will need to read the source code to properly understand this one.\n");
    printf("Although, I hope you are always reading the source code! ;)\n");

    // initialized data segment
    static int key = 997;
    
    // uninitialized data segment
    static char mbuf[10240000];
    
    // allocated in frame for main()
    // remember a pointer is just a numerical type (address)
    char * p;

    // points to memory in heap segment
    p = malloc(1024);

    doCalc(key);

    free(p);

    exit(EXIT_SUCCESS);
}
