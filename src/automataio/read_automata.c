#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "automata/automata.h"
#include "automataio/read_automata.h"

int parse_initial(char *s) {
    int initial;

    sscanf(s, "inic->q%d", &initial);

    return initial;
}

void parse_transitions(char *s, Automaton *a) {
    int from, to;
    char symbols[MAX_ALPHABET_SIZE * 2];

    sscanf(s, "q%d->q%d [label=\"%[^\"]", &from, &to, symbols);

    // For symbol in symbols
    for (int i = 0; i < strlen(symbols); i++) {
        char symbol = symbols[i];
        if (symbol == ',') continue;

        automaton_add_transition(a, from, (int)symbol, to);
    }
}

int parse_final(char *s) {
    int final;

    sscanf(s, "q%d%*s", &final);

    return final;
}

void update_num_states(char s[], int *num_states) {
    // Pre: s matches a transition line
    int a, b;

    sscanf(s, "q%d->q%d%*s", &a, &b);

    if (a >= *num_states) *num_states = a + 1;
    if (b >= *num_states) *num_states = b + 1;
}

void update_alphabet(char *s, IntSet *alphabet) {
    // Pre: s matches a transition line
    char label[MAX_ALPHABET_SIZE * 2];

    sscanf(s, "q%*d->q%*d [label=\"%[^\"]", label);

    for (int i = 0; i < strlen(label); i++) {
        char c = label[i];
        if (c == ',' || c == '_') continue;

        intset_add(alphabet, c);
    }
}

Automaton *read_automaton(char filename[]) {
    FILE *fp;
    char *line = NULL;
    size_t len;
    ssize_t read;

    regex_t initial_regex;
    regex_t transition_regex;
    regex_t final_regex;

    regcomp(&initial_regex, "inic->q[0-9]+", REG_EXTENDED);
    regcomp(&transition_regex, "q[0-9]+->q[0-9]+ \\[label=\".(,.)*\"]", REG_EXTENDED);
    regcomp(&final_regex, "q[0-9]+\\[shape=doublecircle]", REG_EXTENDED);

    int num_states = 0;
    IntSet *alphabet = intset_create();
    int n_transitions = 0;
    int start, final;
    IntSet *finals = intset_create();

    fp = fopen(filename, "r");
    if (fp == NULL) {
        printf("Error opening file\n");
        exit(EXIT_FAILURE);
    }

    while ((read = getline(&line, &len, fp)) != -1) {
        // printf("%s\n", line);
        if (!regexec(&initial_regex, line, 0, NULL, 0)) { // If the line defines an initial state
            start = parse_initial(line);
            if (start >= num_states)
                num_states = start + 1;
        } else if (!regexec(&final_regex, line, 0, NULL, 0)) { // If the line defines a final state
            final = parse_final(line);
            intset_add(finals, final);
            if (final >= num_states)
                num_states = final + 1;
        } else if (!regexec(&transition_regex, line, 0, NULL, 0)) { // If it defines a transition
            update_num_states(line, &num_states);
            update_alphabet(line, alphabet);
        }
    }
//    free(line);

    Automaton *a = automaton_create(num_states, alphabet, start, finals);

    rewind(fp);
    while ((read = getline(&line, &len, fp)) != -1) {
        if (!regexec(&transition_regex, line, 0, NULL, 0)) { // If it defines a transition
            parse_transitions(line, a);
        }
    }
//    free(line);
    regfree(&initial_regex);
    regfree(&transition_regex);
    regfree(&final_regex);
    fclose(fp);

    return a;
}
