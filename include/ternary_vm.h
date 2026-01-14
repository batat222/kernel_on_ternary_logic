#ifndef TERNARY_VM_H
#define TERNARY_VM_H

#include "ternlang.h"

typedef enum {
    VM_MODE_INT,
    VM_MODE_TRIT
} VMMode;

void ternary_run(CompiledProgram *prog, VMMode mode);

#endif

