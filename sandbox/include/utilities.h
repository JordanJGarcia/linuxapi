#pragma once

#ifndef UTILITIES_H
#define UTILITIES_H

/* libraries */
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

/* prototypes */
int is_directory(const char * dir);
int is_reg_file(const char * file);

#endif
