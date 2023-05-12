#include "utils/intset.h"
#include <stdio.h>
#include <stdlib.h>

int main() {
    IntSet *set = intset_create();

    if (intset_size(set) != 0) {
        printf("intset_create() failed\n");
        exit(EXIT_FAILURE);

    if (!intset_is_empty(set)) {
        printf("intset_is_empty() failed\n");
        exit(EXIT_FAILURE);
    }

    intset_add(set, 3);
    intset_add(set, 2);
    intset_add(set, 1);
    intset_add(set, 3);
    // {1, 2, 3}

    if (intset_size(set) != 3) {
        printf("intset_add() failed\n");
        exit(EXIT_FAILURE);
    }

    if (intset_contains(set, 2) == 0 || intset_contains(set, 5) == 1) {
        printf("intset_contains() failed\n");
        exit(EXIT_FAILURE);
    }

    int a[] = {3,1,2};
    IntSet *set2 = intset_from_array(a, 0, 3);

    if (intset_size(set2) != 3) {
        printf("intset_from_array() failed\n");
        exit(EXIT_FAILURE);
    }

    if (!intset_equals(set, set2)) {
        printf("intset_equals() failed\n");
        exit(EXIT_FAILURE);
    }

    int b[] = {2,3,4};
    IntSet *set3 = intset_from_array(b,0,3);
    IntSet *expected;

    IntSet *union_set = intset_union(set, set3);
    int union_exp[] = {1,2,3,4};
    expected = intset_from_array(union_exp,0,4);

    if (!intset_equals(union_set, expected)) {
        printf("intset_union() failed\n");
        exit(EXIT_FAILURE);
    }

    intset_free(expected);
    intset_free(union_set);

    IntSet *intersection = intset_intersection(set, set3);
    int intersection_exp[] = {2, 3};
    expected = intset_from_array(intersection_exp, 0, 2);

    if (!intset_equals(intersection, expected)) {
        printf("intset_intersection() failed\n");
        exit(EXIT_FAILURE);
    }

    intset_free(expected);
    intset_free(intersection);

    intset_free(set);
    intset_free(set2);
    intset_free(set3);

    exit(EXIT_SUCCESS);
}