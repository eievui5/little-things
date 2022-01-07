#include <stdio.h>
#include "varray.h"

int main() {
    printf("Creating VArray and filling it with multiples of 10.\n");
    int* number_vector = va_new(5 * sizeof(int));
    for (int i = 0; i < va_len(number_vector); i++)
        number_vector[i] = i * 10;

    printf("Expanding VArray and appending multiples of 16.\n");
    int* second_vector = va_dup(number_vector);
    va_expand(&second_vector, 3 * sizeof(int));
    for (int i = va_len(number_vector); i < va_len(second_vector); i++)
        second_vector[i] = i * 16;

    puts("The final VArray contains:");
    for (int i = 0; i < va_len(second_vector); i++)
        printf("va[%i]:\t%i\n", i, second_vector[i]);

    va_free(number_vector);
    va_free(second_vector);

    puts("Done with number tests. Now testing string operations.");
    puts("Creating two strings, \"Hello\" and \"World\", adding punctuation, and concatenating them.");
    char* mutable_string = va_newstr("Hello");
    va_append_char(&mutable_string, ',');
    va_append_char(&mutable_string, ' ');

    char* another_string = va_newstr("World");
    va_append_char(&another_string, '!');

    va_strcat(&mutable_string, another_string);

    puts(mutable_string);
    puts(another_string);

    va_free(mutable_string);
    va_free(another_string);
}