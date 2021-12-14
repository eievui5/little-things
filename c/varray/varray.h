#pragma once

// I've said before that one of the only reasons I use C++ is for std::vector
// and std::unordered_map. This tiny library aims to fix that.

// This library has been made as lightweight as possible. There is no compiled
// code, just a collection of macros and static inline functions.

// While some functions provide error checking, this is done using C's `assert`
// and so can easily be disabled by passing -DNDEBUG

// Ultimately this library is able to produce very small and fast output while
// abstracting away the logic behind variably-sized arrays.

// The "V" in "VArray" can be interpreted as either "varible-size" or "vector",
// whichever you prefer to read it as :)

#include <assert.h>
#include <stdlib.h>
#include <string.h>

// Constuct a new VArray of a given size.
#define va_new(t, s) (VArray) {(s), sizeof(t), malloc(s * sizeof(t))}
#define va_foreach(i, each, va) (i) = 0, (each) = (va)->buf; i < (va)->size; i += 1, each += 1

// A malloc-like interface for accessing a dynamically-sized array.
typedef struct VArray {
    size_t size; // Current size of the array in bytes.
    unsigned char width; // Width in bytes of the contained type.
    void* buf;
} VArray;

// Returns the element at a given index in a VArray.
// Asserts that the index is within bounds unless NDEBUG is defined.
#define va_get(va, t, i) \
    (assert((i) < (va)->size), \
    (t*) ((va)->buf + (i) * (va)->width))

// Resizes a VArray.
// Asserts that allocation does not return NULL unless NDEBUG is defined.
static inline void va_resize(VArray* va, size_t s) {
    va->buf = realloc(va->buf, s);
    assert(va->buf != NULL);
    va->size = s;
}

// Appends additional bytes to a VArray.
// Returns the index of the new elements.
static inline size_t va_expand(VArray* va, size_t s) {
    size_t old_size = va->size;
    va->size += s;
    va->buf = realloc(va->buf, va->size * (va)->width);
    assert(va->buf != NULL);
    return old_size;
}

// Execute a memset on a VArray.
static inline void va_set(VArray* va, char n) {
    memset(va->buf, n, va->size);
}

// Duplicate a VArray.
static inline VArray va_dup(VArray* va) {
    void* buf = malloc(va->size * va->width);
    memcpy(buf, va->buf, va->size * va->width);
    return (VArray) {va->size, va->width, buf};
}

// Free a VArray's buffer.
static inline void va_free(VArray* va) {
    free(va->buf);
}
