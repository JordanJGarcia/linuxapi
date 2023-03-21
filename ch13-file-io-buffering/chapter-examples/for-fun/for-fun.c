#include "../../../lib/tlpi-hdr.h"

int
main(int argc, char * argv[])
{
    printf("I/O system calls transfer data directly to the kernel buffer cache\n");
    printf("The stdio library waits until the streams's user-space buffer is full\n");
    printf("before calling write() to transfer that buffer to the kernel buffer cache\n");

    printf("\nSo, as an example, consider the following two lines:\n");
    printf("\tprintf(\"To man the world is twofold, \");\n");
    printf("\twrite(STDOUT_FILENO, \"in accordance with his twofold attitude.\\n\", 41);\n");

    printf("\nThe write will usually print first, since it isn't buffered in the user-space...\n\n");

    printf("To man the world is twofold, ");
    write(STDOUT_FILENO, "in accordance with his twofold attitude.\n", 41);

    exit(EXIT_SUCCESS);
}
