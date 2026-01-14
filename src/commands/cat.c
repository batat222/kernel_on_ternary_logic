#include "cmd.h"
#include "fs.h"
int cmd_cat(cmd_ctx_t *c) {
    if (c->argc < 2) return 1;
    return fs_cat(c->argv[1]);
}
