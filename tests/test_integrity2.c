#include <stdio.h>
#include <stdlib.h>

#include "automata/automata.h"
#include "automataio/read_automata.h"
#include "automataio/write_automata.h"

int main() {
    // Read simple automata
    printf("READING\n");
    printf("Reading automaton A from: /tests/files/test_integrity2_a.dot\n");
    printf("Automata A recognize: b⁺\n");
    Automaton *a = read_automaton("../tests/files/test_integrity2_a.dot"); // e1 = b⁺

    printf("Reading automaton B from: /tests/files/test_integrity2_b.dot\n");
    printf("Automata B recognize: ac*\n");
    Automaton *b = read_automaton("../tests/files/test_integrity2_b.dot"); // e2 = ac*

    printf("Reading automaton C from: /tests/files/test_integrity2_c.dot\n");
    printf("Automata C recognize: a*b\n");
    Automaton *c = read_automaton("../tests/files/test_integrity2_c.dot"); // e3 = a*b


    // Apply operations ((e1|e2).e3)* = ((b*|ac*).a*b)*
    printf("\n\nOPERATIONS\n");
    printf("Applying: A U B\n");
    Automaton *union_aut = automaton_union(a,b);
    printf("Applying: (A U B) . C\n");
    Automaton *concat_aut = automaton_concat(union_aut,c);
    printf("Applying: ((A U B) . C)*\n");
    Automaton *result = automaton_kclosure(concat_aut);
    printf("Writing resultant automaton in: /test/files/test_integrity2_result.dot\n\n");
    write_automaton(result, "../tests/files/test_integrity2_result.dot");


    // Determinize resulting automaton
    printf("\n\nDETERMINIZE\n");
    printf("Determinizing resultant automaton\n");
    Automaton *det_aut = automaton_determinize(result);
    write_automaton(det_aut, "../tests/files/determ.dot");

    // Minimize automaton
    Automaton *min_aut = automaton_minimize(det_aut);

    // Write to .dot file
    write_automaton(min_aut, "../tests/files/test_integrity2_minimized.dot");

    // Test strings
    char *valid_strings[] = {"acccaab", "bbbab", "bb", "aaaaaab", "", "acaaab"};
    char *invalid_strings[] = {"c", "acccccc", "ba", "acccaaaaa", "ccc", "b"};
    int accepted_det;
    int accepted_min;

    for (int i = 0; i < 5; i++) {
        accepted_det = automaton_accepts(det_aut, valid_strings[i]);
        accepted_min = automaton_accepts(min_aut, valid_strings[i]);

        if (!accepted_det) {
            printf("ERROR: Determinized automaton does not accept valid string \"%s\" (expected: accepted)\n", valid_strings[i]);
            exit(EXIT_FAILURE);
        }
        if (!accepted_min) {
            printf("ERROR: Minimized automaton does not accept valid string \"%s\" (expected: accepted)\n", valid_strings[i]);
            exit(EXIT_FAILURE);
        }
        printf("Determinized and minimized automata accept valid string \"%s\"\n", valid_strings[i]);

    }

    for (int i = 0; i < 5; i++) {
        accepted_det = automaton_accepts(det_aut, invalid_strings[i]);
        accepted_min = automaton_accepts(min_aut, invalid_strings[i]);

        if (accepted_det) {
            printf("ERROR: Determinized automaton accepts invalid string \"%s\" (expected: not accepted)\n", invalid_strings[i]);
            exit(EXIT_FAILURE);
        }
        if (accepted_min) {
            printf("ERROR: Minimized automaton accepts invalid string \"%s\" (expected: not accepted)\n", invalid_strings[i]);
            exit(EXIT_FAILURE);
        }
        printf("Determinized and minimized automata reject invalid string \"%s\"\n", invalid_strings[i]);

    }

    printf("\nFinished!\n");

    automaton_free(a);
    automaton_free(b);
    automaton_free(c);
    automaton_free(union_aut);
    automaton_free(concat_aut);
    automaton_free(result);
    automaton_free(det_aut);
    automaton_free(min_aut);
    exit(EXIT_SUCCESS);
}
