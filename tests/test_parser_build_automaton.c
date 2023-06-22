#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "parser/parser.h"
#include "automata/automata.h"
#include "automataio/write_automata.h"

/// Pre: accepted_str and rejected_str are arrays of length == 3
int test(char* file_id, char *er, char* accepted_str[], char* rejected_str[]) {
    Automaton *a;
    int is_re = parse(er, &a);
    if (!is_re) {
        automaton_free(a);
        printf("Invalid Regular Expression");
        exit(EXIT_FAILURE);
    }

    char file_path[100];
    sprintf(file_path, "../tests/files/test_parser_build_automaton_%s.dot", file_id);

    printf("\nWriting automaton for %s in: %s\n\n", er, file_path);
    write_automaton(a, file_path);

    Automaton *det_a = automaton_determinize(a);
    automaton_free(a);

    printf("Testing for %s\n\n", er);

    for (int i=0; i<3; i++){
        printf("Testing %s (Expected: Accepted)\n", accepted_str[i]);
        if (!automaton_accepts(det_a,accepted_str[i])){
            automaton_free(det_a);
            printf("Result: Rejected\n");
            exit(EXIT_FAILURE);
        }
        printf("Result: Accepted\n");
    }

    printf("\n");

    for (int i=0; i<3; i++){
        printf("Testing %s (Expected: Rejected)\n", rejected_str[i]);
        if (automaton_accepts(det_a,rejected_str[i])){
            automaton_free(det_a);
            printf("Result: Accepted\n");
            exit(EXIT_FAILURE);
        }
        printf("Result: Rejected\n");
    }
    printf("\n\n");

    automaton_free(det_a);
}

int main() {
    char *re1 = "(a|b).c";
    char *re2 = "(a.(a|b)*.c)*";
    char *re3 = "a.b.c.(a|b).(a*|b*)*";

    test("re1", re1, (char *[]){"ac", "ac", "bc"},(char *[]){"aa", "a", ""});
    test("re2", re2,
        (char *[]){"ac", "ababc", "aacabc"},
        (char *[]){"b", "acc", "cabc"});
    test("re3", re3,
        (char *[]){"abca", "abcbaaa", "abcabababa"},
        (char *[]){"bcab", "cabcab", ""});

    printf("\nAll test passed!\n");

    return 0;
}