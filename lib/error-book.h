#ifndef ERROR_FUNCTIONS_H
#define ERROR_FUNCTIONS_H

/* This is a copy of the error handling library example provided
 * by the book "The Linux Programming Interface" by Michael Kerrisk (copyright 2010)
*/
void errMsg(const char * format, ...);

#ifdef __GNUC__
#define NORETURN __attribute__ ((__noreturn__))
#else
#define NORETURN
#endif

void errExit(const char * format, ...) NORETURN;
void err_exit(const char * format, ...) NORETURN;
void errExitEN(int errnum, const char * format, ...) NORETURN;
void fatal(const char * format, ...) NORETURN;
void usageErr(const char * format, ...) NORETURN;
void cmdLineErr(const char * format, ...) NORETURN;

#endif
