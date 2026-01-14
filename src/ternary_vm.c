#include <stdio.h>
#include <stdlib.h>
#include "ternary_vm.h"
#include "trit.h"

void ternary_run(CompiledProgram *prog, VMMode mode) {
    if (!prog) return;

    int pc = 0;
    int stack[1024];
    int sp = 0;

    int *vars = calloc(prog->var_count, sizeof(int));
    if (!vars) return;

    for (;;) {
        int op = prog->code[pc++];

        switch (op) {
            case 1: /* PUSH_IMM */
                stack[sp++] = prog->code[pc++];
                break;

            case 2: /* LOAD_VAR */
                stack[sp++] = vars[prog->code[pc++]];
                break;

            case 3: /* STORE_VAR */
                vars[prog->code[pc++]] = stack[--sp];
                break;

            case 4: { /* ADD */
                int b = stack[--sp];
                int a = stack[--sp];
                if (mode == VM_MODE_TRIT)
                    stack[sp++] = trit_add(a, b);
                else
                    stack[sp++] = a + b;
                break;
            }

            case 6: /* PRINT_INT */
                printf("%d\n", stack[--sp]);
                break;

            case 7: /* PRINT_STR */
                printf("%s\n", prog->strings[prog->code[pc++]]);
                break;

            case 9: /* HALT */
                free(vars);
                return;

            default:
                fprintf(stderr, "VM error: unknown opcode %d\n", op);
                free(vars);
                return;
        }
    }
}

