#include <stdio.h>
#include <stdlib.h>
#include "minigrep_utils.h"

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Usage: %s <regex> <input>\n", argv[0]);
        exit(1);
    }
    char *regex = argv[1];
    char *input = argv[2];
    int input_accepted = minigrep(regex, input);
    if (input_accepted)
        printf("Accepted\n");
    else
        printf("Rejected\n");

}