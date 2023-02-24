#include <setjmp.h>
#include "../../lib/tlpi-hdr.h"

// jum_bufs are declared globally usually,
// or less commonly, passed as a func arg
// this stores the information that is needed
// to jump between function calls
static jmp_buf env;
void longjmp(jmp_buf env, int val);

// static functions
static void
f2(void)
{
    longjmp(env, 2);
}

static void
f1(int argc)
{
    if (argc == 1)
        longjmp(env, 1);
    f2();
}


// main
int
main(int argc, char * argv[])
{
    printf("Like many programming languages, C includes the `goto` statement\n");
    printf("One restriction of C's goto is that it is not possible to jump\n");
    printf("out of the current function into another function\n");

    printf("\nSuch functionality is sometimes useful, and can be acheived through\n");
    printf("the functions setjmp() and longjmp()\n");
    
    printf("\nPrototypes:\n");
    printf("\tint setjmp(jmp_buf env);\n");
    printf("\tvoid longjmp(jmp_buf env, int val);\n");

    printf("\nCalling setjmp() establishes a target for a later jump performed\n");
    printf("bye longjmp().\n");

    printf("\nAlong with other information, <env> stores a copy of the program counter\n");
    printf("register, and the stack pointer register at the time of the call to setjmp()\n");
    printf("This information enables the subsequent longjmp() call to accomplish 2 key steps:\n");
    printf("\tStrip off the stack frames for all of the intervening functions on the stack between\n");
    printf("\tthe function calling longjmp() and the function that previously called setjmp()\n");
    printf("\n\tReset the program counter register so that the program continues execution\n");
    printf("\tfrom the location of the initial setjmp() call\n\n");

    switch (setjmp(env))
    {
        // this part always gets called first since
        // setjmp() returns 0 on intial call
        case 0: // return after initial setjmp()
            printf("Calling f1() after initial setjmp()\n");
            
            // never returns
            f1(argc);
            break;
        case 1:
            printf("We jumped back from f1()\n");
            break;
        case 2:
            printf("We jumped back from f2()\n");
            break;
    }

    exit(EXIT_SUCCESS);
}
