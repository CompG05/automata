#include <stdio.h>
#include "automata/automata.h"
#include "automataio/write_automata.h"
#include "utils/intset.h"

int main() {
    // Automaton that accepts 01+
    Automaton *a = automaton_create(5, intset_create_from_range('0', '1'+1), 0, intset_create_from_range(2, 4));

    automaton_add_transition(a, 0, '0', 1);
    automaton_add_transition(a, 0, '1', 4);
    automaton_add_transition(a, 1, '0', 4);
    automaton_add_transition(a, 1, '1', 2);
    automaton_add_transition(a, 2, '0', 4);
    automaton_add_transition(a, 2, '1', 3);
    automaton_add_transition(a, 3, '0', 4);
    automaton_add_transition(a, 3, '1', 2);
    automaton_add_transition(a, 4, '0', 4);
    automaton_add_transition(a, 4, '1', 4);

    printf("Regular Expression: 01⁺\n\n");

    printf("FA:\n");
    automaton_print(a);
    Automaton *m = automaton_minimize(a);
    printf("\n\nMinimized FA:\n");
    automaton_print(m);

    if (m->num_states != 4) {
        printf("num_states\n");
        automaton_free(a);
        automaton_free(m);
        return 1;
    }

    if (intset_size(m->alphabet) != 2) {
        printf("alphabet\n");
        automaton_free(a);
        automaton_free(m);
        return 1;
    }

    if (m->initial_state != 0) {
        printf("initial_state\n");
        automaton_free(a);
        automaton_free(m);
        return 1;
    }

    if (intset_size(m->final_states) != 1) {
        printf("final_states\n");
        automaton_free(a);
        automaton_free(m);
        return 1;
    }

    write_automaton(a, "../tests/files/test_minimize.dot");
    write_automaton(m, "../tests/files/test_minimize_min.dot");

    automaton_free(a);
    automaton_free(m);

    printf("\n");

    printf("FA was written in: \"tests/files/test_minimize.dot\"\n");
    printf("FA minimize was written in: \"tests/files/test_minimize_min.dot\"\n");

    return 0;
}