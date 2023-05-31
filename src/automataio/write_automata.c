#include <stdio.h>
#include <stdlib.h>

#include "automata/automata.h"
#include "automataio/write_automata.h"

void write_automaton(Automaton *a, char filename[]) {
    FILE *f = fopen(filename, "w");
    if (f == NULL)
        exit(EXIT_FAILURE);

    fputs("digraph{\n", f);

    // Write initial state
    fputs("  inic[shape=point];\n", f);
    fprintf(f, "  inic->q%d;\n", a->initial_state);

    fputs("\n", f);

    // Write transitions
    IntSet *alphabet_and_lambda = intset_clone(a->alphabet);
    intset_add(alphabet_and_lambda, '_');
    IntSetIterator *alphabet_it = intset_iterator_create(alphabet_and_lambda);
    char symbol;
    char symbols[MAX_ALPHABET_SIZE];
    int symbols_count = 0;
    IntSet *dest_states;
    for (int i = 0; i < a->num_states; i++) {
        for (int j = 0; j < a->num_states; j++) {
            symbols_count = 0;
            while (intset_iterator_has_next(alphabet_it)) {
                symbol = intset_iterator_next(alphabet_it);

                dest_states = a->transition_table[i][code_of(symbol)];
                if (dest_states != NULL) {
                    if (intset_contains(dest_states, j))
                        symbols[symbols_count++] = symbol;
                }
            }
            intset_iterator_reset(alphabet_it);

            if (symbols_count > 0) {
                fprintf(f, "  q%d->q%d[label=\"%c", i, j, symbols[0]);
                for (int k = 1; k < symbols_count; k++)
                    fprintf(f, ",%c", symbols[k]);
                fputs("\"];\n", f);
            }
        }
    }
    intset_iterator_free(alphabet_it);
    intset_free(alphabet_and_lambda);

    fputs("\n", f);

    // Write final states
    IntSetIterator *final_states_it = intset_iterator_create(a->final_states);
    while (intset_iterator_has_next(final_states_it))
        fprintf(f, "  q%d[shape=doublecircle];\n", intset_iterator_next(final_states_it));
    intset_iterator_free(final_states_it);

    fputs("}", f);
    fclose(f);
}
