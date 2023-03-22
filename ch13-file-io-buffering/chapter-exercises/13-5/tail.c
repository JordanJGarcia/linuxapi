#include "../../../lib/tlpi-hdr.h"
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>

static void
display_lines(int fd, int n)
{
    char buf[1], * line = NULL;
    char * lines[n];
    ssize_t num_read;
    int i, x = 0;
    off_t line_start;
    int beg_of_file = 0;

    for(i = 0; i < n; i++)
    { 
        do 
        {
            // read in one byte
            num_read = read(fd, buf, 1);
            if (num_read == -1)
                errExit("read");
            
            // seek to prev char
            if (lseek(fd, -2, SEEK_CUR) == -1)
            {
                beg_of_file = 1;
                break; // reached beginning of file
            }

            // record char
            x++;
        }
        while(*buf != '\n');

        // create buffer to hold line
        line = (char *)malloc((x + 1) * sizeof(char));
        if (line == NULL)
            errExit("malloc");

        // mark current offset (skip past the newline of prev line)
        if((line_start = lseek(fd, (beg_of_file) ? 0 : 2, SEEK_CUR)) == -1)
            errExit("lseek 3");

        // store line
        num_read = read(fd, line, x); 
        if (num_read == -1)
            errExit("read");

        line[x] = '\0';
        lines[i] = line;

        // reset offset
        if (lseek(fd, (beg_of_file) ? line_start : line_start - 2, SEEK_SET) == -1)
            errExit("lseek 4");

        // reset x
        x = 0;
    }

    for(i = n - 1; i >=0; i--)
    {
        write(STDOUT_FILENO, lines[i], strlen(lines[i])); 
        free(lines[i]);
    }
}

int
main(int argc, char * argv[])
{
    // usage
    if (argc < 2 || argc > 4 || strcmp(argv[1], "--help") == 0 || strcmp(argv[1], "-h") == 0)
        usageErr("%s [-n num] file", argv[0]);

    int fd, n = (argc == 4) ? getInt(argv[2], GN_ANY_BASE, "n") : 10;

    // open file
    fd = open(argv[argc - 1], O_RDONLY);
    if (fd == -1)
        errExit("open");

    // seek to end of file
    if (lseek(fd, 0, SEEK_END) == -1)
        errExit("lseek 1");

    // display last n lines
    display_lines(fd, n);

    exit(EXIT_SUCCESS);
}
