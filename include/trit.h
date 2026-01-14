#ifndef TRIT_H
#define TRIT_H

#include <stdint.h>

/*
 Binary encoding of ternary trit:
   -1 → 0b10
    0 → 0b00
   +1 → 0b01
*/

typedef int8_t trit_t;

#define TRIT_NEG  (-1)
#define TRIT_ZERO (0)
#define TRIT_POS  (1)

uint8_t trit_encode(trit_t t);
trit_t  trit_decode(uint8_t b);
trit_t  trit_add(trit_t a, trit_t b);
int     trit_is_valid(trit_t t);

#endif

