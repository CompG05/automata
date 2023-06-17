#include <stdio.h>

#include "parser/parser.h"
#include "automata/automata.h"
#include "utils/intset.h"

static IntSet *ds[15];

void get_directive_symbols();
void free_directive_symbols();
int test_cc(char *input, int *cc_idx, char c);

int derive_S(char *input, int *cc_idx, Automaton **a);
int derive_E(char *input, int *cc_idx, Automaton **a);
int derive_Ep(char *input, int *cc_idx, Automaton **a);
int derive_T(char *input, int *cc_idx, Automaton **a);
int derive_Tp(char *input, int *cc_idx, Automaton **a);
int derive_F(char *input, int *cc_idx, Automaton **a);
int derive_Fp(char *input, int *cc_idx, Automaton **a);
int derive_P(char *input, int *cc_idx, Automaton **a);
int derive_L(char *input, int *cc_idx, Automaton **a);
Automaton *create_automaton_from_char(char c);


int parse(char *input, Automaton **a) {
    get_directive_symbols();

    int cc_idx = 0;
    if (!derive_S(input, &cc_idx, a)) {
        free_directive_symbols();
        return 0;
    }

    free_directive_symbols();
    return input[cc_idx] == '\0';
}

int derive_S(char *input, int *cc_idx, Automaton **a) {
    if (intset_contains(ds[0], input[*cc_idx])) {
        return derive_E(input, cc_idx, a);
    }else{
        return 0;
    }
}

int derive_E(char *input, int *cc_idx, Automaton **a) {
    if (intset_contains(ds[1], input[*cc_idx])) {
        if (!derive_T(input, cc_idx, a)) return 0;
        if (!derive_Ep(input,cc_idx, a)) return 0;
        return 1;
    } else {
        return 0;
    }
}

int derive_Ep(char *input, int *cc_idx, Automaton **a) {
    if (intset_contains(ds[2], input[*cc_idx])) {
        Automaton *b, *aux;
        if (!test_cc(input, cc_idx, '|')) return 0;
        if (!derive_T(input, cc_idx, &b)) return 0;
        if (!derive_Ep(input, cc_idx, &b)) return 0;
        aux = *a;
        *a = automaton_union(*a, b);
        automaton_free(aux);
        automaton_free(b);
        return 1;
    } else if (!intset_contains(ds[3], input[*cc_idx])) {
        return 0;
    }
}

int derive_T(char *input, int *cc_idx, Automaton **a) {
    if (intset_contains(ds[4], input[*cc_idx])) {
        if (!derive_F(input, cc_idx, a)) return 0;
        if (!derive_Tp(input, cc_idx, a)) return 0;
        return 1;
    } else {
        return 0;
    }
}

int derive_Tp(char *input, int *cc_idx, Automaton **a) {
    if (intset_contains(ds[5], input[*cc_idx])) {
        Automaton *b, *aux;
        if (!test_cc(input, cc_idx, '.')) return 0;
        if (!derive_F(input, cc_idx, b)) return 0;
        if (!derive_Tp(input, cc_idx, b)) return 0;
        aux = *a;
        *a = automaton_concat(*a, b);
        automaton_free(aux);
        automaton_free(b);
        return 1;
    } else if (!intset_contains(ds[6], input[*cc_idx])) {
        return 0;
    }
}

int derive_F(char *input, int *cc_idx, Automaton **a) {
    if (intset_contains(ds[7], input[*cc_idx])) {
        if (!derive_P(input, cc_idx, a)) return 0;
        if (!derive_Fp(input, cc_idx, a)) return 0;
        return 1;
    } else {
        return 0;
    }
}

int derive_Fp(char *input, int *cc_idx, Automaton **a) {
    if (intset_contains(ds[8], input[*cc_idx])) {
        Automaton *aux;
        if (!test_cc(input, cc_idx, '*')) return 0;
        aux = *a;
        *a = automaton_kclosure(*a);
        automaton_free(aux);
        return 1;
    } else if (!intset_contains(ds[9], input[*cc_idx])) {
        return 0;
    }
}

int derive_P(char *input, int *cc_idx, Automaton **a) {
    if (intset_contains(ds[10], input[*cc_idx])) {
        if (!test_cc(input, cc_idx, '(')) return 0;
        if (!derive_E(input, cc_idx, a)) return 0;
        if (!test_cc(input, cc_idx, ')')) return 0;
        return 1;
    } else if (intset_contains(ds[11], input[*cc_idx])) {
        if (!derive_L(input, cc_idx, a)) return 0;
        return 1;
    } else {
        return 0;
    }
}
int derive_L(char *input, int *cc_idx, Automaton **a) {
    if (input[*cc_idx] == 'a' || input[*cc_idx] == 'b' || input[*cc_idx] == 'c') {
        *a = create_automaton_from_char(input[*cc_idx]);
        (*cc_idx)++;
        return 1;
    } else {
        return 0;
    }
}

Automaton *create_automaton_from_char(char c) {
    IntSet *alphabet = intset_create_from_value(c);
    IntSet *finals = intset_create_from_value(1);
    Automaton *a = automaton_create(2, alphabet,0,finals);
    automaton_add_transition(a, 0, c, 1);
    return a;
}

void get_directive_symbols() {
    ds[0] = intset_from_array((int[]){'(', 'a', 'b', 'c'}, 0, 4);
    ds[1] = intset_from_array((int[]){'(', 'a', 'b', 'c'}, 0, 4);
    ds[2] = intset_create_from_value('|');
    ds[3] = intset_from_array((int[]){'\0', ')'}, 0, 2);
    ds[4] = intset_from_array((int[]){'(', 'a', 'b', 'c'}, 0, 4);
    ds[5] = intset_create_from_value('.');
    ds[6] = intset_from_array((int[]){'|', '\0', ')'}, 0, 3);
    ds[7] = intset_from_array((int[]){'(', 'a', 'b', 'c'}, 0, 4);
    ds[8] = intset_create_from_value('*');
    ds[9] = intset_from_array((int[]){'.', '|', '\0', ')'}, 0, 4);
    ds[10] = intset_create_from_value('(');
    ds[11] = intset_from_array((int[]){'a', 'b', 'c'}, 0, 3);
    ds[12] = intset_create_from_value('a');
    ds[13] = intset_create_from_value('b');
    ds[14] = intset_create_from_value('c');
}

void free_directive_symbols() {
    for (int i = 0; i < 15; i++) {
        intset_free(ds[i]);
    }
}

int test_cc(char *input, int *cc_idx, char c) {
    if (input[*cc_idx] == c) {
        (*cc_idx)++;
        return 1;
    }
    else
        return 0;
}
