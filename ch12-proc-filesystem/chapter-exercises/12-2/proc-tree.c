#include "proc-tree.h"

struct process * head = NULL;
int num_procs = 0;

// an array to hold the processes in the tree
// for simple searching
// this is actually a makeshift hash table
static struct process * procs[MAX_PROCS];

// func prototypes
static void
print_tabs(int n)
{
    int i;
    for(i = 0; i < n; i++)
        printf("\t");
}

static long
get_long(char * str)
{
    long res;
    char * endptr;

    if (str == NULL || *str == '\0')
        return -1;

    errno = 0;
    res = strtol(str, &endptr, 0);

    if (errno != 0)
        return -1;

    if (*endptr != '\0')
        return -1;

    return res;
}

static int
copy_word(char * loc, char * buf)
{
    char * marker = loc;
    int ctr = 0;

    // skip any leading whitespace
    while(isspace(*marker))
        marker++;

    // grab field value up to next whitespace
    while(!isspace(*marker))
    {
        if (ctr >= BUF_SIZE)
            return -1;

        buf[ctr] = *marker;
        ctr++;
        marker++;
    }

    // null-terminate buffer
    buf[ctr] = '\0';

    return 0;
}

// MAIN
int
main(int argc, char * argv[])
{
    if (load_procs() == -1)
        exit(EXIT_FAILURE);

    printf("Tree loaded...\n\n");

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
        print_tabs(current->level);
        printf("[%ld] %s\n", (long)current->pid, current->name);
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
        init->num_children = 0;
        init->level = 1;
        init->parent = NULL;
        init->previous = NULL;
        init->next = NULL;
        init->child = NULL;
        strcpy(init->name, name);

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
    new->num_children = 0;
    new->parent = found;
    new->previous = previous;
    new->next = current;
    new->child = NULL;
    new->level = found->level + 1;
    strcpy(new->name, name);

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
    proc_dir = opendir("/proc/");
    if (proc_dir == NULL)
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

char *
get_status_field(char * buf, char * field_name)
{
    static char found[BUF_SIZE];
    char * loc = buf;
    int field_size = strlen(field_name);

    while(*loc)
    {
        // make local copy of process parent id
        if (strncmp(loc, field_name, field_size) == 0)
        {
            loc += field_size;
            if (copy_word(loc, found) == -1)
                return NULL;

            return found;
        }
        loc++;
    }
    return NULL;
}
