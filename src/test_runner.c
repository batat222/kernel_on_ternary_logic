#include <stdio.h>
#include "test.h"

int test_trit(void);

int test_run_all(void) {
    if (test_trit()) return 1;
    return 0;
}

int main(void) {
    return test_run_all();
}

