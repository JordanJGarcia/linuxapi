#ifndef PROC_UTILITIES_H
#define PROC_UTILITIES_H

#include <sys/types.h>
#include <ctype.h>
#include <dirent.h>
#include <stdlib.h>
#include "../../lib/error.h"

#define READ_LIM 2500
#define BUF_SIZE 100

// prototypes
char *  get_status_field(char * buf, char * field_name);
long    get_long(char * str);

#endif

