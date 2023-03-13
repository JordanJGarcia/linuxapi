#pragma once

#ifndef ERROR_H
#define ERROR_H

/* libraries */
#include <stdio.h>  /* stdio functions */
#include <string.h> /* string functions */
#include <stdarg.h> /* uh.... */
#include <errno.h>  /* errno and error constants */

// errno is statically allocated, so does not need to be passed to these macro functions
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

#endif
