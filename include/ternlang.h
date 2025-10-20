#ifndef TERNLANG_H
#define TERNLANG_H

/* Compiled program structure returned by compiler */
typedef struct {
    int *code;          /* integer code words */
    int code_len;
    char **strings;     /* string table */
    int str_count;
    char **vars;        /* variable names in order */
    int var_count;
} CompiledProgram;

/* compile source -> CompiledProgram* (caller must free with free_compiled) */
CompiledProgram *tern_compile(const char *src);

/* free compiled program */
void free_compiled(CompiledProgram *p);

#endif

