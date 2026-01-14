#include "cmd.h"
#include <stdlib.h>
int cmd_unset(cmd_ctx_t *c) {
    if (c->argc < 2) return 1;
    return unsetenv(c->argv[1]);
}
