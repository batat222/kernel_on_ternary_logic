#include "cmd.h"
#include <stdio.h>

int cmd_help(cmd_ctx_t *ctx) {
    (void)ctx;
    for (int i = 0; command_table[i].name; i++)
        printf("%s\n", command_table[i].name);
    return 0;
}
