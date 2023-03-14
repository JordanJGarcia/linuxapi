#ifndef PROC_TREE_H
#define PROC_TREE_H

#include <sys/types.h>
#include <ctype.h>
#include <dirent.h>
#include <fcntl.h>
#include "../../../lib/tlpi-hdr.h"
#include "../../../lib/error.h"

#define MAX_CHILDREN 1000
#define MAX_PROCS 1000000
#define READ_LIM 2500
#define BUF_SIZE 100

// represents a process tree
struct process {
    pid_t pid;
    char name[BUF_SIZE];
    int num_children;
    int level;
    struct process * parent;
    struct process * previous;
    struct process * next;
    struct process * child;
};

// prototypes
void print_tree(struct process * node);
int add_process(const pid_t parent, const pid_t pid, char * name);
struct process * find_process(const pid_t pid);
int load_procs(void);
char * get_status_field(char * buf, char * field_name);

#endif
