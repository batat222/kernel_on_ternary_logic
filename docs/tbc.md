# TBC — Ternary ByteCode Specification

**TBC** is a simple, deterministic bytecode format designed for **ternOS**.
It is intended as an intermediate execution format between the shell (`run`) and future higher-level languages.

TBC is:

* Stack-based
* Little-endian
* Deterministic
* Host-independent
* VM-executed

---

## 1. Design Goals

* Extremely small instruction set
* Easy to parse and validate
* Friendly to future assemblers / compilers
* Portable across architectures
* Suitable for ternary logic experimentation

Non-goals:

* Self-modifying code
* JIT
* Dynamic linking
* Undefined behavior tolerance

---

## 2. File Format

### 2.1 Layout

```
+------------------+
| Magic "TBC1"     | 4 bytes
+------------------+
| Code length      | uint32
+------------------+
| Bytecode         | N bytes
+------------------+
```

### 2.2 Endianness

* Little-endian

### 2.3 Validation Rules

* Magic must match exactly
* Code length must match file size
* Invalid opcode → immediate termination

---

## 3. Execution Model

* Stack-based VM
* Single thread
* Single stack
* No heap (yet)
* Linear execution
* No self-jumps (yet)

---

## 4. Data Types

### 4.1 Trit

A **trit** is a balanced ternary digit:

| Name      | Value |
| --------- | ----- |
| TRIT_NEG  | -1    |
| TRIT_ZERO | 0     |
| TRIT_POS  | +1    |

Encoded as signed 8-bit integer.

---

### 4.2 Strings

* UTF-8
* Length-prefixed
* No null terminator

```
[u16 length][bytes...]
```

---

## 5. Instruction Format

Each instruction begins with a **1-byte opcode**.

Some instructions are followed by arguments.

```
[opcode][operands...]
```

---

## 6. Instruction Set

### 6.1 Process Control

#### `OP_EXIT`

```
Opcode: 0x00
Operands: none
```

Terminates program execution.

Exit code:

* Top of stack if present
* Otherwise `0`

---

### 6.2 Stack Operations

#### `OP_PUSH_TRIT`

```
Opcode: 0x01
Operands:
  int8 value
```

Pushes a trit onto the stack.

Valid values: `-1`, `0`, `1`

---

#### `OP_POP`

```
Opcode: 0x02
Operands: none
```

Removes top stack element.

---

### 6.3 Arithmetic (Ternary)

#### `OP_ADD`

```
Opcode: 0x10
Operands: none
```

Pops two trits and pushes result of balanced ternary addition.

---

#### `OP_NEG`

```
Opcode: 0x11
Operands: none
```

Negates top trit.

---

### 6.4 I/O

#### `OP_PRINT`

```
Opcode: 0x20
Operands:
  string
```

Writes string to standard output.

---

### 6.5 Environment

#### `OP_GETENV`

```
Opcode: 0x30
Operands:
  string (name)
```

Pushes environment variable value onto stack as string.

If not found:

* Pushes empty string

---

## 7. Example Programs

### 7.1 Hello World

```
TBC1
OP_PRINT "Hello from ternOS\n"
OP_EXIT
```

---

### 7.2 Ternary Math

```
PUSH_TRIT 1
PUSH_TRIT 1
ADD
EXIT
```

Result on exit: `-1`

---

### 7.3 Environment Dump

```
GETENV "PATH"
PRINT
EXIT
```

---

## 8. Error Handling

| Condition       | Behavior                |
| --------------- | ----------------------- |
| Invalid opcode  | Abort execution         |
| Stack underflow | Abort                   |
| Invalid trit    | Abort                   |
| Truncated file  | Reject before execution |

---

## 9. Versioning

* Current version: **TBC1**
* Forward incompatible by default
* Future versions must change magic

---

## 10. Roadmap (Explicitly Non-Binding)

* Conditional jumps
* Registers
* Heap allocation
* Syscalls
* ELF compatibility layer

---

## 11. Philosophy

TBC exists to:

* Bootstrap a system
* Enable experimentation
* Avoid premature complexity

It is **not** a replacement for native machine code.

---

### End of Specification

If you want next:

* Reference VM implementation
* Assembler syntax
* Bytecode encoder
* Test programs
* Formal grammar

Say the word.

