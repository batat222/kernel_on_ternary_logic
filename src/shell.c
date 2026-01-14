#include "shell.h"
#include "cmd.h"
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

static int split(char *line, char **argv) {
    int argc = 0;
    char *t = strtok(line, " \t\n");
    while (t && argc < 63) {
        argv[argc++] = t;
        t = strtok(NULL, " \t\n");
    }
    argv[argc] = NULL;
    return argc;
}

static void execute(int argc, char **argv) {
    if (argc == 0) return;

    cmd_ctx_t ctx = { argc, argv };

    for (int i = 0; command_table[i].name; i++) {
        if (strcmp(argv[0], command_table[i].name) == 0) {
            command_table[i].fn(&ctx);
            return;
        }
    }
    printf("unknown command: %s\n", argv[0]);
}

void shell_run(void) {
    char line[1024];
    char *argv[64];

    while (1) {
        char *cwd = getcwd(NULL, 0);
        printf("[ternos]%s$ ", cwd ? cwd : "");
        free(cwd);

        if (!fgets(line, sizeof(line), stdin))
            break;

        int argc = split(line, argv);
        execute(argc, argv);
    }
}
