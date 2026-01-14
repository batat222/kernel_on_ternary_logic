#ifndef TRIT_H
#define TRIT_H

typedef enum {
    TRIT_NEG  = -1,
    TRIT_ZERO =  0,
    TRIT_POS  =  1
} trit_t;

trit_t trit_add(trit_t a, trit_t b);
trit_t trit_neg(trit_t a);

#endif

