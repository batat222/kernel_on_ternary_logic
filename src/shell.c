#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <limits.h>
#include "fs.h"
#include "ternlang.h"

/* Simple command shell for TernOS */
void shell_loop(void) {
    char line[256];
    char cwd[PATH_MAX];

    printf("Welcome to TernOS shell!\nType 'help' for available commands.\n\n");

    while (1) {
        if (getcwd(cwd, sizeof(cwd)) == NULL) {
            perror("getcwd");
            strcpy(cwd, "?");
        }

        printf("%s> ", cwd);
        fflush(stdout);

        if (!fgets(line, sizeof(line), stdin))
            break;  // EOF (Ctrl+D)

        line[strcspn(line, "\n")] = 0;  // Remove newline
        if (strlen(line) == 0)
            continue;

        /* Built-in commands */
        if (strcmp(line, "exit") == 0) {
            printf("Exiting shell.\n");
            break;
        } else if (strcmp(line, "help") == 0) {
            printf("Available commands:\n");
            printf("  ls [dir]   - list files\n");
            printf("  cd <dir>   - change directory\n");
            printf("  pwd        - print current directory\n");
            printf("  run <file> - run a .tl program\n");
            printf("  exit       - quit shell\n");
            continue;
        } else if (strncmp(line, "ls", 2) == 0) {
            char *arg = line + 2;
            while (*arg == ' ') arg++;
            fs_list(strlen(arg) ? arg : ".");
        } else if (strncmp(line, "cd ", 3) == 0) {
            const char *dir = line + 3;
            if (chdir(dir) != 0)
                perror("cd");
        } else if (strcmp(line, "pwd") == 0) {
            if (getcwd(cwd, sizeof(cwd)))
                printf("%s\n", cwd);
        } else if (strncmp(line, "run ", 4) == 0) {
            const char *path = line + 4;
            char *code = fs_read(path);
            if (!code) {
                printf("Could not read file: %s\n", path);
                continue;
            }
            printf("[Running %s]\n", path);

            /* compile and execute TL code */
            CompiledProgram *prog = tern_compile(code);
            if (prog)
                tern_exec(prog);
            else
                printf("Failed to compile %s\n", path);

            free(code);
        } else {
            printf("Unknown command: %s\n", line);
            printf("Use 'help' to get list of available commands.\n");
        }
    }
}

