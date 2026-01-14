#include "trit.h"

trit_t trit_add(trit_t a, trit_t b) {
    int s = a + b;
    if (s > 1)  return TRIT_NEG;
    if (s < -1) return TRIT_POS;
    return (trit_t)s;
}

trit_t trit_neg(trit_t a) {
    return (trit_t)(-a);
}
