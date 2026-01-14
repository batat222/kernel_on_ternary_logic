#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <limits.h>

#include "fs.h"
#include "ternlang.h"
#include "ternary_vm.h"

static VMMode vm_mode = VM_MODE_INT;

void shell_loop(void) {
    char line[256];
    char cwd[PATH_MAX];

    while (1) {
        getcwd(cwd, sizeof(cwd));
        printf("%s> ", cwd);

        if (!fgets(line, sizeof(line), stdin))
            break;

        line[strcspn(line, "\n")] = 0;

        if (!strcmp(line, "exit"))
            break;

        if (!strcmp(line, "mode trit")) {
            vm_mode = VM_MODE_TRIT;
            continue;
        }

        if (!strcmp(line, "mode int")) {
            vm_mode = VM_MODE_INT;
            continue;
        }

        if (!strncmp(line, "run ", 4)) {
            char *src = fs_read(line + 4);
            if (!src) continue;
            CompiledProgram *p = tern_compile(src);
            ternary_run(p, vm_mode);
            free_compiled(p);
            free(src);
        }
    }
}

