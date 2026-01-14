#include "cmd.h"
#include "fs.h"
int cmd_cp(cmd_ctx_t *c) {
    if (c->argc < 3) return 1;
    return fs_copy(c->argv[1], c->argv[2]);
}
