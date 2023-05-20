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

/// Creates a new IntSet with the given value.
IntSet *intset_create_from_value(int value);

/// Creates a new IntSet with the values in the given range.
/// Parameters:
/// - start: the start of the range (inclusive).
/// - end: the end of the range (exclusive).
IntSet *intset_create_from_range(int start, int end);

/// Adds the given value to the given IntSet.
void intset_add(IntSet *set, int value);

/// Returns 1 if the given IntSet contains the given value, 0 otherwise.
int intset_contains(IntSet *set, int value);

/// Returns an arbitrary element of the set
int intset_get_one(IntSet *set);

/// Returns 1 if the intset contains any of the values of src, 0 otherwise.
/// Parameters:
/// - set: the IntSet to check.
/// - src: the IntSet to check against.
int intset_contains_any(IntSet *set, IntSet *src);

/// Returns 1 if the given IntSet is empty, 0 otherwise.
int intset_is_empty(IntSet *set);

/// Returns 1 if the given IntSet is equal to the given IntSet, 0 otherwise.
int intset_equals(IntSet *set1, IntSet *set2);

/// Returns the size of the given IntSet.
int intset_size(IntSet *set);

/// Adds the elements from an IntList to a IntSet.
void intset_append_list(IntSet *set, IntList *list);

/// Adds the elements of an IntSet to another.
/// Parameters:
/// - src: the IntSet to copy from.
/// - dst: the IntSet to copy to.
void intset_append_set(IntSet *dst, IntSet *src);

/// Returns a set created from an array.
IntSet *intset_from_array(int *array, int i_start, int i_end);

/// Returns the union of set1 and set2.
IntSet *intset_union(IntSet *set1, IntSet *set2);

/// Returns the intersection of set1 and set2.
IntSet *intset_intersection(IntSet *set1, IntSet *set2);

/// Returns a string representation of the given IntSet.
char *intset_to_string(IntSet *set);

/// Prints the given IntSet.
void intset_print(IntSet *set);

/// Prints the given IntSet as a set of chars.
void intset_print_chars(IntSet *set);

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

/// Resets the iterator
void intset_iterator_reset(IntSetIterator *it);

/// Frees the given IntSet iterator.
void intset_iterator_free(IntSetIterator *it);


#endif // INTSET_H
