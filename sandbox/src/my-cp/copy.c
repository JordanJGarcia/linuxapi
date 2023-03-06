#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <libgen.h>
#include <errno.h>

#include "../../include/errors.h"
#include "../../include/utilities.h"

/* macros */
#ifndef BUF_SIZE /* allow "cc -D" to override defintion */
#define BUF_SIZE 1024
#endif

#define MAXFILES 255

#define INMODE "rb"

#define OUTFILE argv[argc - 1]
#define OUTMODE "wbx"
#define OVERWRITEMODE "wb"
#define DECLINEOVERWRITE 2

#define PROGNAME basename(argv[0])

/* prototypes */
static int open_input_file(char * file);
static int open_output_file(char * file);
static int transfer_data();
static int close_files();

/* globals */
static FILE * fd_in[MAXFILES], * fd_out[MAXFILES];

/* Function: main
 * Description:
 *      This program is my implementation of cp
*/
int
main(int argc, char * argv[])
{
    int i, outfile_open = 0;
    char outfile[BUF_SIZE];

    /* usage check */
    if(argc > 3 && !is_directory(OUTFILE))
    {
        log_err(stderr, "Usage: %s input [input] [input] ... directory\n", PROGNAME);
        exit(EXIT_FAILURE);
    }
    else if(argc < 3 || strcmp(argv[1], "--help") == 0)
    {
        log_err(stderr, "Usage: %s input output\n", PROGNAME);
        exit(EXIT_FAILURE);
    }

    /* open output file, case when copying one file to another, and not multiple files to a directory */
    if(!is_directory(OUTFILE))
    {
        if(!open_output_file(OUTFILE))
            exit(EXIT_FAILURE);

        outfile_open = 1;
    }

    /* copy */
    for(i = 1; i < argc - 1; i++)
    {
        /* if output file hasn't been open, then it is a directory */
        /* we already verified that it is a directory on line 58 */
        if(!outfile_open)
        {
            /* create file path, since only directory provided */
            sprintf(outfile, "%s%s%s", OUTFILE, (OUTFILE[strlen(OUTFILE) - 1] == '/' ? "" : "/"), argv[i]);

            int rc = open_output_file(outfile);

            /* if user declines overwrite, we don't want to run any of the remaining functions */
            if(rc == DECLINEOVERWRITE)
                continue;
            else if(!rc)
                exit(EXIT_FAILURE);
        }

        /* open input file */
        if(!open_input_file(argv[i]))
            exit(EXIT_FAILURE);

        /* transfer data */
        if(!transfer_data())
            exit(EXIT_FAILURE);

        /* close current input & output file */
        if(!close_files())
            exit(EXIT_FAILURE);
    }

    /* done */
    return EXIT_SUCCESS;
}


/* Function: open_input_file
 * Description:
 *      Opens input and output files with proper checks
*/
static int
open_input_file(char * file)
{
    static int fileno = 0;

    /* open input file */
    fd_in[fileno] = fopen(file, INMODE);
    if(fd_in[fileno] == NULL)
    {
        log_err(stderr, "Error: opening file '%s' in mode '%s': %s\n",
                 file, INMODE, strerror(errno));
        return 0;
    }
    fileno++;
    return 1;
}


/* Function: open_input_file
 * Description:
 *      Opens output file with proper checks
*/
static int
open_output_file(char * file)
{
    static int fileno = 0;

    /* open output file */
    fd_out[fileno] = fopen(file, OUTMODE);
    if(fd_out[fileno] == NULL)
    {
        /* if file exists, prompt user if they want to overwrite it */
        if(errno == EEXIST)
        {
            char option[255];

            /* prompt user if they want to overwrite */
            do
            {
                log_err(stderr, "Warning: file '%s' already exists, overwrite (y or n)? ", file);
                scanf("%s", option);
            }
            while(strcmp(option, "y") && strcmp(option, "n") &&
                  strcmp(option, "Y") && strcmp(option, "N"));

            /* overwrite file if desired */
            if(strcmp(option, "y") == 0 || strcmp(option, "Y") == 0)
            {
                fd_out[fileno] = fopen(file, OVERWRITEMODE);
                if(fd_out[fileno] == NULL)
                {
                    log_err(stderr, "Error: opening file '%s' in mode '%s': %s\n",
                            file, OVERWRITEMODE, strerror(errno));
                    return 0;
                }
            }
            else /* exit successfully if overwrite not desired */
                return DECLINEOVERWRITE;
        }
        else
        {
            log_err(stderr, "Error: opening file '%s' in mode '%s': %s\n",
                    file, OUTMODE, strerror(errno));
            return 0;
        }
    }

    fileno++;
    return 1;
}


/* Function: transfer_data
 * Description:
 *      Copies data from one file to another
*/
static int
transfer_data()
{
    static int fileno = 0;
    size_t bytes_read;
    char buffer[BUF_SIZE];

    /* check that output file descriptor is open, it would not be if user doesn't want to overwrite file */
    while(fd_out[fileno] && (bytes_read = fread(buffer, 1, BUF_SIZE, fd_in[fileno])) > 0)
    {
        /* check that bytes written = bytes read */
        if(fwrite(buffer, 1, bytes_read, fd_out[fileno]) != bytes_read)
        {
            log_err(stderr, "Error: could not write entire buffer to file %d (fd %d): %s\n",
                    fileno, fd_out[fileno], strerror(errno));
            return 0;
        }
    }

    /* check that read didn't error out */
    if(bytes_read == -1)
    {
        log_err(stderr, "Error: reading input file %d (fd %d): %s\n", 
                fileno, fd_in[fileno], strerror(errno));
        return 0;
    }

    fileno++;
    return 1;
}


/* Function: close_file
 * Description:
 *      Closes a file descriptor if open
*/
static int
close_files()
{
    static int fileno = 0;

    /* close input file */
    if(fclose(fd_in[fileno]) != 0)
    {
        log_err(stderr, "Error: could not close file %d (fd %d): %s\n", fileno, fd_in[fileno], strerror(errno));
        return 0;
    }

    /* close output file */
    if(fclose(fd_out[fileno]) != 0)
    {
        log_err(stderr, "Error: could not close file %d (fd %d): %s\n", fileno, fd_out[fileno], strerror(errno));
        return 0;
    }

    fileno++;
    return 1;
}

