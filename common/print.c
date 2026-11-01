#include "runtime.h"

long sys_write(int fd, const void *buf, size_t len)
{
    register long a0 asm("a0") = fd;
    register const void *a1 asm("a1") = buf;
    register size_t a2 asm("a2") = len;
    register long a7 asm("a7") = 64; /* write */
    /* "memory": the ecall reads buf, so pending stores must land first. */
    asm volatile("ecall" : "+r"(a0) : "r"(a1), "r"(a2), "r"(a7) : "memory");
    return a0;
}

void print_char(char c)
{
    sys_write(1, &c, 1);
}

void print_str(const char *s)
{
    sys_write(1, s, strlen(s));
}

static const uint64_t pow10[20] = {
    10000000000000000000ULL, 1000000000000000000ULL, 100000000000000000ULL,
    10000000000000000ULL,    1000000000000000ULL,    100000000000000ULL,
    10000000000000ULL,       1000000000000ULL,       100000000000ULL,
    10000000000ULL,          1000000000ULL,          100000000ULL,
    10000000ULL,             1000000ULL,             100000ULL,
    10000ULL,                1000ULL,                100ULL,
    10ULL,                   1ULL,
};

/* Decimal output by repeated subtraction of powers of ten: no division. */
void print_u64(uint64_t v)
{
    char buf[20];
    int n = 0;
    for (int i = 0; i < 20; i++) {
        char d = '0';
        while (v >= pow10[i]) {
            v -= pow10[i];
            d++;
        }
        if (n > 0 || d != '0' || i == 19) /* skip leading zeros */
            buf[n++] = d;
    }
    sys_write(1, buf, n);
}

void print_u32(uint32_t v)
{
    print_u64(v);
}

void print_i64(int64_t v)
{
    if (v < 0) {
        print_char('-');
        /* 0 - (uint64_t)v is well defined even for INT64_MIN. */
        print_u64(0 - (uint64_t) v);
    } else {
        print_u64((uint64_t) v);
    }
}

void print_hex32(uint32_t v)
{
    char buf[10] = {'0', 'x'};
    for (int i = 0; i < 8; i++)
        buf[2 + i] = "0123456789abcdef"[(v >> (28 - 4 * i)) & 0xf];
    sys_write(1, buf, 10);
}
