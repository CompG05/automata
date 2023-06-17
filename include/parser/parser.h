#ifndef AUTOMATA_PARSER_PARSER_H
#define AUTOMATA_PARSER_PARSER_H

#include "automata/automata.h"

/// Return 1 if the input string is accepted by the grammar, 0 otherwise
/// Pre: input ends with '\0'
int parse(char *input, Automaton **a);

#endif // AUTOMATA_PARSER_PARSER_H