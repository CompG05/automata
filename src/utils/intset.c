#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "utils/intset.h"
#include "utils/intlist.h"

IntSet *intset_create(){
    IntSet *set = (IntSet *)malloc(sizeof(IntSet));
    set->list = intlist_create();
    return set;
}

void intset_add(IntSet *set, int value){
    IntList *list = set->list;
    intlist_add(list, value);
    intlist_remove_duplicates(list);
}

int intset_contains(IntSet *set, int value){
    IntList *list = set->list;
    return intlist_contains(list, value);
}

int intset_contains_any(IntSet *set, IntSet *src) {
    int ret = 0;
    IntSetIterator *it = intset_iterator_create(src);
    while (intset_iterator_has_next(it)) {
        if (intset_contains(set, intset_iterator_next(it))) {
            ret = 1;
            break;
        }
    }
    intset_iterator_free(it);
    return ret;
}

int intset_is_empty(IntSet *set){
    return intset_size(set) == 0;
}

int intset_equals(IntSet *set1, IntSet *set2){
    if(intset_size(set1) != intset_size(set2))
        return 0;
    IntSetIterator *it = intset_iterator_create(set1);
    while (intset_iterator_has_next(it)) {
        if (! intset_contains(set2, intset_iterator_next(it)))
            return 0;
    }

    intset_iterator_free(it);
    return 1;
}

int intset_size(IntSet *set) {
    IntList *list = set->list;
    return intlist_size(list);
}

void intset_append_list(IntSet *set, IntList *list) {
    intlist_append_list(set->list, list);
    intlist_remove_duplicates(set->list);
}

void intset_append_set(IntSet *dst, IntSet *src) {
    intset_append_list(dst, src->list);
}

IntSet *intset_from_array(int *array, int i_start, int i_end) {
    IntSet *set = intset_create();
    set->list = intlist_from_array(array, i_start, i_end);
    intlist_remove_duplicates(set->list);
    return set;
}

IntSet *intset_union(IntSet *set1, IntSet *set2) {
    IntSet *union_set = intset_create();
    intset_append_list(union_set, set1->list);
    intset_append_list(union_set, set2->list);
    return union_set;
}

IntSet *intset_intersection(IntSet *set1, IntSet *set2) {
    IntSet *inter_set = intset_create();
    int current;

    IntSetIterator *it = intset_iterator_create(set1);
    while(intset_iterator_has_next(it)){
        current = intset_iterator_next(it);
        if(intset_contains(set2, current))
            intset_add(inter_set, current);
    }

    intset_iterator_free(it);
    return inter_set;
}

void intset_print(IntSet *set) {
    if (intset_is_empty(set)) {
        printf("{}");
        return;
    }
    IntSetIterator *it = intset_iterator_create(set);
    printf("{%d", intset_iterator_next(it));
    while (intset_iterator_has_next(it)) {
         printf(", %d", intset_iterator_next(it));
    }
    printf("}");

    intset_iterator_free(it);
}

void intset_free(IntSet *set) {
    intlist_free(set->list);
    free(set);
}

IntSet *intset_clone(IntSet *set) {
    IntSet *new_set = intset_create();
    new_set->list = intlist_clone(set->list);
    return new_set;
}

IntSetIterator *intset_iterator_create(IntSet *set) {
    IntSetIterator *it = (IntSetIterator *)malloc(sizeof(IntSetIterator));
    it->iterator = intlist_iterator_create(set->list);
    return it;
}

int intset_iterator_has_next(IntSetIterator *it) {
    return intlist_iterator_has_next(it->iterator);
}

int intset_iterator_next(IntSetIterator *it) {
    return intlist_iterator_next(it->iterator);
}

void intset_iterator_reset(IntSetIterator *it) {
    intlist_iterator_reset(it->iterator);
}

void intset_iterator_free(IntSetIterator *it) {
    intlist_iterator_free(it->iterator);
    free(it);
}
