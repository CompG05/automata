#ifndef AUTOMATA_MINIGREP_H
#define AUTOMATA_MINIGREP_H

#include "automata/automata.h"

int minigrep(char *regex, char *input);

Automaton *create_universe_automaton();

void concat_universe(Automaton **a);


#endif // AUTOMATA_MINIGREP_H
