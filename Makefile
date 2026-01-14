CC = gcc
CFLAGS = -std=c99 -Wall -Wextra -Iinclude -D_POSIX_C_SOURCE=200809L

BIN = ternos

SRC = \
	src/main.c \
	src/shell.c \
	src/fs.c \
	src/trit.c \
	src/command_table.c

CMD_SRC = $(wildcard src/commands/*.c)

OBJ = $(SRC:src/%.c=build/%.o) \
      $(CMD_SRC:src/commands/%.c=build/commands/%.o)

TEST_SRC = \
	tests/test_runner.c \
	tests/test_trit.c

TEST_OBJ = $(TEST_SRC:tests/%.c=build/tests/%.o)

.PHONY: all clean tests memtest

all: $(BIN)

$(BIN): $(OBJ)
	$(CC) $(CFLAGS) $^ -o $@

build/%.o: src/%.c
	mkdir -p build
	$(CC) $(CFLAGS) -c $< -o $@

build/commands/%.o: src/commands/%.c
	mkdir -p build/commands
	$(CC) $(CFLAGS) -c $< -o $@

# ===== TESTS =====

build/tests/%.o: tests/%.c
	mkdir -p build/tests
	$(CC) $(CFLAGS) -c $< -o $@

build/tests/test_runner: $(TEST_OBJ) build/trit.o
	$(CC) $(CFLAGS) $^ -o $@

tests: build/tests/test_runner
	./build/tests/test_runner

memtest: build/tests/test_runner
	valgrind --leak-check=full ./build/tests/test_runner

clean:
	rm -rf build $(BIN)

