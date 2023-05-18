#include <stdlib.h>

#include "automata/automata.h"
#include "utils/intset.h"

void clone_transition_table(Automaton *src, Automaton *dst, int state_delta) {
    IntSetIterator *alphabet_it = intset_iterator_create(src->alphabet);
    int symbol, to;

    // For each state ∈ Q
    for (int i = 0; i < src->num_states; i++) {
        // For each symbol ∈ Σ
        while (intset_iterator_has_next(alphabet_it)) {
            symbol = intset_iterator_next(alphabet_it);

            if (src->transition_table[i][code_of(symbol)] == NULL)
                continue;

            IntSetIterator *to_it = intset_iterator_create(src->transition_table[i][code_of(symbol)]);

            // For each to ∈ δ(state, symbol)
            while (intset_iterator_has_next(to_it)) {
                to = intset_iterator_next(to_it) + state_delta;
                // Add (state -- symbol --> to) to δ
                automaton_add_transition(dst, i + state_delta, symbol, to);
            }
            intset_iterator_free(to_it);
        }
        intset_iterator_reset(alphabet_it);
    }
    intset_iterator_free(alphabet_it);
}

Automaton *automaton_union(Automaton *a1, Automaton *a2) {
    int num_states = a1->num_states + a2->num_states + 2;
    IntSet *alphabet = intset_union(a1->alphabet, a2->alphabet);
    int start_state = 0;
    IntSet *final_state = intset_create();
    intset_add(final_state, num_states - 1);

    int d1 = 1; // To shift the states of a1
    int d2 = a1->num_states + d1; // To shift the states of a2

    Automaton *union_autom = automaton_create(num_states, alphabet, start_state, final_state);

    clone_transition_table(a1, union_autom, d1);
    clone_transition_table(a2, union_autom, d2);
    automaton_add_transition(union_autom, 0, '_', d1);
    automaton_add_transition(union_autom, 0, '_', d2);

    // Add lambda transitions from the final states to the new final state
    IntSetIterator *final_states_it = intset_iterator_create(a1->final_states);
    while (intset_iterator_has_next(final_states_it))
        automaton_add_transition(union_autom, intset_iterator_next(final_states_it) + d1, '_', num_states - 1);
    intset_iterator_free(final_states_it);

    final_states_it = intset_iterator_create(a2->final_states);
    while (intset_iterator_has_next(final_states_it))
        automaton_add_transition(union_autom, intset_iterator_next(final_states_it) + d2, '_', num_states - 1);
    intset_iterator_free(final_states_it);

    return union_autom;
}
