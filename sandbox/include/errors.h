#pragma once

#ifndef ERROR_H
#define ERROR_H

/* libraries */
#include <stdio.h>
#include <string.h>
#include <stdarg.h>

/* macros */
#ifdef __GNUC__
#define NORETURN __attribute__ ((__noreturn__))
#else
#define NORETURN
#endif

#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif

/* this is meant to come after a condition, so the blocks "{" "}" are included. */
#define LOG_ERROR(file, format, ...) \
    do                                                              \
    {                                                               \
        fprintf(file, format,  ##__VA_ARGS__);                      \
        fprintf(file, "file: %s\tline: %d\n", __FILE__, __LINE__);  \
        fprintf(file, "Errno: %s\n", strerror(errno));              \
    }                                                               \
    while(0)                                                    

#define LOG_ERROR_EXIT(status, file, format, ...) \
    do                                                              \
    {                                                               \
        fprintf(file, format,  ##__VA_ARGS__);                      \
        fprintf(file, "file: %s\tline: %d\n", __FILE__, __LINE__);  \
        fprintf(file, "Errno: %s\n", strerror(errno));              \
        exit(status);                                               \
    }                                                               \
    while(0)                                                    


/* function prototypes */
void log_err(FILE * fp, char * msg, ...);

#endif
