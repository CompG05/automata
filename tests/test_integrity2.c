+q
int main() {
    // Read simple automata
    Automaton *a = read_automaton("../tests/files/test_integrity2_a.dot"); // e1 = b⁺
    Automaton *b = read_automaton("../tests/files/test_integrity2_b.dot"); // e2 = ac*
    Automaton *c = read_automaton("../tests/files/test_integrity2_c.dot"); // e3 = a*b

    // Apply operations ((e1|e2).e3)* = ((b*|ac*).a*b)*
    Automaton *union_aut = automaton_union(a,b);
    write_automaton(union_aut, "../tests/files/union.dot");
    Automaton *concat_aut = automaton_concat(union_aut,c);
    write_automaton(concat_aut, "../tests/files/concat.dot");
    Automaton *result = automaton_kclosure(concat_aut);
    write_automaton(result, "../tests/files/kleene.dot");

    // Determinize resulting automaton
    Automaton *det_aut = automaton_determinize(result);
    write_automaton(det_aut, "../tests/files/determ.dot");

    // Minimize automaton
    Automaton *min_aut = automaton_minimize(det_aut);

    // Write to .dot file
    write_automaton(min_aut, "../tests/files/test_integrity2_minimized.dot");

    // Test strings
}