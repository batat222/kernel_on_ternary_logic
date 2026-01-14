#ifndef TERNLANG_H
#define TERNLANG_H

typedef struct {
    int *code;
    int code_len;
    char **strings;
    int str_count;
    char **vars;
    int var_count;
} CompiledProgram;

CompiledProgram *tern_compile(const char *src);
void tern_exec(CompiledProgram *prog);
void free_compiled(CompiledProgram *prog);

#endif

