#include <stdlib.h>
#include "cmd.h"

int cmd_set(cmd_ctx_t *c) {
    if (c->argc < 3) return 1;
    return setenv(c->argv[1], c->argv[2], 1);
}
