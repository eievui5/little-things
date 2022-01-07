#include <stdio.h>

#include "llist.h"

int main() {
    char** string_list_head = ll_new(sizeof(char**));
    *string_list_head = "Hello";
    char** string_list_next = lls_insert_after(string_list_head);
    *string_list_next = "world";
    string_list_next = lls_insert_before(string_list_next);
    *string_list_next = ", ";
    string_list_next = lls_append(string_list_next);
    *string_list_next = "!";

    puts("Printing result!");
    ll_fornext (string_list_head, i)
        printf("%p <- %p: \"%s\" -> %p\n", ll_last(i), i, *i, ll_next(i));
    puts("Concatenated, that looks like:");
    ll_fornext (string_list_head, i)
        fputs(*i, stdout);
    putchar('\n');
}