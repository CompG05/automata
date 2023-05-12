//===-- utils/inset.h - IntSet -----------------------------------*- C -*-===//
//
// Defines the IntSet type and its functions.
// An IntSet is a set of integers.
// It is implemented as a linked list of integers.
//
//===----------------------------------------------------------------------===//

#ifndef INTSET_H
#define INTSET_H

#include "intlist.h"


typedef struct IntSet {
    IntList *list;
} IntSet;

typedef struct IntSetIterator {
    IntListIterator *iterator;
} IntSetIterator;

/// Creates a new IntSet.
IntSet *intset_create();

/// Adds the given value to the given IntSet.
void intset_add(IntSet *set, int value);

/// Returns 1 if the given IntSet contains the given value, 0 otherwise.
int intset_contains(IntSet *set, int value);

/// Returns 1 if the given IntSet is empty, 0 otherwise.
int intset_is_empty(IntSet *set);

/// Returns 1 if the given IntSet is equal to the given IntSet, 0 otherwise.
int intset_equals(IntSet *set1, IntSet *set2);

/// Returns the size of the given IntSet.
int intset_size(IntSet *set);

/// Prints the given IntSet.
void intset_print(IntSet *set);

/// Frees the given IntSet.
void intset_free(IntSet *set);

/// Returns a new IntSet with the values of the given IntSet.
IntSet *intset_clone(IntSet *set);

/// Returns an IntSet iterator
IntSetIterator *intset_iterator_create(IntSet *set);

/// Returns 1 if the given IntSet iterator has a next value, 0 otherwise.
int intset_iterator_has_next(IntSetIterator *it);

/// Returns the next value of the given IntSet iterator.
int intset_iterator_next(IntSetIterator *it);

/// Frees the given IntSet iterator.
void intset_iterator_free(IntSetIterator *it);


#endif // INTSET_H
