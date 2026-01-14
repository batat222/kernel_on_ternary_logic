#include <stdio.h>

int test_trit(void);

int main(void) {
    int fails = 0;

    fails += test_trit();

    if (fails == 0) {
        printf("[OK] all tests passed\n");
        return 0;
    } else {
        printf("[FAIL] %d tests failed\n", fails);
        return 1;
    }
}

