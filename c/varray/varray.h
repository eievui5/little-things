#pragma once

// I've said before that one of the only reasons I use C++ is for std::vector
// and std::unordered_map. This tiny library aims to fix that.

// This library has been made as lightweight as possible. There is no compiled
// code, just a collection of macros and static inline functions.

// Ultimately this library is able to produce very small and fast output while
// abstracting away the logic behind variably-sized arrays.

// The "V" in "VArray" can be interpreted as either "varible-size" or "vector",
// whichever you prefer to read it as :)

#include <assert.h>
#include <stdlib.h>
#include <string.h>

#define _va_head(va) ((VArrayHeader*) ((va) - sizeof(VArrayHeader)))

// A malloc-like interface for accessing a dynamically-sized array.
typedef struct VArrayHeader {
    size_t size; // Current size of the array in bytes.
} VArrayHeader;

// Constuct a new VArray of a given size.
static inline void* va_new(size_t s) {
    VArrayHeader* head = malloc(s + sizeof(VArrayHeader));
    head->size = s;
    return head + 1;
}

// Get the size of a VArray.
static inline size_t va_sizeof(void* va) {
    return _va_head(va)->size;
}

// Resizes a VArray.
static inline void* va_resize(void* va, size_t s) {
    VArrayHeader* head = realloc(_va_head(va), sizeof(VArrayHeader) + s);
    head->size = s;
    return head + 1;
}

// Appends additional bytes to a VArray.
static inline void* va_expand(void* va, size_t s) {
    return va_resize(va, _va_head(va)->size + s);
}

// Duplicate a VArray.
static inline void* va_dup(void* va) {
    VArrayHeader* new_va = malloc(_va_head(va)->size + sizeof(VArrayHeader));
    memcpy(new_va, _va_head(va), _va_head(va)->size + sizeof(VArrayHeader));
    return new_va + 1;
}

// Free a VArray's buffer.
static inline void va_free(void* va) {
    free(_va_head(va));
}
