# ternOS — experimental ternary-based shell and VM

This repository contains an experimental UNIX-like shell and execution environment built around **balanced ternary logic**.

The project includes:
- A minimal interactive shell
- Modular built-in commands
- A ternary arithmetic core (trits)
- A simple virtual machine
- TBC (Ternary ByteCode) execution via `run` command
- Tests and valgrind support

The goal is to explore low-level system design, ternary computation, and clean C architecture without relying on external shells or runtimes.

---

## Project structure

.
├── include/        # Public headers
├── src/            # Core sources
│   ├── commands/   # Built-in shell commands
│   └── ...
├── tests/          # Unit tests
├── docs/           # Documentation (TBC spec, etc.)
├── build/          # Build artifacts
├── Makefile
└── README.md

---

## Features

- Interactive shell (`ternos`)
- Built-in commands:
  - cat, cd, cp, echo, env
  - exit, help, ls, mv
  - pwd, rm, run, set, unset
- Environment variable handling
- POSIX filesystem operations
- Ternary arithmetic primitives
- Bytecode execution (TBC)
- Valgrind memory testing

---

## Build

```bash
make
````

---

## Run

```bash
./ternos
```

---

## Tests

```bash
make tests
```

---

## Memory check

```bash
make memtest
```

---

## Requirements

* GCC (C99)
* POSIX-compatible system
* valgrind (optional, for memtest)

Tested on Linux.

---

## Notes

This project is experimental and intended for learning and research purposes.
The architecture favors clarity and correctness over performance.
