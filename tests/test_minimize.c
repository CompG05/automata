#include "automata/automata.h"
#include "utils/intset.h"

int main() {
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

    Automaton *m = automaton_minimize(a);
    automaton_free(a);
    automaton_print(m);
    automaton_free(m);
}