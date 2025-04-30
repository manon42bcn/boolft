
#include "boolft.h"

/**
 * @brief Multiplies two unsigned integers using bitwise operations.
 *
 * @note How it works?
 *  The way that this function works is quite intuitive after you get it.
 *  Basically you start to add N times, but with a twist. Let's check it out:
 *
 *  a x b -> 2 x 3 -> 010b x 011b
 *  rst = 000b
 *  1-> b lest significant bit is 1, so with use adder to plus rst and a.
 *      rst + a -> 0 + 2 -> 000b + 010b = 010b.
 *      Then a will be shifted by 1 to the left (increasing its magnitude).
 *      and b will be shifted by 1 to the right (decreasing its magnitude).
 *      Due to a and b are written in 2 base, increase and decrease its magnitude
 *      means multiple or divide by 2.
 *  2-> a = 100b; b = 001b; b last bit is 1, so:
 *      rst + a -> 010b + 100b (2 + 4) = 100b.
 *      a <<= 1 = 1000b (won't be used).
 *      b <<= 1 = 0000b and the loop ends.
 *
 * @param a The multiplicand — the number to be added multiple times.
 * @param b The multiplier — controls how many times `a` is added.
 * @return The product of `a` and `b`.
 *
 */
unsigned int multiplier(unsigned int a, unsigned int b) {
    unsigned int rst = 0;
    while (b != 0) {
        if (b & 1) {
            rst = adder(rst, a);
        }
        a <<= 1;
        b >>= 1;
    }
    return rst;
}
