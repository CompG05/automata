#include <stdio.h>
#include <stdlib.h>

#include "parser/parser.h"

int main() {
    char *accepted_re[] = {"(a|b).c", "(a.(a|b)*.c)*", "a.b.c.(a|b).(a*|b*)*", "(a.(b).c|b.b.a|a*)"};
    char *rejected_re[] = {"(a|).c", "(a.(a|b)*c)*", "a.b.c.b).(a*|b*)*", "(a.(b)..c|b.b.a|a*)" , "a**"};

    for (int i = 0; i<4; i++){
        printf("Testing parser with: %s  (expected: Accept)\n", accepted_re[i]);
        if(parse(accepted_re[i]))
            printf("Result: Accept\n");
        else {
            printf("Result: Rejected\n");
            exit(1);
        }
    }

    for (int i = 0; i<4; i++){
        printf("Testing parser with: %s  (expected: Reject)\n", rejected_re[i]);
        if(!parse(rejected_re[i]))
            printf("Result: Rejected\n");
        else {
            printf("Result: Accepted\n");
            exit(1);
        }
    }

    return 0;
}