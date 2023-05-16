#include <stdio.h>
#include <stdlib.h>

#include "automata/automata.h"
#include "utils/intset.h"

int test_automaton_accepts() {
    int num_states = 4;
    int alphabet_array[] = {'0', '1'};
    int start = 0;
    int finals[] = {3};

    Automaton *a =
        automaton_create(num_states, intset_from_array(alphabet_array, 0, 2), start, intset_from_array(finals, 0, 1));

    automaton_add_transition(a, 0, '0', 1); // (q0, 0) -> q1
    automaton_add_transition(a, 0, '1', 0); // (q0, 1) -> q0
    automaton_add_transition(a, 1, '0', 1); // (q1, 0) -> q1
    automaton_add_transition(a, 1, '1', 2); // (q1, 1) -> q2
    automaton_add_transition(a, 2, '0', 3); // (q2, 0) -> q3
    automaton_add_transition(a, 2, '1', 2); // (q2, 1) -> q2
    automaton_add_transition(a, 3, '0', 3); // (q3, 0) -> q3
    automaton_add_transition(a, 3, '1', 3); // (q3, 1) -> q3

    char *accepted_strings[] = {"010", "1110001101", "00011100011"};
    char *rejected_strings[] = {"01", "", "111000111"};

    automaton_print(a);
    printf("\n");
    for (int i = 0; i < 3; i++) {
        if (!automaton_accepts(a, accepted_strings[i]))
            return 1;
    }

    for (int i = 0; i < 3; i++) {
        if (automaton_accepts(a, rejected_strings[i]))
            return 1;
    }

    automaton_free(a);

    return 0;
}

int test_lambda_closure() {
    int num_states = 6;
    int alphabet_array[] = {'a', 'b', 'c'};
    int start = 0;
    int finals[] = {4, 5};

    // Equivalent to (ac|b+)+
    Automaton *a =
        automaton_create(num_states, intset_from_array(alphabet_array, 0, 3), start, intset_from_array(finals, 0, 2));

    automaton_add_transition(a, 0, '_', 3);
    automaton_add_transition(a, 0, 'a', 1);
    automaton_add_transition(a, 1, 'c', 2);
    automaton_add_transition(a, 2, '_', 5);
    automaton_add_transition(a, 3, 'b', 4);
    automaton_add_transition(a, 3, 'b', 5);
    automaton_add_transition(a, 4, 'b', 4);
    automaton_add_transition(a, 4, 'b', 5);

    int source_states_array[] = {0};
    int expected1_array[] = {0, 3};

    IntSet *source_states = intset_from_array(source_states_array, 0, 1);
    IntSet *expected1 = intset_from_array(expected1_array, 0, 2);

    IntSet *closure1 = lambda_closure(a, source_states);

    if (!intset_equals(closure1, expected1))
        return 1;

    intset_free(source_states);
    intset_free(expected1);
    intset_free(closure1);

    int source_states2_array[] = {0, 2};
    int expected2_array[] = {0, 2, 3, 5};

    IntSet *source_states2 = intset_from_array(source_states2_array, 0, 2);
    IntSet *expected2 = intset_from_array(expected2_array, 0, 4);

    IntSet *closure2 = lambda_closure(a, source_states2);

    if (!intset_equals(closure2, expected2))
        return 1;

    automaton_free(a);
    intset_free(source_states2);
    intset_free(expected2);
    intset_free(closure2);

    return 0;
}

int test_move() {
    int num_states = 6;
    int alphabet_array[] = {'a', 'b', 'c'};
    int start = 0;
    int finals[] = {4, 5};

    // Equivalent to (ac|b+)+
    Automaton *a =
        automaton_create(num_states, intset_from_array(alphabet_array, 0, 3), start, intset_from_array(finals, 0, 2));

    automaton_add_transition(a, 0, '_', 3);
    automaton_add_transition(a, 0, 'a', 1);
    automaton_add_transition(a, 1, 'c', 2);
    automaton_add_transition(a, 2, '_', 5);
    automaton_add_transition(a, 3, 'b', 4);
    automaton_add_transition(a, 3, 'b', 5);
    automaton_add_transition(a, 4, 'b', 4);
    automaton_add_transition(a, 4, 'b', 5);

    int source_states_array[] = {0, 1, 3};
    int expected1_array[] = {2};

    IntSet *source_states = intset_from_array(source_states_array, 0, 3);
    IntSet *expected1 = intset_from_array(expected1_array, 0, 1);

    IntSet *move1 = move(a, source_states, 'c');

    intset_print(move1);
    printf("\n");

    if (!intset_equals(move1, expected1))
        return 1;

    automaton_free(a);
    intset_free(source_states);
    intset_free(expected1);
    intset_free(move1);

    return 0;
}

int test_determinize() {
    int num_states = 6;
    int alphabet_array[] = {'a', 'b', 'c'};
    int start = 0;
    int finals[] = {4, 5};

    // Equivalent to (ac|b+)+
    Automaton *a =
        automaton_create(num_states, intset_from_array(alphabet_array, 0, 3), start, intset_from_array(finals, 0, 2));

    automaton_add_transition(a, 0, '_', 3);
    automaton_add_transition(a, 0, 'a', 1);
    automaton_add_transition(a, 1, 'c', 2);
    automaton_add_transition(a, 2, '_', 5);
    automaton_add_transition(a, 3, 'b', 4);
    automaton_add_transition(a, 3, 'b', 5);
    automaton_add_transition(a, 4, 'b', 4);
    automaton_add_transition(a, 4, 'b', 5);
    automaton_add_transition(a, 5, '_', 0);

    printf("NFA:\n");
    automaton_print(a);
    printf("\n");

    Automaton *dfa_a = automaton_determinize(a);

    printf("DFA:\n");
    automaton_print(dfa_a);
    printf("\n");

    if (dfa_a->num_states != 5) {
        printf("num_states\n");
        automaton_free(a);
        automaton_free(dfa_a);
        return 1;
    }

    if (intset_size(dfa_a->alphabet) != 3) {
        printf("alphabet\n");
        automaton_free(a);
        automaton_free(dfa_a);
        return 1;
    }

    if (dfa_a->initial_state != 0) {
        printf("initial_state\n");
        automaton_free(a);
        automaton_free(dfa_a);
        return 1;
    }

    if (intset_size(dfa_a->final_states) != 2) {
        printf("final_states\n");
        automaton_free(a);
        automaton_free(dfa_a);
        return 1;
    }

    automaton_free(a);
    automaton_free(dfa_a);

    return 0;
}

int main() {
    if (test_automaton_accepts()) {
        printf("Failed: test_automaton_accepts\n");
        exit(EXIT_FAILURE);
    }

    printf("\nPassed: test_automaton_accepts\n");

    if (test_lambda_closure()) {
        printf("Failed: test_lambda_closure\n");
        exit(EXIT_FAILURE);
    }

    printf("\nPassed: test_lambda_closure\n");

    if (test_move()) {
        printf("Failed: test_move\n");
        exit(EXIT_FAILURE);
    }

    printf("\nPassed: test_move\n");

    if (test_determinize()) {
        printf("Failed: test_determinize\n");
        exit(EXIT_FAILURE);
    }

    printf("\nPassed: test_determinize\n");

    exit(EXIT_SUCCESS);
}
