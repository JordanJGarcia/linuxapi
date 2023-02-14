#ifndef TLPI_HDR_H
#define TLPI_HDR_H

#include <sys/types.h>  /* type definitions used by many programs */
#include <stdio.h>      /* standard I/O functions */
#include <stdlib.h>     /* prototypes of common library functions, as well as EXIT_SUCCESS & EXIT_FAILURE */
#include <unistd.h>     /* prototypes for many system calls */
#include <errno.h>      /* errno and error constants */
#include <string.h>     /* common string-handling functions */
#include "get-num.h"
#include "error-book.h"

typedef enum { FALSE, TRUE } Boolean;

#define min(m,n) ((m) < (n) ? (m) : (n))
#define max(m,n) ((m) > (n) ? (m) : (n))

#endif
