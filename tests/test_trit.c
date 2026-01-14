#include "test.h"
#include "trit.h"

int test_trit(void) {
    ASSERT(trit_encode(-1) == 0b10);
    ASSERT(trit_encode(0) == 0b00);
    ASSERT(trit_encode(1) == 0b01);

    ASSERT(trit_decode(0b10) == -1);
    ASSERT(trit_decode(0b01) == 1);
    ASSERT(trit_decode(0b00) == 0);

    ASSERT(trit_add(1, 1) == 1);
    ASSERT(trit_add(-1, -1) == -1);
    ASSERT(trit_add(1, -1) == 0);

    return 0;
}

