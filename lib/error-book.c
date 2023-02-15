#include <stdarg.h>
#include "error-book.h"
#include "tlpi-hdr.h"
#include "ename.c.inc"

/*
 * Terminates the program with either exit(3) or _exit(2)
*/
NORETURN
static void
terminate(Boolean useExit3)
{
    char * s;

    /* Dump core if EF_DUMPCORE environment variable is defined and
     * is a nonempty string; otherwise call exit(3) or _exit(2),
     * depending on the value of 'useExit3'
    */

    s = getenv("EF_DUMPCORE");

    if (s != NULL && *s != '\0')
        abort();
    else if (useExit3)
        exit(EXIT_FAILURE);
    else
        _exit(EXIT_FAILURE);
}

/* 
 * Outputs error info and message to stderr
*/
static void
outputError(Boolean useErr, int err, Boolean flushStdout,
        const char * format, va_list ap)
{
#define BUF_SIZE 500
    char buf[1024], userMsg[BUF_SIZE], errText[BUF_SIZE];

    // place user (formatted) message into buffer
    vsnprintf(userMsg, BUF_SIZE, format, ap);

    // place error information into buffer
    if (useErr)
        snprintf(errText, BUF_SIZE, " [%s - %s]",
                (err > 0 && err <= MAX_ENAME) ?
                ename[err] : "?UNKNOWN?", strerror(err));
    else
        snprintf(buf, BUF_SIZE, ":");

    // create complete error message buffer (contains above 2 buffers)
    snprintf(buf, BUF_SIZE, "ERROR%s %s\n", errText, userMsg);

    // flush any pending stdout
    if (flushStdout)
        fflush(stdout);

    // print to stderr & flush stderr
    fputs(buf, stderr);
    fflush(stderr);
}


/*
 * Prints error text corresponding to current value of errno (name and desc)
 * followed by formatted output specified, to stderr
*/
void
errMsg(const char * format, ...)
{
    va_list argList;
    int savedErrno;

    // save errno in case we change it in upcoming call
    savedErrno = errno;

    va_start(argList, format);
    outputError(TRUE, errno, TRUE, format, argList);
    va_end(argList);

    errno = savedErrno;
}


/* 
 * Operates like errMsg() above, but also terminates the program
 * by calling exit(3)
*/
void
errExit(const char * format, ...)
{
    va_list argList;

    va_start(argList, format);
    outputError(TRUE, errno, TRUE, format, argList);
    va_end(argList);

    terminate(TRUE);
}


/* 
 * Operates like errExit() above, but does not flush stdout before printing
 * the error message, and terminates the process by calling _exit(2)
 * which causes the process to terminate without flushing stdio buffers or
 * invoking exit handlers
*/
void
err_exit(const char * format, ...)
{
    va_list argList;

    va_start(argList, format);
    outputError(TRUE, errno, FALSE, format, argList);
    va_end(argList);

    terminate(FALSE);
}


/* 
 * Operates like errExit(), but prints the error text correpsonding
 * to the error number, rather than the current value of errno.
 * Mainly used for POSIX threads api errors.
*/
void
errExitEN(int errnum, const char * format, ...)
{
    va_list argList; 

    va_start(argList, format);
    outputError(TRUE, errnum, TRUE, format, argList);
    va_end(argList);

    terminate(TRUE);
}


/*
 * Used to diagnose genera errors that don't set errno, and terminates
 * the process using exit(3)
*/
void
fatal(const char * format, ...)
{
    va_list argList;

    va_start(argList, format);
    outputError(FALSE, 0, TRUE, format, argList);
    va_end(argList);

    terminate(TRUE);
}


/*
 * Used to print a Usage: error
*/
void
usageErr(const char * format, ...)
{
    va_list argList;

    // flush any pending stdout
    fflush(stdout);

    fprintf(stderr, "Usage: ");
    va_start(argList, format);
    vfprintf(stderr, format, argList);
    va_end(argList);

    // in case stderr is not line-buffered
    fflush(stderr);

    exit(EXIT_FAILURE);
}


/*
 * Similar to usageErr() but intended for diagnosing
 * errors in the command-line arguments specified to
 * a program
*/
void
cmdLineErr(const char * format, ...)
{
    va_list argList;

    // flush any pending stdout
    fflush(stdout);

    fprintf(stderr, "Command-line usage error: ");
    
    va_start(argList, format);
    vfprintf(stderr, format, argList);
    va_end(argList);

    // in case stderr is not line-buffered
    fflush(stderr);
    exit(EXIT_FAILURE);
}
