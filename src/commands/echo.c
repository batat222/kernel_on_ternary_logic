#include "cmd.h"
#include <stdio.h>
int cmd_echo(cmd_ctx_t *c) {
    for (int i = 1; i < c->argc; i++)
        printf("%s ", c->argv[i]);
    printf("\n");
    return 0;
}
