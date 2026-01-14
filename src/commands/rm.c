#include "cmd.h"
#include "fs.h"
int cmd_rm(cmd_ctx_t *c) {
    if (c->argc < 2) return 1;
    return fs_remove(c->argv[1]);
}
