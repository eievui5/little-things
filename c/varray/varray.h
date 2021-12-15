#pragma once

// I've said before that one of the only reasons I use C++ is for std::string,
// std::vector and std::unordered_map. This tiny library aims to fix that.

// This library has been made as lightweight as possible. There is no compiled
// code, just a collection of macros and static inline functions.

// Ultimately this library is able to produce very small and fast output while
// abstracting away the logic behind variably-sized arrays.

// The "V" in "VArray" can be interpreted as either "variable" or "vector",
// whichever you prefer to read it as :)

#include <stdlib.h>
#include <string.h>

// Return the header of a VArray.
#define va_header(va) ((VArrayHeader*) (va) - 1)
// Get the number of elements in a VArray. Only works if the type is known.
#define va_len(va) (va_size(va) / sizeof(typeof(*va)))
// Append a value of any type to the end of a VArray.
#define va_append(va, i) \
    (va_expand((&va), sizeof(i))), \
    ((va)[va_len(va) - 1] = (i))

// Header for VArray objects. This is placed *before* the array, so that the
// user is able to directly index the array and use any type of their choosing.
typedef struct VArrayHeader {
    size_t size; // Current size of the array in bytes.
} VArrayHeader;

// Construct a new VArray of a given size.
static inline void* va_new(size_t s) {
    VArrayHeader* head = malloc(s + sizeof(VArrayHeader));
    head->size = s;
    return head + 1;
}

// Get the size of a VArray.
static inline size_t va_size(void* va) {
    return va_header(va)->size;
}

// Resizes a VArray. Requires a pointer to the VArray.
static inline void va_resize(void* va, size_t s) {
    VArrayHeader* head = realloc(va_header(*(void**) va), sizeof(VArrayHeader) + s);
    head->size = s;
    *(void**) va = head + 1;
}

// Appends additional bytes to a VArray. Requires a pointer to the VArray.
static inline void va_expand(void* va, size_t s) {
    va_resize(va, va_header(*(void**) va)->size + s);
}

// Duplicate a VArray.
static inline void* va_dup(void* va) {
    VArrayHeader* new_va = malloc(va_header(va)->size + sizeof(VArrayHeader));
    memcpy(new_va, va_header(va), va_header(va)->size + sizeof(VArrayHeader));
    return new_va + 1;
}

// Free a VArray's buffer.
static inline void va_free(void* va) {
    free(va_header(va));
}

// Create a new VArray using a string.
static inline void* va_newstr(const char* str) {
    size_t string_len = strlen(str);
    void* result = va_new(string_len);
    memcpy(result, str, string_len);
    return result;
}

// Append a single byte to a VArray string and adjust the null byte. Requires
// a pointer to the VArray.
static inline void va_append_char(char** va, char c) {
    va_expand(va, sizeof(char));
    (*va)[va_size(*va) - 1] = c;
    (*va)[va_size(*va)] = 0;
}

// Append a string to the end of a VArray. Requires a pointer to the VArray.
static inline void va_strcat(char** va, const char* str) {
    size_t s = strlen(str);
    size_t old_size = va_size(*va);
    va_expand(va, s);
    memcpy(*va + old_size, str, s);
    va_header(*va)->size += s;
}