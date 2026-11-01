#pragma once

/* Freestanding headers: provided by GCC itself, not by a C library. */
#include <stddef.h>
#include <stdint.h>

/* System calls (newlib convention: number in a7). */
long sys_write(int fd, const void *buf, size_t len);

/* Output, built only on sys_write; no division anywhere. */
void print_char(char c);
void print_str(const char *s);
void print_u32(uint32_t v);
void print_u64(uint64_t v);
void print_i64(int64_t v);
void print_hex32(uint32_t v);

/* perfcounter.S: hi-lo-hi reads with retry. */
uint64_t get_cycles(void);
uint64_t get_instret(void);

/* Helpers GCC may call on its own. */
void *memcpy(void *dst, const void *src, size_t n);
void *memset(void *dst, int c, size_t n);
void *memmove(void *dst, const void *src, size_t n);
int memcmp(const void *a, const void *b, size_t n);
size_t strlen(const char *s);
