#ifndef CMD_H
#define CMD_H

typedef struct {
    int argc;
    char **argv;
} cmd_ctx_t;

typedef int (*cmd_fn_t)(cmd_ctx_t *);

typedef struct {
    const char *name;
    cmd_fn_t fn;
} cmd_def_t;

extern const cmd_def_t command_table[];

#endif
