#include <stddef.h>
#include "cmd.h"
#include "fs.h"

int cmd_ls(cmd_ctx_t *ctx) {
    fs_list(ctx->argc > 1 ? ctx->argv[1] : NULL);
    return 0;
}
