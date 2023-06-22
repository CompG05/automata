#include <stdio.h>
#include <stdlib.h>
#include "minigrep_utils.h"

int test_regex_with_strings(char *er, char* accepted_str[], char* rejected_str[]) {
    for (int i=0; i<3; i++){
        printf("Testing %s with %s (Expected: Accepted)\n", er, accepted_str[i]);
        if (!minigrep(er,accepted_str[i])) {
            printf("Result: Rejected\n");
            exit(EXIT_FAILURE);
        }
        printf("Result: Accepted\n");
    }

    for (int i=0; i<3; i++){
        printf("Testing %s with %s (Expected: Rejected)\n", er, rejected_str[i]);
        if (minigrep(er,rejected_str[i])) {
            printf("Result: Accepted\n");
            exit(EXIT_FAILURE);
        }
        printf("Result: Rejected\n");
    }
}

int main() {
    char *re1 = "(a|b).c";
    char *re2 = "a.(a|b)*.c";
    char *re3 = "a.b.c.(a|b).(a*|b*)*";

    test_regex_with_strings(re1,
                            (char *[]){"ac", "ac", "bc"},
                            (char *[]){"aa", "a", ""});
    test_regex_with_strings(re2,
                            (char *[]){"ac", "ababc", "aacabc"},
                            (char *[]){"b", "caa", "bca"});
    test_regex_with_strings(re3,
                            (char *[]){"cababcacba", "abcbaaa", "abcabababa"},
                            (char *[]){"abc", "cabcc", ""});
}