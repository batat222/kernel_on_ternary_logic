#include "trit.h"

uint8_t trit_encode(trit_t t) {
    switch (t) {
        case TRIT_NEG:  return 0b10;
        case TRIT_ZERO: return 0b00;
        case TRIT_POS:  return 0b01;
        default:        return 0b00;
    }
}

trit_t trit_decode(uint8_t b) {
    switch (b & 0b11) {
        case 0b10: return TRIT_NEG;
        case 0b01: return TRIT_POS;
        default:   return TRIT_ZERO;
    }
}

int trit_is_valid(trit_t t) {
    return (t == -1 || t == 0 || t == 1);
}

trit_t trit_add(trit_t a, trit_t b) {
    int s = a + b;
    if (s > 1)  return 1;
    if (s < -1) return -1;
    return (trit_t)s;
}

