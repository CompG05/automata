#ifndef AUTOMATA_H
#define AUTOMATA_H

#import "intset.h"

typedef struct Automata {
    IntSet *states;
    IntSet *alphabet;
    IntSet ***transition_table; // (IntSet *) Matrix
    int initial_state;
    IntSet *final_states;
} Automata;

#endif // AUTOMATA_H
