#include <stdio.h>
#include "fs.h"
#include "shell.h"

int main(void) {
    fs_init();
    printf("=== TernOS (hosted) ===\n");
    shell_loop();
    printf("bye\n");
    return 0;
}

