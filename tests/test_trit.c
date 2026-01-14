#include <stdio.h>
#include "trit.h"

static int fails = 0;

#define ASSERT_EQ(a,b) \
    if ((a) != (b)) { \
        printf("FAIL %s:%d\n", __FILE__, __LINE__); \
        fails++; \
    }

int test_trit(void) {
    /* neg */
    ASSERT_EQ(trit_neg(TRIT_POS), TRIT_NEG);
    ASSERT_EQ(trit_neg(TRIT_NEG), TRIT_POS);
    ASSERT_EQ(trit_neg(TRIT_ZERO), TRIT_ZERO);

    /* add */
    ASSERT_EQ(trit_add(TRIT_POS, TRIT_POS), TRIT_NEG);
    ASSERT_EQ(trit_add(TRIT_POS, TRIT_NEG), TRIT_ZERO);
    ASSERT_EQ(trit_add(TRIT_NEG, TRIT_NEG), TRIT_POS);

    return fails;
}

