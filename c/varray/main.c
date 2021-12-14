#include <stdio.h>
#include "varray.h"

int main() {
    int* number_vector = va_new(10 * sizeof(int));
    printf("Created VArray of size %zu\n", va_sizeof(number_vector));
    for (int i = 0; i < va_sizeof(number_vector) / sizeof(int); i++) {
        number_vector[i] = i * 10;
        printf("Number %i is %i\n", i, number_vector[i]);
    }

    size_t old_size = va_sizeof(number_vector);
    int* second_vector = va_expand(va_dup(number_vector), 10 * sizeof(int));
    puts("Expanding vector...");
    for (int i = old_size / sizeof(int); i < va_sizeof(second_vector) / sizeof(int); i++) {
        second_vector[i] = i * 16;
    }
    printf("After expansion, VArray has a size of %zu.\n", va_sizeof(second_vector));

    puts("The final VArray contains:");
    for (int i = 0; i < va_sizeof(second_vector) / sizeof(int); i++) {
        printf("va[%i]:\t%i\n", i, second_vector[i]);
    }

    puts("Done. Exiting.");
    va_free(&number_vector);
    va_free(&second_vector);
}