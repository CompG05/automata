#ifndef UTILS_INTLIST_H
#define UTILS_INTLIST_H

//===-- utils/intlist.h - IntList ---------------------------------*- C -*-===//
//
// Defines the IntList type and its functions.
// An IntList is a list of integers.
// It is implemented as a linked list of integers.
//
//===----------------------------------------------------------------------===//

typedef struct IntNode {
    int value;
    struct IntNode *next;
} IntNode;

typedef struct IntList {
    IntNode *head;
    int size;
} IntList;

/// Creates a new IntNode with the given value.
IntNode *intnode_create(int value);

/// Creates a new IntList.
IntList *intlist_create();

/// Adds the given value to the given IntList.
void intlist_add(IntList *list, int value);

/// Returns the value at the given index in the given IntList.
int intlist_get(IntList *list, int index);

/// Removes the value at the given index in the given IntList.
void intlist_remove(IntList *list, int index);

/// Returns the size of the given IntList.
int intlist_size(IntList *list);

/// Returns 1 if the given IntList contains the given value, 0 otherwise.
int intlist_contains(IntList *list, int value);

/// Returns 1 if the given IntList is empty, 0 otherwise.
int intlist_is_empty(IntList *list);

/// Returns 1 if the given IntList is equal to the given IntList, 0 otherwise.
int intlist_equals(IntList *list1, IntList *list2);

/// Appends the second IntList to the first one.
void intlist_append(IntList *list1, IntList *list2);

/// Constructs a new IntList with the values of the given IntList.
IntList *intlist_clone(IntList *list);

/// Constructs a new IntList with the values of in the range of the given array.
IntList *intlist_from_array(int *array, int i_start, int i_end);

/// Prints the given IntList.
void intlist_print(IntList *list);

/// Frees the given IntList.
void intlist_free(IntList *list);


#endif // UTILS_INTLIST_H
