#include "../include/utilities.h"

/*
 * Name: is_directory
 * Description:
 *      const char * dir: name of directory
*/
int 
is_directory(const char * dir)
{
    struct stat buffer;
    if(stat(dir, &buffer) != 0)
        return 0;

    return S_ISDIR(buffer.st_mode);
} 

/*
 * Name: is_reg_file
 * Description:
 *      const char * file: name of file
*/
int
is_reg_file(const char * file)
{
    struct stat buffer;
    if( stat( file, &buffer ) != 0 )
        return 0;

    return S_ISREG( buffer.st_mode );
}
