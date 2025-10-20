#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "ternlang.h"

/*
Tiny compiler from a small TernLang subset to bytecode.

Language:
 - Lines separated by newline or ';'
 - print(<expr_or_string>)
 - <ident> = <expr>
 - expr := term { + term }
 - term := integer | ident

Bytecode (int words):
 OP_PUSH_IMM  1 <value>
 OP_LOAD_VAR  2 <var_index>
 OP_STORE_VAR 3 <var_index>
 OP_ADD       4
 OP_SUB       5
 OP_PRINT_INT 6
 OP_PRINT_STR 7 <string_index>
 OP_HALT      9

We also emit string table and variable table in the CompiledProgram struct.
*/

static char *dupstr_local(const char *s) {
    if (!s) return NULL;
    char *d = malloc(strlen(s) + 1);
    if (d) strcpy(d, s);
    return d;
}

typedef struct {
    char **items;
    int count;
    int cap;
} StrList;

static void sl_init(StrList *s) { s->items = NULL; s->count = 0; s->cap = 0; }
static void sl_push(StrList *s, char *x) {
    if (s->count == s->cap) {
        s->cap = s->cap ? s->cap * 2 : 8;
        s->items = realloc(s->items, sizeof(char*) * s->cap);
    }
    s->items[s->count++] = x;
}
static void sl_free(StrList *s) { for (int i=0;i<s->count;i++) free(s->items[i]); free(s->items); s->items = NULL; s->count = s->cap = 0; }

/* variable list */
typedef struct {
    char **names; int count; int cap;
} VarList;
static void vl_init(VarList *v){ v->names = NULL; v->count = v->cap = 0; }
static int vl_index(VarList *v, const char *name) {
    for (int i=0;i<v->count;i++) if (strcmp(v->names[i], name)==0) return i;
    // add
    if (v->count == v->cap) { v->cap = v->cap ? v->cap*2 : 8; v->names = realloc(v->names, sizeof(char*) * v->cap); }
    v->names[v->count] = dupstr_local(name);
    return v->count++;
}
static void vl_free(VarList *v){ for (int i=0;i<v->count;i++) free(v->names[i]); free(v->names); v->names = NULL; v->count = v->cap = 0; }

/* token helpers */
static char *trim(char *s) {
    while (*s && isspace((unsigned char)*s)) s++;
    char *e = s + strlen(s) - 1;
    while (e >= s && isspace((unsigned char)*e)) { *e = 0; e--; }
    return s;
}

/* parse integer or variable; emit code that pushes the value (PUSH_IMM or LOAD_VAR), returns 0 on success */
static void emit_term(const char *tok, int **codep, int *szp, VarList *vars) {
    if (!tok) return;
    char *t = (char*)tok;
    while (*t && isspace((unsigned char)*t)) t++;
    if (*t == '\0') return;
    // number?
    char *endptr = NULL;
    long val = strtol(t, &endptr, 10);
    if (endptr != t && (*endptr == 0 || isspace((unsigned char)*endptr))) {
        // literal
        *codep = realloc(*codep, sizeof(int) * (*szp + 2));
        (*codep)[(*szp)++] = 1; // PUSH_IMM
        (*codep)[(*szp)++] = (int)val;
        return;
    }
    // else identifier
    int idx = vl_index(vars, t);
    *codep = realloc(*codep, sizeof(int) * (*szp + 2));
    (*codep)[(*szp)++] = 2; // LOAD_VAR
    (*codep)[(*szp)++] = idx;
}

/* parse expression with + chain */
static void emit_expr(char *expr, int **codep, int *szp, VarList *vars) {
    // split by '+'
    char *p = expr;
    char *start = p;
    while (1) {
        char *plus = strchr(start, '+');
        if (plus) {
            *plus = 0;
            char *tok = trim(start);
            emit_term(tok, codep, szp, vars);
            // after pushing two terms, we emit ADD when second term is pushed later; but we need to handle sequence:
            // We'll maintain: for first term no ADD. For each additional term: push term then emit ADD.
            // To implement that, we need to count terms. Simpler: do left-associative: parse first term then for each plus: parse next term then emit ADD.
            // So implement differently below.
            start = plus + 1;
        } else {
            break;
        }
    }
    // Simpler robust approach: tokenise by '+' into array
    // Build tokens list:
    char *s = expr;
    // normalize: replace '+' with '\0' while collecting tokens
    char *tmp = strdup(expr);
    char *t = tmp;
    char *tokens[64]; int tcount = 0;
    char *tok = strtok(t, "+");
    while (tok && tcount < 64) { tokens[tcount++] = trim(tok); tok = strtok(NULL, "+"); }
    if (tcount == 0) { free(tmp); return; }
    // emit first term
    emit_term(tokens[0], codep, szp, vars);
    for (int i=1;i<tcount;i++) {
        emit_term(tokens[i], codep, szp, vars);
        // now add
        *codep = realloc(*codep, sizeof(int) * (*szp + 1));
        (*codep)[(*szp)++] = 4; // ADD
    }
    free(tmp);
}

