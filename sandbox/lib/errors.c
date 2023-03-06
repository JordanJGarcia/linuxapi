#include "../include/errors.h"

/*
 * Name: log_err
 * Description:
 *      char * msg: error message
 *      va_list args: argments for format specifiers
*/
void 
log_err(FILE * fp, char * msg, ...)
{
    va_list args;
    va_start(args, msg);

    if(fp == stderr)
        fflush(fp);

    vfprintf(fp, msg, args);

    va_end(args);
}
