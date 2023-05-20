#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "automata/automata.h"
#include "utils/intset.h"

// Path: automata/automata.c

int code_of(char c) {
    return (int)c - 32;
}

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
    if (automaton->transition_table[state][code_of(symbol)] == NULL)
        automaton->transition_table[state][code_of(symbol)] = intset_create();

    intset_add(automaton->transition_table[state][code_of(symbol)], next_state);
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

        if (automaton->transition_table[current_state][code_of(symbol)] == NULL)
            return 0;

        // Get the only element in the set
        IntSetIterator *it = intset_iterator_create(automaton->transition_table[current_state][code_of(symbol)]);
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

        moved = automaton->transition_table[state][code_of(symbol)];
        if (moved != NULL)
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
            symbol = (char)intset_iterator_next(alphabet_it);

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

    Automaton *dfa = automaton_create(t_size, intset_clone(automaton->alphabet),
                                      0, final_states);

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

void reset_marks(int marks[], int n) {
    for (int i = 0; i < n; i++)
        marks[i] = 0;
}

int get_class(int e, IntSet *set[], int n) {
    for (int i = 0; i < n; i++) {
        if (intset_contains(set[i], e))
            return i;
    }
    return -1;
}

void quotient_set(Automaton *automaton, IntSet **result[], int *n) {
    //Variables
    IntSetIterator *alphabet_it = intset_iterator_create(automaton->alphabet);

    IntSet **P = (IntSet **) malloc(sizeof(IntSet *) * automaton->num_states);
    int p_size;
    IntSet *P2[automaton->num_states]; int p2_size = 0;

    IntSet *X;
    IntSet *X2;
    int mark[automaton->num_states];

    int e, e2;
    int dest1, dest2, class1, class2;
    int same_eq_set;
    char symbol;

    //Init

    P[0] = intset_create();
    for(int i=0; i<automaton->num_states; i++) {
        if (!intset_contains(automaton->final_states, i)) {
            intset_add(P[0], i);
        }
    }

    P[1] = intset_clone(automaton->final_states);
    p_size = 2;

    IntSetIterator *X_it;
    IntSetIterator *X_it2;
    while(1) {
        for (int i=0; i<p_size; i++) {
            X = P[i];

            reset_marks(mark, automaton->num_states);
            X_it = intset_iterator_create(X);
            while (intset_iterator_has_next(X_it)) {
                e = intset_iterator_next(X_it);
                if (mark[e]) continue;
                mark[e] = 1;

                X2 = intset_create_from_value(e);
                X_it2 = intset_iterator_create(X);
                while(intset_iterator_has_next(X_it2)) {
                    e2 = intset_iterator_next(X_it2);
                    if (mark[e2]) continue;

                    same_eq_set = 1;
                    while (intset_iterator_has_next(alphabet_it)) {
                        symbol = (char) intset_iterator_next(alphabet_it);

                        dest1 = intset_get_one(automaton->transition_table[e][code_of(symbol)]);
                        dest2 = intset_get_one(automaton->transition_table[e2][code_of(symbol)]);

                        class1 = get_class(dest1, P, p_size);
                        class2 = get_class(dest2, P, p_size);

                        if(class1 != class2) {
                            same_eq_set = 0;
                            break;
                        }
                    }
                    intset_iterator_reset(alphabet_it);

                    if(same_eq_set) {
                        mark[e2] = 1;
                        intset_add(X2, e2);
                    }
                }
                intset_iterator_free(X_it2);

                P2[p2_size++] = X2;
            }
            intset_iterator_free(X_it);

        }
        //if P == P' then break
        if (p_size == p2_size) {
            for (int i = 0; i < p2_size; i++)
                intset_free(P2[i]);
            break;
        }

        for (int i = 0; i < p2_size; i++) {
            if (i < p_size)
                intset_free(P[i]);
            P[i] = P2[i];
        }
        p_size = p2_size;
        p2_size = 0;

    }

    intset_iterator_free(alphabet_it);
    *n = p_size;
    *result = P;


}

Automaton *automaton_minimize(Automaton* automaton) {
    IntSet **qset = (IntSet **)malloc(automaton->num_states * sizeof(IntSet *));
    int num_states = -1;

    quotient_set(automaton, &qset, &num_states);

    int start_state = get_class(automaton->initial_state, qset, num_states);
    IntSet *alphabet = intset_clone(automaton->alphabet);

    IntSet *finals = intset_create();
    IntSetIterator *finals_it = intset_iterator_create(automaton->final_states);
    while (intset_iterator_has_next(finals_it))
        intset_add(finals, get_class(intset_iterator_next(finals_it), qset, num_states));
    intset_iterator_free(finals_it);

    Automaton *minimized = automaton_create(num_states, alphabet, start_state, finals);

    IntSetIterator *X_it;
    IntSetIterator *alphabet_it = intset_iterator_create(automaton->alphabet);
    for (int i = 0; i < num_states; i++) {
        X_it = intset_iterator_create(qset[i]);

        while (intset_iterator_has_next(X_it)) {
            int state = intset_iterator_next(X_it);

            while (intset_iterator_has_next(alphabet_it)) {
                char symbol = intset_iterator_next(alphabet_it);

                if (automaton->transition_table[state][code_of(symbol)] == NULL) continue;

                int dest = intset_get_one(automaton->transition_table[state][code_of(symbol)]);
                automaton_add_transition(minimized, i, symbol, get_class(dest, qset, num_states));
            }
            intset_iterator_reset(alphabet_it);
        }
        intset_iterator_free(X_it);
    }
    intset_iterator_free(alphabet_it);
    for (int i = 0; i < num_states; i++)
        intset_free(qset[i]);

    return minimized;
}




void automaton_print(Automaton *automaton) {
    IntSet *states = intset_create();
    for (int i = automaton->num_states-1; i >= 0; i--)
        intset_add(states, i);

    printf("Automaton:");
    printf("\n  States: "); intset_print(states); intset_free(states);
    printf("\n  Alphabet: "); intset_print_chars(automaton->alphabet);
    printf("\n  Initial state: %d", automaton->initial_state);
    printf("\n  Final states: "); intset_print(automaton->final_states);

    printf("\n  Transitions:");
    for (int i = 0; i < automaton->num_states; i++) {
        for (int j = 0; j < MAX_ALPHABET_SIZE; j++) {
            IntSet *to = automaton->transition_table[i][j];

            if (to != NULL) {
                printf("\n    %d -- %c --> ", i, j + 32); intset_print(to);
            }
        }
    }
    printf("");
}
