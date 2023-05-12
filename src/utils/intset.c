#include <stdlib.h>
#include <stdio.h>

#include "utils/intset.h"
#include "utils/intlist.h"

IntSet *intset_create(){
    IntSet *set = malloc(sizeof(IntSet));
    IntList *list = intlist_create();
    set->list = list;
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

int intset_is_empty(IntSet *set){
    return intset_size(set) == 0;
}

int intset_equals(IntSet *set1, IntSet *set2){
    if(intset_size(set1) != intset_size(set2))
        return 0;
    IntSetIterator *it = intset_iterator_create(set1);
    while (intset_iterator_has_next(it)) {
        if (! intset_contains(set2, intset_iterator_has_next(it)))
            return 0;
    }
    return 1;
}

int intset_size(IntSet *set){
    IntList *list = set->list;
    return intlist_size(list);
}

void intset_print(IntSet *set){
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
}

void intset_free(IntSet *set){
    IntList *list = set->list;
    intlist_free(list);
    free(set);
}

IntSet *intset_clone(IntSet *set){
    IntSet *new_set = intset_create();
    new_set->list = intlist_clone(set->list);
    return new_set;
}

IntSetIterator *intset_iterator_create(IntSet *set){
    IntSetIterator *it = malloc(sizeof(IntSetIterator));
    it->iterator = intlist_iterator_create(set->list);
    return it;
}

int intset_iterator_has_next(IntSetIterator *it){
    return intlist_iterator_has_next(it->iterator);
}

int intset_iterator_next(IntSetIterator *it) {
    return intlist_iterator_next(it->iterator);
}

void intset_iterator_free(IntSetIterator *it) {
    intlist_iterator_free(it->iterator);
    free(it);
}
