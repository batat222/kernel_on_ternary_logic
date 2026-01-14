#include "cmd.h"
#include <stdio.h>
int cmd_run(cmd_ctx_t *c) {
    if (c->argc < 2) return 1;
    printf("run ternlang: %s (stub)\n", c->argv[1]);
    return 0;
}
