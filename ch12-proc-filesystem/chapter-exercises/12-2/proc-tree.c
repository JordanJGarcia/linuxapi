#include "proc-tree.h"

struct process * head = NULL;
int num_procs = 0;

// an array to hold the processes in the tree
// for simple searching
// this is actually a makeshift hash table
// made static so it is zero-initialized
static struct process * procs[MAX_PROCS];

// func prototypes
static void
print_tabs(int n)
{
    int i;
    for(i = 0; i < n; i++)
        printf("\t");
}

// MAIN
int
main(int argc, char * argv[])
{
    if (load_procs() == -1)
        exit(EXIT_FAILURE);

    print_tree(head);
    exit(EXIT_SUCCESS);
}

void
print_tree(struct process * node)
{
    // start at first child
    struct process * current = node;
    
    while(current)
    {
        printf("[%ld] %s\n", (long)current->pid, current->name);
        print_tabs(current->level);
        print_tree(current->child);
        current = current->next;
    }
}

int
add_process(const pid_t parent, const pid_t pid, char * name)
{
    // first process added to tree
    if (parent == 0 && num_procs == 0)
    {
        struct process * init = (struct process *)malloc(sizeof(struct process));
        if (init == NULL)
            return -1;

        init->pid = pid;
        strcpy(init->name, name);
        init->num_children = 0;
        init->level = 1;
        init->parent = NULL;
        init->previous = NULL;
        init->next = NULL;
        init->child = NULL;

        head = init;
        procs[pid] = head;
        num_procs++;

        return 0;
    }

    // all other processes
    struct process * previous, 
                   * current, 
                   * new,
                   * temp,
                   * found = find_process(parent);

    if(found->child)
    {
        previous = found->child;
        current = found->child->next;
    }
    else
    {
        previous = NULL;
        current = NULL;
    }
        
    // keep in ascending order
    while(current && current->pid < pid)
    {
        temp = current;
        current = current->next;
        previous = temp;
    }

    // create new node
    new = (struct process *)malloc(sizeof(struct process));
    if (new == NULL)
        return -1;

    // shuffle pointers around
    new->pid = pid;
    strcpy(new->name, name);
    new->num_children = 0;
    new->parent = found;
    new->previous = previous;
    new->next = current;
    new->child = NULL;
    new->level = found->level + 1;

    if (previous)
        previous->next = new;

    if (current)
        current->previous = new;

    if (found->child == NULL)
        found->child = new;

    found->num_children++;

    procs[pid] = new;
    num_procs++;

    return 0;
}


struct process *
find_process(const pid_t pid)
{
    return procs[pid];
}

int
load_procs(void)
{
    char status_file[BUF_SIZE], buffer[READ_LIM], * name = NULL;
    int fd;
    pid_t pid, ppid;
    ssize_t num_read;
    DIR * proc_dir;
    struct dirent * proc_dirent;

    // open /proc/ dir
    if ((proc_dir = opendir("/proc/")) == NULL)
    {
        LOG_ERROR(stderr, "Could not open /proc/ dir");
        return -1;
    }

    // read through files in /proc/
    while((proc_dirent = readdir(proc_dir)) != NULL)
    {
        // convert filename to long
        // if conversion doesn't work, its not a PID
        // so continue past it
        pid = get_long(proc_dirent->d_name);
        // 2 seems to be an idle process with no parent
        if (pid == -1 || pid == 2)
            continue;

        // set status file
        sprintf(status_file, "/proc/%ld/status", (long)pid);

        // open status file
        fd = open(status_file, O_RDONLY);
        if (fd == -1)
        {
            LOG_ERROR(stderr, "cannot open file: process [%ld] may have terminated", (long)pid);
            continue;
        }

        // read file contents (up to 2500 bytes) into buffer
        num_read = read(fd, buffer, READ_LIM);
        if (num_read == -1)
        {
            LOG_ERROR(stderr, "cannot read file: process [%ld] may have terminated", (long)pid);
            continue;
        }

        // null-terminate buffer
        buffer[num_read - 1] = '\0';

        // get parent pid
        ppid = (pid_t)get_long(get_status_field(buffer, "PPid:"));
        if (ppid == -1)
            return -1;
        else if (ppid == 2)
            continue;

        // get process name
        name = get_status_field(buffer, "Name:");
        if (name == NULL)
            continue;

        // close file
        if (close(fd) == -1)
        {
            LOG_ERROR(stderr, "cannot close file: process [%ld] may have terminated", (long)pid);
            continue;
        }
        
        // add to process tree
        if (add_process(ppid, pid, name) == -1)
            return -1;
    }
    return 0;
}
