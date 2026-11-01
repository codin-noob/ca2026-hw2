#include "runtime.h"

int main(void)
{
    volatile uint32_t x = 12345, y = 6789;
    volatile uint64_t big = 0xFFFFFFFFULL;
    char a[8] = "abcdefg", b[8];

    print_str("hello from arch-hw2\n");

    print_str("u64 max   = "); print_u64(18446744073709551615ULL); print_char('\n');
    print_str("i64 min   = "); print_i64(-9223372036854775807LL - 1); print_char('\n');
    print_str("zero      = "); print_u64(0); print_char('\n');
    print_str("hex       = "); print_hex32(0xDEADBEEF); print_char('\n');
    print_str("mulsi3    = "); print_u32(x * y); print_char('\n');
    print_str("muldi3    = "); print_u64(big * big); print_char('\n');

    memcpy(b, a, sizeof a);
    print_str("memcpy    = "); print_str(b); print_char('\n');

    uint64_t c0 = get_cycles(), c1 = get_cycles();
    print_str("cycles ok = "); print_u32(c1 > c0); print_char('\n');

    return 0;
}
