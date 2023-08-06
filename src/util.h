#ifndef _UTIL_H
#define _UTIL_H

#ifdef __cplusplus
extern "C" {
#endif


#include <stddef.h>

#define PAGE_SIZE 4096

#define DIV_ROUNDUP(VALUE, DIV) ((VALUE + (DIV - 1)) / DIV)

#define ALIGN_UP(VALUE, ALIGN) (DIV_ROUNDUP(VALUE, ALIGN) * ALIGN)
#define ALIGN_DOWN(VALUE, ALIGN) (VALUE / ALIGN * ALIGN)

// same as standard memset, except this uses 64-bit operation to speed the process up
void* fast_memset(void* ptr, size_t value, size_t num);

// same as standard memcpy, except this uses 64-bit operation to speed the process up
void* fast_memcpy(void* dest, const void* src, size_t num);


#ifdef __cplusplus
}
#endif

#endif /* _UTIL_H */