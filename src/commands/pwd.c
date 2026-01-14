#include "cmd.h"
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int cmd_pwd(cmd_ctx_t *ctx) {
    (void)ctx;
    char *cwd = getcwd(NULL, 0);
    if (cwd) {
        printf("%s\n", cwd);
        free(cwd);
    }
    return 0;
}
