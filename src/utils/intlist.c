#include <stdio.h>
#include <stdlib.h>

#include "utils/intlist.h"

IntNode *intnode_create(int value) {
    IntNode *node = (IntNode *)malloc(sizeof(IntNode));
    node->value = value;
    node->next = NULL;
    return node;
}

IntList *intlist_create() {
    IntList *list = (IntList *)malloc(sizeof(IntList));
    list->head = intnode_create(0);
    list->size = 0;
    return list;
}

void intlist_add(IntList *list, int value) {
    IntNode *node = intnode_create(value);
    node->next = list->head->next;
    list->head->next = node;
    list->size++;
}

void intlist_insert(IntList *list, int value, int index) {
    if (index > list->size)
        exit(EXIT_FAILURE);

    IntNode *node = intnode_create(value);
    IntNode *current = list->head;
    for (int i = 0; i < index; i++)
        current = current->next;

    node->next = current->next;
    current->next = node;
    list->size += 1;
}

int intlist_get(IntList *list, int index) {
    IntNode *current = list->head->next;
    for (int i = 0; i < index; i++) {
        current = current->next;
    }
    return current->value;
}

void intlist_remove(IntList *list, int index) {
    IntNode *current = list->head;

    for (int i = 0; i < index; i++) {
        current = current->next;
    }

    IntNode *to_remove = current->next;
    current->next = to_remove->next;
    free(to_remove);
    list->size -= 1;
}

int intlist_size(IntList *list) { return list->size; }

int intlist_contains(IntList *list, int value) {
    IntListIterator *it = intlist_iterator_create(list);
    int retvalue = 0;

    while (intlist_iterator_has_next(it)) {
        if (intlist_iterator_next(it) == value) {
            retvalue = 1;
            break;
        }
    }

    intlist_iterator_free(it);
    return retvalue;
}

int intlist_is_empty(IntList *list) { return list->size == 0; }

int intlist_equals(IntList *list1, IntList *list2) {
    if (list1->size != list2->size)
        return 0;

    IntListIterator *it1 = intlist_iterator_create(list1);
    IntListIterator *it2 = intlist_iterator_create(list2);
    int v1, v2;
    int retvalue = 1;

    while (intlist_iterator_has_next(it1)) {
        v1 = intlist_iterator_next(it1);
        v2 = intlist_iterator_next(it2);

        if (v1 != v2) {
            retvalue = 0;
            break;
        }
    }

    intlist_iterator_free(it1);
    intlist_iterator_free(it2);
    return retvalue;
}

void intlist_append_list(IntList *list1, IntList *list2) {
    IntListIterator *it = intlist_iterator_create(list2);

    while (intlist_iterator_has_next(it)) {
        intlist_insert(list1, intlist_iterator_next(it), list1->size);
    }

    intlist_iterator_free(it);
}

IntList *intlist_clone(IntList *list) {
    IntList *new_list = intlist_create();
    intlist_append_list(new_list, list);
    return new_list;
}

IntList *intlist_from_array(int *array, int i_start, int i_end) {
    IntList *new_list = intlist_create();

    for (int i = i_end - 1; i >= i_start; i--)
        intlist_add(new_list, array[i]);

    return new_list;
}

void intlist_remove_duplicates(IntList *list) {
    IntNode *left = list->head->next;
    IntNode *right, *prev;

    while (left != NULL) {
        right = left->next;
        prev = left;

        while (right != NULL) {
            if (right->value == left->value) {
                prev->next = right->next;
                free(right);
                right = prev->next;
                list->size -= 1;
            } else {
                prev = right;
                right = right->next;
            }
        }

        left = left->next;
    }
}

void intlist_print(IntList *list) {
    if (intlist_is_empty(list)) {
        printf("[]");
        return;
    }
    IntListIterator *it = intlist_iterator_create(list);
    printf("[%d", intlist_iterator_next(it));
    while (intlist_iterator_has_next(it)) {
        printf(", %d", intlist_iterator_next(it));
    }
    printf("]");
}

void intlist_free(IntList *list) {
    IntNode *current = list->head;
    IntNode *next;
    while (current != NULL) {
        next = current->next;
        free(current);
        current = next;
    }
    free(list);
}

IntListIterator *intlist_iterator_create(IntList *list) {
    IntListIterator *it = (IntListIterator *)malloc(sizeof(IntListIterator));
    it->first = list->head->next;
    it->current = list->head->next;
    return it;
}

int intlist_iterator_has_next(IntListIterator *it) { return it->current != NULL; }

int intlist_iterator_next(IntListIterator *it) {
    int value = it->current->value;
    it->current = it->current->next;
    return value;
}

void intlist_iterator_reset(IntListIterator *it) {
  it->current = it->first;
}

void intlist_iterator_free(IntListIterator *it) { free(it); }
