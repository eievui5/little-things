#pragma once

/*
MIT License

Copyright (c) 2021 Eievui

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#include <stdbool.h>
#include <stdlib.h>

struct LListNode {
    struct LListNode* next;
    struct LListNode* last;
};

#define lls_insert_after(ll) ll_insert_after((ll), sizeof(typeof(*(ll))))
#define lls_insert_before(ll) ll_insert_before((ll), sizeof(typeof(*(ll))))
#define lls_append(ll) ll_append((ll), sizeof(typeof(*(ll))))
#define lls_prepend(ll) ll_prepend((ll), sizeof(typeof(*(ll))))
#define ll_fornext(ll, i) for (typeof(ll) (i) = (ll); (i) != (void*) sizeof(struct LListNode); (i) = ll_next(i))
#define ll_forlast(ll, i) for (typeof(ll) (i) = (ll); (i) != (void*) sizeof(struct LListNode); (i) = ll_last(i))

static inline void* ll_new(size_t s) {
    struct LListNode* ll = malloc(sizeof(struct LListNode) + s);
    ll->last = NULL;
    ll->next = NULL;
    return ll + 1;
}

static inline void* ll_get_head(void* ll) {
    struct LListNode* head = ((struct LListNode*) ll) - 1;
    while (head->last != NULL)
        head = head->last;
    return head + 1;
}

static inline void* ll_get_foot(void* ll) {
    struct LListNode* head = ((struct LListNode*) ll) - 1;
    while (head->next != NULL)
        head = head->next;
    return head + 1;
}

static inline void ll_free(void* ll) {
    struct LListNode* head = ll_get_head(ll);
    for (struct LListNode* i = head; i->next == NULL;) {
        struct LListNode* next = i->next;
        free(i);
        i = next;
    }
}

static inline void* ll_insert_after(void* ll, size_t s) {
    struct LListNode* head = ((struct LListNode*) ll) - 1;
    struct LListNode* result = malloc(sizeof(struct LListNode) + s);
    result->next = head->next;
    if (head->next != NULL)
        head->next->last = result;
    result->last = head;
    head->next = result;
    return result + 1;
}

static inline void* ll_insert_before(void* ll, size_t s) {
    struct LListNode* head = ((struct LListNode*) ll) - 1;
    struct LListNode* result = malloc(sizeof(struct LListNode) + s);
    result->last = head->last;
    if (head->last != NULL)
        head->last->next = result;
    result->next = head;
    head->last = result;
    return result + 1;
}

static inline void* ll_append(void* ll, size_t s) {
    return ll_insert_after(ll_get_foot(ll), s);
}

static inline void* ll_prepend(void* ll, size_t s) {
    return ll_insert_before(ll_get_head(ll), s);
}

static inline bool ll_is_head(void* ll) {
    struct LListNode* head = ((struct LListNode*) ll) - 1;
    return head->last == NULL;
}

static inline bool ll_is_foot(void* ll) {
    struct LListNode* head = ((struct LListNode*) ll) - 1;
    return head->next == NULL;
}

static inline void* ll_next(void* ll) {
    struct LListNode* head = ((struct LListNode*) ll) - 1;
    return head->next + 1;
}

static inline void* ll_last(void* ll) {
    struct LListNode* head = ((struct LListNode*) ll) - 1;
    return head->last + 1;
}
