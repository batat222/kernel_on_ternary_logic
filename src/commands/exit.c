#include "cmd.h"
#include <stdlib.h>

int cmd_exit(cmd_ctx_t *ctx) {
    (void)ctx;
    exit(0);
}