/* compile source into CompiledProgram */
CompiledProgram *tern_compile(const char *src) {
    CompiledProgram *p = malloc(sizeof(CompiledProgram));
    p->code = NULL; p->code_len = 0;
    p->strings = NULL; p->str_count = 0;
    p->vars = NULL; p->var_count = 0;

    StrList sl; sl_init(&sl);
    VarList vl; vl_init(&vl);

    // split into lines
    char *copy = strdup(src);
    char *line = strtok(copy, "\n");
    while (line) {
        char *s = trim(line);
        if (*s == '#' || *s == '\0') { line = strtok(NULL, "\n"); continue; }
        // handle print(...)
        if (strncmp(s, "print", 5) == 0) {
            char *lp = strchr(s, '(');
            char *rp = strrchr(s, ')');
            if (!lp || !rp || rp <= lp) { line = strtok(NULL, "\n"); continue; }
            char argbuf[1024]; strncpy(argbuf, lp+1, (size_t)(rp - lp - 1)); argbuf[rp-lp-1] = 0;
            char *arg = trim(argbuf);
            if (*arg == '"' ) {
                // string literal
                char *startq = arg + 1;
                char *endq = strrchr(startq, '"');
                if (!endq) { line = strtok(NULL, "\n"); continue; }
                *endq = 0;
                // add string to sl
                sl_push(&sl, dupstr_local(startq));
                // emit PRINT_STR index
                p->code = realloc(p->code, sizeof(int) * (p->code_len + 2));
                p->code[p->code_len++] = 7; // PRINT_STR
                p->code[p->code_len++] = sl.count - 1; // index
            } else {
                // expression -> compile to code that leaves value on stack, then PRINT_INT
                emit_expr(arg, &p->code, &p->code_len, &vl);
                p->code = realloc(p->code, sizeof(int) * (p->code_len + 1));
                p->code[p->code_len++] = 6; // PRINT_INT
            }
        } else {
            // assignment: name = expr
            // find '='
            char *eq = strchr(s, '=');
            if (eq) {
                *eq = 0;
                char *name = trim(s);
                char exprbuf[1024]; strcpy(exprbuf, trim(eq + 1));
                // ensure var exists
                int vidx = vl_index(&vl, name);
                // compile expr -> leaves value on stack; then STORE_VAR vidx
                emit_expr(exprbuf, &p->code, &p->code_len, &vl);
                p->code = realloc(p->code, sizeof(int) * (p->code_len + 2));
                p->code[p->code_len++] = 3; // STORE_VAR
                p->code[p->code_len++] = vidx;
            }
        }
        line = strtok(NULL, "\n");
    }
    // HALT
    p->code = realloc(p->code, sizeof(int) * (p->code_len + 1));
    p->code[p->code_len++] = 9;

    // finalize string list and var list into the CompiledProgram
    if (sl.count) {
        p->strings = malloc(sizeof(char*) * sl.count);
        for (int i=0;i<sl.count;i++) p->strings[i] = sl.items[i];
        p->str_count = sl.count;
    } else {
        p->strings = NULL; p->str_count = 0;
        sl_free(&sl);
    }
    if (vl.count) {
        p->vars = malloc(sizeof(char*) * vl.count);
        for (int i=0;i<vl.count;i++) p->vars[i] = vl.names[i];
        p->var_count = vl.count;
    } else {
        p->vars = NULL; p->var_count = 0;
        vl_free(&vl);
    }

    free(copy);
    return p;
}

void free_compiled(CompiledProgram *p) {
    if (!p) return;
    if (p->code) free(p->code);
    if (p->strings) { for (int i=0;i<p->str_count;i++) free(p->strings[i]); free(p->strings); }
    if (p->vars) { for (int i=0;i<p->var_count;i++) free(p->vars[i]); free(p->vars); }
    free(p);
}

