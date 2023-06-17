#ifndef AUTOMATA_PARSER_PARSER_H
#define AUTOMATA_PARSER_PARSER_H

/// Return 1 if the input string is accepted by the grammar, 0 otherwise
/// Pre: input ends with '\0'
int parse(char *input);

#endif // AUTOMATA_PARSER_PARSER_H