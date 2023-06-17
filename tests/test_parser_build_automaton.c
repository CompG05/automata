#include <stdio.h>
#include <stdlib.h>

#include "parser/parser.h"
#include "automata/automata.h"
#include "automataio/write_automata.h"

int main() {
    char *accepted_re[] = {"(a|b).c", "(a.(a|b)*.c)*", "a.b.c.(a|b).(a*|b*)*"};


    Automaton *a;
    printf("Testing parser with: %s  (expected: Accept)\n", accepted_re[2]);
    parse(accepted_re[2], &a);

    write_automaton(a, "joelquiereprobar.dot");

    automaton_free(a);
    return 0;
}