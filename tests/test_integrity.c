#include <stdio.h>
#include <stdlib.h>

#include "automata/automata.h"
#include "automataio/read_automata.h"
#include "automataio/write_automata.h"

int main() {
    Automaton *a, *d;
    int accepted;

    printf("Testing automaton 1\n\n");

    printf("Reading automaton from tests/files/automatonIT1.dot...\n");
    a = read_automaton("../tests/files/automatonIT1.dot");
    automaton_print(a);
    printf("\n");

    printf("Determinizing automaton...\n");
    d = automaton_determinize(a);
    automaton_free(a);

    printf("Writing dfa to tests/files/d_automatonIT1.dot...\n");
    write_automaton(d, "../tests/files/d_automatonIT1.dot");

    printf("Tesgting strings...\n");
    char *valid_strings1[] = {"0", "0110", "100110101"};
    char *invalid_strings1[] = {"01", "", "01101011"};

    for (int i = 0; i < 3; i++) {
        accepted = automaton_accepts(d, valid_strings1[i]);
        printf("\"%s\" is %s (expected: accepted)\n", valid_strings1[i], accepted ? "accepted" : "rejected");
        if (!accepted)
            exit(EXIT_FAILURE);
    }

    for (int i = 0; i < 3; i++) {
        accepted = automaton_accepts(d, invalid_strings1[i]);
        printf("\"%s\" is %s (expected: rejected)\n", invalid_strings1[i], accepted ? "accepted" : "rejected");
        if (accepted)
            exit(EXIT_FAILURE);
    }

    automaton_free(d);

    printf("Testing automaton 2\n\n");

    printf("Reading automaton from tests/files/automatonIT2.dot...\n");
    a = read_automaton("../tests/files/automatonIT2.dot");
    automaton_print(a);
    printf("\n");

    printf("Determinizing automaton...\n");
    d = automaton_determinize(a);
    automaton_free(a);

    printf("Writing dfa to tests/files/d_automatonIT2.dot...\n");
    write_automaton(d, "../tests/files/d_automatonIT2.dot");

    printf("Tesgting strings...\n");
    char *valid_strings2[] = {"c", "babab", "bbc"};
    char *invalid_strings2[] = {"aaab", "baaa", ""};

    for (int i = 0; i < 3; i++) {
        accepted = automaton_accepts(d, valid_strings2[i]);
        printf("\"%s\" is %s (expected: accepted)\n", valid_strings2[i], accepted ? "accepted" : "rejected");
        if (!accepted)
            exit(EXIT_FAILURE);
    }

    for (int i = 0; i < 3; i++) {
        accepted = automaton_accepts(d, invalid_strings2[i]);
        printf("\"%s\" is %s (expected: rejected)\n", invalid_strings2[i], accepted ? "accepted" : "rejected");
        if (accepted)
            exit(EXIT_FAILURE);
    }

    automaton_free(d);

    printf("Testing automaton 3\n\n");

    printf("Reading automaton from tests/files/automatonIT3.dot...\n");
    a = read_automaton("../tests/files/automatonIT3.dot");
    automaton_print(a);
    printf("\n");

    printf("Determinizing automaton...\n");
    d = automaton_determinize(a);
    automaton_free(a);

    printf("Writing dfa to tests/files/d_automatonIT3.dot...\n");
    write_automaton(d, "../tests/files/d_automatonIT3.dot");

    printf("Tesgting strings...\n");
    char *valid_strings3[] = {"", "0022", "1"};
    char *invalid_strings3[] = {"10", "020", "3"};

    for (int i = 0; i < 3; i++) {
        accepted = automaton_accepts(d, valid_strings3[i]);
        printf("\"%s\" is %s (expected: accepted)\n", valid_strings3[i], accepted ? "accepted" : "rejected");
        if (!accepted)
            exit(EXIT_FAILURE);
    }

    for (int i = 0; i < 3; i++) {
        accepted = automaton_accepts(d, invalid_strings3[i]);
        printf("\"%s\" is %s (expected: rejected)\n", invalid_strings3[i], accepted ? "accepted" : "rejected");
        if (accepted)
            exit(EXIT_FAILURE);
    }

    automaton_free(d);

    exit(EXIT_SUCCESS);
}
