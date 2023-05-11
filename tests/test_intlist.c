#include "utils/intlist.h"
#include <stdio.h>
#include <stdlib.h>

int main() {
    IntList *list = intlist_create();

    if (list->size != 0) {
        printf("intlist_create() failed\n");
        exit(EXIT_FAILURE);
    }

    if (!intlist_is_empty(list)) {
        printf("intlist_is_empty() failed\n");
        exit(EXIT_FAILURE);
    }

    intlist_add(list, 3);
    intlist_add(list, 2);
    intlist_add(list, 1);
    // [1, 2, 3]

    if (list->size != 3) {
        printf("intlist_add() failed\n");
        exit(EXIT_FAILURE);
    }

    intlist_insert(list, 4, 1);
    // [1, 4, 2, 3]

    if (intlist_get(list, 1) != 4) {
        printf("intlist_insert() failed\n");
        printf("Expected 4, got %d\n", intlist_get(list, 1));
        exit(EXIT_FAILURE);
    }

    intlist_remove(list, 2);
    // [1, 4, 3]

    if (list->size != 3) {
        printf("intlist_remove() failed\n");
        exit(EXIT_FAILURE);
    }

    if (intlist_get(list, 2) != 3) {
        printf("intlist_get() failed\n");
        printf("Expected 3, got %d\n", intlist_get(list, 2));
        exit(EXIT_FAILURE);
    }

    if (intlist_contains(list, 4) == 0 || intlist_contains(list, 5) == 1) {
        printf("intlist_contains() failed\n");
        exit(EXIT_FAILURE);
    }

    int a[] = {1, 4, 3};
    IntList *list2 = intlist_from_array(a, 0, 3);

    if (intlist_size(list2) != 3) {
        printf("intlist_from_array() failed\n");
        exit(EXIT_FAILURE);
    }

    if (!intlist_equals(list, list2)) {
        printf("intlist_equals() failed\n");
        exit(EXIT_FAILURE);
    }

    exit(EXIT_SUCCESS);
}
