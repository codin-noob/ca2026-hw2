#include "runtime.h"

/*
 * RV32I has no multiply: GCC lowers 32-bit '*' to __mulsi3 and 64-bit '*'
 * to __muldi3. Shift-and-add, like long multiplication in base 2.
 */

uint32_t __mulsi3(uint32_t a, uint32_t b)
{
    uint32_t r = 0;
    while (b) {
        if (b & 1)
            r += a;
        a <<= 1;
        b >>= 1;
    }
    return r;
}

uint64_t __muldi3(uint64_t a, uint64_t b)
{
    uint64_t r = 0;
    while (b) {
        if (b & 1)
            r += a;
        a <<= 1; /* constant shift: inlined, no __ashldi3 */
        b >>= 1;
    }
    return r;
}
