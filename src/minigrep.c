#include "automata/automata.h"
#include "parser/parser.h"
#include <stdio.h>
#include <stdlib.h>

Automaton *create_universe_automaton();
void concat_universe(Automaton **a);

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Usage: %s <regex> <input>\n", argv[0]);
        exit(1);
    }

    char *regex = argv[1];
    char *input = argv[2];
    Automaton *a, *det_a, *aux;

    // Get automaton
    int valid_regex = parse(regex, &a);
    if (!valid_regex) {
        automaton_free(a);
        fprintf(stderr, "Invalid regex\n");
        exit(1);
    }

    // Concatenate sigma* on both sides
    concat_universe(&a);

    // Determinize
    det_a = automaton_determinize(a);
    automaton_free(a);

    int input_accepted = automaton_accepts(det_a, input);
    if (input_accepted)
        printf("Accepted\n");
    else
        printf("Rejected\n");
}

Automaton *create_universe_automaton(){
    int symbols[] = {'a', 'b', 'c'};
    IntSet *alphabet = intset_from_array(symbols, 0, 3);
    IntSet *finals = intset_create_from_value(0);
    Automaton *a = automaton_create(1, alphabet, 0, finals);
    for(int i=0; i<3; i++){
        automaton_add_transition(a,0,(char)symbols[i],0);
    }
    return a;
}

void concat_universe(Automaton **a) {
    Automaton *aux;
    Automaton *U = create_universe_automaton();

    // Concat left
    aux = *a;
    *a = automaton_concat(U, *a);
    automaton_free(aux);

    // Concat right
    aux = *a;
    *a = automaton_concat(*a, U);
    automaton_free(aux);
}