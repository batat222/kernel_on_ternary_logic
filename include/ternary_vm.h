#ifndef TERNARY_VM_H
#define TERNARY_VM_H

#include "ternlang.h"

/* Run a compiled program in the ternary VM.
   Variables are initialized to 0. */
void ternary_run(CompiledProgram *prog);

#endif

