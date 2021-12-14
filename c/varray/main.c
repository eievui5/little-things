#include <stdio.h>
#include "varray.h"

int main() {
    VArray number_vector = va_new(int, 10);
    printf("Created VArray of size %zu\n", number_vector.size);
    for (int i = 0; i < number_vector.size; i++) {
        *va_get(&number_vector, int, i) = i * 10;
        printf("Number %i is %i\n", i, *va_get(&number_vector, int, i));
    }

    VArray second_vector = va_dup(&number_vector);
    puts("Expanding vector...");
    for (int i = va_expand(&second_vector, 10); i < second_vector.size; i++) {
        *va_get(&second_vector, int, i) = i * 10;
    }
    printf("After expansion, VArray has a size of %zu.\n", second_vector.size);

    puts("The final VArray contains:");
    int i; int* each;
    for (va_foreach(i, each, &second_vector)) {
        printf("va[%i]:\t%i\n", i, *each);
    }

    puts("Done. Exiting.");
    va_free(&number_vector);
    va_free(&second_vector);
}