#include "cmd.h"
#include <stdio.h>
extern char **environ;
int cmd_env(cmd_ctx_t *c) {
    (void)c;
    for (char **e = environ; *e; e++)
        printf("%s\n", *e);
    return 0;
}
