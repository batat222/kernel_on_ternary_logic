#include <stddef.h>
#include "cmd.h"

int cmd_help(cmd_ctx_t *);
int cmd_ls(cmd_ctx_t *);
int cmd_cd(cmd_ctx_t *);
int cmd_pwd(cmd_ctx_t *);
int cmd_cp(cmd_ctx_t *);
int cmd_mv(cmd_ctx_t *);
int cmd_rm(cmd_ctx_t *);
int cmd_cat(cmd_ctx_t *);
int cmd_echo(cmd_ctx_t *);
int cmd_env(cmd_ctx_t *);
int cmd_set(cmd_ctx_t *);
int cmd_unset(cmd_ctx_t *);
int cmd_run(cmd_ctx_t *);
int cmd_exit(cmd_ctx_t *);

const cmd_def_t command_table[] = {
    {"help",  cmd_help},
    {"ls",    cmd_ls},
    {"cd",    cmd_cd},
    {"pwd",   cmd_pwd},
    {"cp",    cmd_cp},
    {"mv",    cmd_mv},
    {"rm",    cmd_rm},
    {"cat",   cmd_cat},
    {"echo",  cmd_echo},
    {"env",   cmd_env},
    {"set",   cmd_set},
    {"unset", cmd_unset},
    {"run",   cmd_run},
    {"exit",  cmd_exit},
    {NULL, NULL}
};
