CC = gcc
CFLAGS = -std=c99 -Wall -Wextra -Iinclude

SRC   = src
TESTS = tests
BUILD = build

CORE_SRCS = fs.c shell.c ternlang.c ternary_vm.c trit.c
CORE_OBJS = $(CORE_SRCS:%.c=$(BUILD)/%.o)

TEST_SRCS = test_trit.c
TEST_OBJS = $(TEST_SRCS:%.c=$(BUILD)/%.o)

all: ternos

$(BUILD):
	mkdir -p $(BUILD)

$(BUILD)/%.o: $(SRC)/%.c | $(BUILD)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD)/%.o: $(TESTS)/%.c | $(BUILD)
	$(CC) $(CFLAGS) -c $< -o $@

ternos: $(CORE_OBJS) $(BUILD)/main.o
	$(CC) $(CFLAGS) $^ -o $@

tests: $(CORE_OBJS) $(TEST_OBJS) $(BUILD)/test_runner.o
	$(CC) $(CFLAGS) $^ -o test_runner
	./test_runner

memtest: tests
	valgrind --leak-check=full --error-exitcode=1 ./test_runner

clean:
	rm -rf $(BUILD) ternos test_runner

