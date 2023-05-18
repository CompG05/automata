#include <stdio.h>
#include <stdlib.h>

#include "automata/automata.h"

int test_union() {
    // Automaton that accepts 'ab'
    Automaton *a1 = automaton_create(3, intset_create_from_range('a', 'b'+1), 0, intset_create_from_value(2));
    automaton_add_transition(a1, 0, 'a', 1);
    automaton_add_transition(a1, 1, 'b', 2);

    // Automaton that accepts 'aba'
    Automaton *a2 = automaton_create(4, intset_create_from_range('a', 'b'+1), 0, intset_create_from_value(3));
    automaton_add_transition(a2, 0, 'a', 1);
    automaton_add_transition(a2, 1, 'b', 2);
    automaton_add_transition(a2, 2, 'a', 3);

    Automaton *a3 = automaton_union(a1, a2);
    automaton_free(a1);
    automaton_free(a2);

    automaton_print(a3);
    Automaton *d_a3 = automaton_determinize(a3);
    automaton_free(a3);

    if (!automaton_accepts(d_a3, "ab") || !automaton_accepts(d_a3, "aba")) {
        printf("Error: union automaton does not accept 'ab' or 'aba'\n");
        return 1;
    }

    if (automaton_accepts(d_a3, "") || automaton_accepts(d_a3, "b") || automaton_accepts(d_a3, "abab")) {
        printf("Error: union automaton accepts 'a', 'b' or 'abab'\n");
        return 1;
    }

    automaton_free(d_a3);
    return 0;
}

int main() {
    printf("Testing union...\n");
    if (test_union() != 0) {
        printf("Error: union test failed\n");
        exit(EXIT_FAILURE);
    }
    printf("Union test passed\n");
    exit(EXIT_SUCCESS);
}
