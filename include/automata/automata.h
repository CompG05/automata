//===-- automata/automata.h - Automaton ---------------------------*- C -*-===//
//
// Define the automaton structure.
// The automaton is a 5-tuple (Q, Σ, δ, q0, F) where:
// - Q is a finite set of states;
// - Σ is a finite set of symbols called the alphabet;
// - δ is the transition function where δ: Q × Σ∪{λ} → P(Q);
// - q0 is the initial state from where any input is processed (q0 ∈ Q);
// - F is a set of final states (F ⊆ Q).
//
//===----------------------------------------------------------------------===//

#ifndef AUTOMATA_AUTOMATA_H
#define AUTOMATA_AUTOMATA_H

#include "utils/intset.h"

const int MAX_ALPHABET_SIZE = 127 - 32;
const int LAMBDA_CODE = '_' - 32;

typedef struct Automaton {
    int num_states;
    IntSet *alphabet;
    IntSet ***transition_table; // (IntSet *) Matrix
    int initial_state;
    IntSet *final_states;
} Automaton;

typedef struct Transition {
  int from;
  char symbol;
  IntSet *to;
} Transition;

/// Create a new automaton.
Automaton *automaton_create(int num_states, IntSet *alphabet, int initial_state, IntSet *final_states);

/// Create a new transition.
Transition *transition_create(int from, char symbol, IntSet *to);

/// Free the transition.
void transition_free(Transition *transition);

/// Add a transition to the automaton.
void automaton_add_transition(Automaton *automaton, int state, char symbol, int next_state);

/// Free the memory allocated by the automaton.
void automaton_free(Automaton *automaton);

/// Return 1 if the automaton accepts the string, 0 otherwise.
int automaton_accepts(Automaton *automaton, char *string);

/// Return a deterministic automaton equivalent to the given one.
/// Parameters:
/// - automaton: the automaton to determinize.
Automaton *automaton_determinize(Automaton *automaton);

/// Print the automaton.
void automaton_print(Automaton *automaton);

/// Print the automaton in the DOT format.
void automaton_print_dot(Automaton *automaton);

#endif // AUTOMATA_AUTOMATA_H
