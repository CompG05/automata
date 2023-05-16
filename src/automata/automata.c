#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "automata/automata.h"
#include "utils/intset.h"

// Path: automata/automata.c

Automaton *automaton_create(int num_states, IntSet *alphabet, int initial_state, IntSet *final_states) {
    Automaton *a = (Automaton *)malloc(sizeof(Automaton));

    a->num_states = num_states;
    a->alphabet = alphabet;
    a->initial_state = initial_state;
    a->final_states = final_states;

    a->transition_table = (IntSet ***)malloc(sizeof(IntSet **) * num_states);
    for (int i = 0; i < num_states; i++) {
        a->transition_table[i] = (IntSet **)malloc(sizeof(IntSet *) * MAX_ALPHABET_SIZE);

        for (int j = 0; j < MAX_ALPHABET_SIZE; j++)
            a->transition_table[i][j] = NULL;
    }

    return a;
}

Transition *transition_create(int from, char symbol, IntSet *to) {
    Transition *t = (Transition *)malloc(sizeof(Transition));

    t->from = from;
    t->symbol = symbol;
    t->to = to;

    return t;
}

void transition_free(Transition *transition) {
    intset_free(transition->to);
    free(transition);
}

void automaton_add_transition(Automaton *automaton, int state, char symbol, int next_state) {
    if (automaton->transition_table[state][symbol] == NULL)
        automaton->transition_table[state][symbol] = intset_create();

    intset_add(automaton->transition_table[state][symbol], next_state);
}

void automaton_free(Automaton *automaton) {
    for (int i = 0; i < automaton->num_states; i++) {
        for (int j = 0; j < MAX_ALPHABET_SIZE; j++) {
            if (automaton->transition_table[i][j] != NULL)
                intset_free(automaton->transition_table[i][j]);
        }

        free(automaton->transition_table[i]);
    }

    free(automaton->transition_table);
    intset_free(automaton->alphabet);
    intset_free(automaton->final_states);
    free(automaton);
}

int automaton_accepts(Automaton *automaton, char *string) {
    int current_state = automaton->initial_state;

    for (int i = 0; i < strlen(string); i++) {
        char symbol = string[i];

        if (automaton->transition_table[current_state][symbol] == NULL)
            return 0;

        // Get the only element in the set
        IntSetIterator *it = intset_iterator_create(automaton->transition_table[current_state][symbol]);
        current_state = intset_iterator_next(it);
        intset_iterator_free(it);
    }

    return intset_contains(automaton->final_states, current_state);
}

IntSet *lambda_closure(Automaton *automaton, IntSet *states) {
    IntSet *closure, *new_states, *lambda_adjacents;
    IntSetIterator *it;
    int state, old_size;
    int changed = 1;

    closure = intset_clone(states);

    while (changed) {
        changed = 0;
        new_states = intset_create();
        it = intset_iterator_create(closure);

        while (intset_iterator_has_next(it)) {
            state = intset_iterator_next(it);
            lambda_adjacents = automaton->transition_table[state][LAMBDA_CODE];

            if (lambda_adjacents != NULL)
                intset_append_set(new_states, lambda_adjacents);
        }
        intset_iterator_free(it);

        old_size = intset_size(closure);
        intset_append_set(closure, new_states);

        if (intset_size(closure) > old_size)
            changed = 1;

        intset_free(new_states);
    }

    return closure;
}

IntSet *move(Automaton *automaton, IntSet *states, char symbol) {
    IntSet *move_set = intset_create();
    IntSet *moved;
    IntSetIterator *it = intset_iterator_create(states);
    int state;

    while (intset_iterator_has_next(it)) {
        state = intset_iterator_next(it);

        moved = automaton->transition_table[state][symbol];
        intset_append_set(move_set, moved);
    }
    intset_iterator_free(it);

    return move_set;
}

Automaton *automaton_determinize(Automaton *automaton) {
    IntSetIterator *alphabet_it;
    IntSet *T[(int)pow(2, automaton->num_states)];
    IntSet *start_set, *final_states, *move_set, *M, *to;
    Transition *transitions[(int)pow(2, automaton->num_states) * MAX_ALPHABET_SIZE];
    int transitions_size = 0;
    int t_size, counted, M_index;
    char symbol;

    t_size = 0;

    // Generate the initial state set
    intset_add(start_set = intset_create(), automaton->initial_state);
    T[t_size++] = lambda_closure(automaton, start_set);
    intset_free(start_set);

    alphabet_it = intset_iterator_create(automaton->alphabet);
    final_states = intset_create();
    for (int state_set = 0; state_set < t_size; state_set++) {
        while (intset_iterator_has_next(alphabet_it)) {
            symbol = intset_iterator_next(alphabet_it);

            move_set = move(automaton, T[state_set], symbol);
            M = lambda_closure(automaton, move_set);
            intset_free(move_set);

            counted = 0;
            for (int i = 0; i < t_size; i++) {
                if (intset_equals(T[i], M)) {
                    counted = 1;
                    M_index = i;
                    break;
                }
            }

            if (!counted) {
                T[t_size] = M;
                M_index = t_size++;
            } else
                intset_free(M);

            to = intset_create();
            intset_add(to, M_index);
            transitions[transitions_size++] = transition_create(state_set, symbol, to);
        }
        intset_iterator_reset(alphabet_it);

        if (intset_contains_any(T[state_set], automaton->final_states))
            intset_add(final_states, state_set);
    }
    intset_iterator_free(alphabet_it);

    for (int i = 0; i < t_size; i++)
        intset_free(T[i]);

    Automaton *dfa = automaton_create(t_size, intset_clone(automaton->alphabet), 0, final_states);

    IntSetIterator *to_it;
    for (int i = 0; i < transitions_size; i++) {
        Transition *t = transitions[i];
        to_it = intset_iterator_create(t->to);

        while (intset_iterator_has_next(to_it))
            automaton_add_transition(dfa, t->from, t->symbol, intset_iterator_next(to_it));

        intset_iterator_free(to_it);
        transition_free(t);
    }

    return dfa;
}

void automaton_print(Automaton *automaton) {
    IntSet *states = intset_create();
    for (int i = 0; i < automaton->num_states; i++)
        intset_add(states, i);

    printf("Automaton:\n");
    printf("  States: "); intset_print(states);
    printf("  Alphabet: "); intset_print(automaton->alphabet);
    printf("  Initial state: %d\n", automaton->initial_state);
    printf("  Final states: "); intset_print(automaton->final_states);

    printf("  Transitions:\n");
    for (int i = 0; i < automaton->num_states; i++) {
        for (int j = 0; j < MAX_ALPHABET_SIZE; j++) {
            IntSet *to = automaton->transition_table[i][j];

            if (to != NULL) {
                printf("    %d -- %c --> ", i, j + 32); intset_print(to);
            }
        }
    }
}
