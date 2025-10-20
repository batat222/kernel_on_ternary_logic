#include <stdio.h>
#include <stdlib.h>
#include "ternary_vm.h"

/*
Bytecode conventions (see ternlang.c):
 1 PUSH_IMM <value>
 2 LOAD_VAR <var_index>
 3 STORE_VAR <var_index>
 4 ADD
 5 SUB
 6 PRINT_INT
 7 PRINT_STR <str_index>
 9 HALT
*/

static int trit_wrap(int v) {
    /* We will treat integers as standard ints but wrap only to ternary-ish -1/0/1 for small sums.
       However TernLang supports arbitrary integers; the VM will print them as-is.
       To maintain previous simple ternary behavior for + of -1/0/1, we can wrap to -1,0,1
       when operands are in [-1,1].
    */
    return v; /* leave full integer semantics for now */
}

void ternary_run(CompiledProgram *prog) {
    if (!prog) return;
    int *code = prog->code;
    int pc = 0;
    int stack[1024];
    int sp = 0;
    /* variable storage: allocate slots for all variables */
    int var_count = prog->var_count;
    int *vars = NULL;
    if (var_count > 0) {
        vars = calloc(var_count, sizeof(int));
    }
    for (;;) {
        int op = code[pc++];
        switch (op) {
            case 1: { /* PUSH_IMM */
                int v = code[pc++]; stack[sp++] = v; break;
            }
            case 2: { /* LOAD_VAR */
                int idx = code[pc++]; int val = 0; if (idx >= 0 && idx < var_count) val = vars[idx]; stack[sp++] = val; break;
            }
            case 3: { /* STORE_VAR */
                int idx = code[pc++]; if (sp <= 0) { fprintf(stderr, "stack underflow\n"); goto end; } int v = stack[--sp]; if (idx >= 0 && idx < var_count) vars[idx] = v; break;
            }
            case 4: { /* ADD */
                if (sp < 2) { fprintf(stderr, "stack underflow\n"); goto end; }
                int b = stack[--sp]; int a = stack[--sp]; int r = a + b; /* optionally wrap to ternary small domain */
                stack[sp++] = trit_wrap(r);
                break;
            }
            case 5: { /* SUB */
                if (sp < 2) { fprintf(stderr, "stack underflow\n"); goto end; }
                int b = stack[--sp]; int a = stack[--sp]; int r = a - b;
                stack[sp++] = trit_wrap(r);
                break;
            }
            case 6: { /* PRINT_INT */
                if (sp <= 0) { fprintf(stderr, "stack underflow\n"); goto end; }
                int v = stack[--sp];
                printf("%d\n", v);
                break;
            }
            case 7: { /* PRINT_STR */
                int sidx = code[pc++]; if (sidx >= 0 && sidx < prog->str_count) printf("%s\n", prog->strings[sidx]); else printf("(null)\n");
                break;
            }
            case 9: goto end;
            default: fprintf(stderr, "unknown opcode %d at pc=%d\n", op, pc-1); goto end;
        }
    }
end:
    if (vars) free(vars);
}

