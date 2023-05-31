#include <stdlib.h>

#include "automata/automata.h"
#include "utils/intset.h"

void clone_transition_table(Automaton *src, Automaton *dst, int state_delta) {
    IntSet *alphabet_and_lambda = intset_clone(src->alphabet);
    intset_add(alphabet_and_lambda, '_');
    IntSetIterator *alphabet_it = intset_iterator_create(alphabet_and_lambda);
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
    intset_free(alphabet_and_lambda);
}

Automaton *automaton_union(Automaton *a1, Automaton *a2) {
    int num_states = a1->num_states + a2->num_states + 2;
    IntSet *alphabet = intset_union(a1->alphabet, a2->alphabet);
    int start_state = 0;
    IntSet *final_state = intset_create_from_value(num_states - 1);

    int d1 = 1;                   // To shift the states of a1
    int d2 = a1->num_states + d1; // To shift the states of a2
    int init1 = a1->initial_state + d1;
    int init2 = a2->initial_state + d2;

    Automaton *union_autom = automaton_create(num_states, alphabet, start_state, final_state);

    clone_transition_table(a1, union_autom, d1);
    clone_transition_table(a2, union_autom, d2);
    automaton_add_transition(union_autom, 0, '_', init1);
    automaton_add_transition(union_autom, 0, '_', init2);

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

Automaton *automaton_concat(Automaton *a1, Automaton *a2) {
    int num_states = a1->num_states + a2->num_states;
    IntSet *alphabet = intset_union(a1->alphabet, a2->alphabet);
    int start_state = a1->initial_state;
    IntSet *final_states = intset_create();

    int d2 = a1->num_states;   //To shift the states of a2

    // For each f ∈ F_2, add (f + Δ_2) to F
    IntSetIterator *final_states_it = intset_iterator_create(a2->final_states);
    while (intset_iterator_has_next(final_states_it))
        intset_add(final_states, intset_iterator_next(final_states_it) + a1->num_states);
    intset_iterator_free(final_states_it);

    int q_02 = a2->initial_state + d2; // The old start state of a2

    Automaton *concat_autom = automaton_create(num_states, alphabet, start_state, final_states);

    clone_transition_table(a1, concat_autom, 0);
    clone_transition_table(a2, concat_autom, d2);

    // For each f ∈ F_1, add (f -- λ --> q_02) to δ
    final_states_it = intset_iterator_create(a1->final_states);
    while (intset_iterator_has_next(final_states_it))
        automaton_add_transition(concat_autom, intset_iterator_next(final_states_it), '_', q_02);
    intset_iterator_free(final_states_it);

    return concat_autom;
}

Automaton *automaton_kclosure(Automaton *automaton) {
    int num_states = automaton->num_states + 2;
    IntSet *alphabet = intset_clone(automaton->alphabet);
    int start_state = 0;
    IntSet *final_states = intset_create_from_value(num_states - 1);

    int d = 1; // To shift the states of the automaton
    int q_0 = automaton->initial_state + d; // The old initial state shifted

    Automaton *kclosure_autom = automaton_create(num_states, alphabet, start_state, final_states);

    clone_transition_table(automaton, kclosure_autom, d);
    automaton_add_transition(kclosure_autom, 0, '_', q_0);
    automaton_add_transition(kclosure_autom, 0, '_', num_states - 1);

    // For each f ∈ F, add (f -- λ --> d), (f -- λ --> q_f) to δ
    IntSetIterator *final_states_it = intset_iterator_create(automaton->final_states);
    while (intset_iterator_has_next(final_states_it)) {
        int f = intset_iterator_next(final_states_it);
        automaton_add_transition(kclosure_autom, f + d, '_', q_0);
        automaton_add_transition(kclosure_autom, f + d, '_', num_states - 1);
    }

    return kclosure_autom;
}

