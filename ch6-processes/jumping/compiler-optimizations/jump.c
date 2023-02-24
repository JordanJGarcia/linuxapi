#include <stdio.h>
#include <stdlib.h>
#include <setjmp.h>

static jmp_buf env;

static void
doJump(int nvar, int rvar, int vvar)
{
    printf("Inside doJump(): nvar = %d rvar = %d vvar = %d\n", nvar, rvar, vvar);
    longjmp(env, 1);
}


int
main(int argc, char * argv[])
{
    printf("Since jump operations performed via setjmp() and longjmp() are established\n");
    printf("and executed at run time, and are not reflected in the lexical structure of the\n");
    printf("program, a compiler optimizer is unable to take them into account when\n");
    printf("performing optimizations\n");
    printf("\nFurthermore, the semantics of some ABI implementations require longjmp()\n");
    printf("to restore copies of the CPU registers saved by the earlier setjmp() call\n\n");
    printf("This may not make much sense right now, see pg. 136 of the book for more info\n\n");

    int nvar;
    register int rvar;
    volatile int vvar;

    nvar = 111;
    rvar = 222;
    vvar = 333;

    if (setjmp(env) == 0)
    {
        nvar = 777;
        rvar = 888;
        vvar = 999;
        doJump(nvar, rvar, vvar);
    }
    else
        printf("After longjmp(): nvar = %d rvar = %d vvar = %d\n", nvar, rvar, vvar);

    exit(EXIT_SUCCESS);
}
