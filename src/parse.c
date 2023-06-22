#include <stdio.h>
#include <stdlib.h>
#include "automata/automata.h"
#include "automataio/write_automata.h"
#include "parser/parser.h"

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Use: %s <regex> <out_file>\n", argv[0]);
        exit(1);
    }

    char *regex = argv[1];
    char *out_file = argv[2];
    Automaton *a;

    if (!parse(regex, &a)) {
        automaton_free(a);
        printf("Invalid regex\n");
        exit(1);
    }

    write_automaton(a, out_file);
    automaton_free(a);
}