#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "ternlang.h"
#include "ternary_vm.h"

/* utility */
static char *dupstr(const char *s) {
    char *d = malloc(strlen(s) + 1);
    if (d) strcpy(d, s);
    return d;
}

static char *trim(char *s) {
    while (isspace((unsigned char)*s)) s++;
    char *e = s + strlen(s) - 1;
    while (e >= s && isspace((unsigned char)*e)) *e-- = 0;
    return s;
}

/* variable table */
typedef struct {
    char **names;
    int count, cap;
} VarTable;

static int var_index(VarTable *t, const char *n) {
    for (int i = 0; i < t->count; i++)
        if (!strcmp(t->names[i], n))
            return i;

    if (t->count == t->cap) {
        t->cap = t->cap ? t->cap * 2 : 8;
        t->names = realloc(t->names, sizeof(char*) * t->cap);
    }
    t->names[t->count] = dupstr(n);
    return t->count++;
}

static void emit_push(int **code, int *len, int v) {
    *code = realloc(*code, sizeof(int) * (*len + 2));
    (*code)[(*len)++] = 1; /* PUSH_IMM */
    (*code)[(*len)++] = v;
}

CompiledProgram *tern_compile(const char *src) {
    CompiledProgram *p = calloc(1, sizeof(*p));
    VarTable vars = {0};

    char *copy = dupstr(src);
    char *line = strtok(copy, "\n");

    while (line) {
        char *s = trim(line);

        if (!strncmp(s, "print(", 6)) {
            char *arg = strchr(s, '(') + 1;
            char *end = strrchr(arg, ')');
            if (!end) goto next;
            *end = 0;

            int v = atoi(arg);
            emit_push(&p->code, &p->code_len, v);

            p->code = realloc(p->code, sizeof(int) * (p->code_len + 1));
            p->code[p->code_len++] = 6; /* PRINT_INT */
        }
        else if (strchr(s, '=')) {
            char *eq = strchr(s, '=');
            *eq = 0;

            int idx = var_index(&vars, trim(s));
            int val = atoi(trim(eq + 1));

            emit_push(&p->code, &p->code_len, val);

            p->code = realloc(p->code, sizeof(int) * (p->code_len + 2));
            p->code[p->code_len++] = 3; /* STORE_VAR */
            p->code[p->code_len++] = idx;
        }

    next:
        line = strtok(NULL, "\n");
    }

    p->code = realloc(p->code, sizeof(int) * (p->code_len + 1));
    p->code[p->code_len++] = 9; /* HALT */

    p->vars = vars.names;
    p->var_count = vars.count;

    free(copy);
    return p;
}

void tern_exec(CompiledProgram *prog) {
    ternary_run(prog, VM_MODE_INT);
}

void free_compiled(CompiledProgram *p) {
    if (!p) return;
    free(p->code);
    for (int i = 0; i < p->var_count; i++)
        free(p->vars[i]);
    free(p->vars);
    free(p);
}

