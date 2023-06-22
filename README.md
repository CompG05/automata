# automata

This project is for the Practical Assignments for the Automata and Languages course.

In the section [PA3](#pa3---regex-parser) can be found information about the 3rd practical assignment. The first and second assignments are explained in the [The Project](#the-project) section.

### Authors:

- D'Autilio Joel
- Rossi Pablo

### Building

This project requires cmake for building.

To build, execute [build.sh](build.sh).

## PA3 - Regex parser

### Execute

A [minigrep](bin/minigrep) executable is built into the /bin directory that checks whether a given regex accepts a given string. Example:

    $ bin/minigrep "a.(b|c)*.a" abcba
    > Accepted

    $ bin/minigrep "a.(b|c)*.a" bab
    > Rejected

### Main files

The main files for this assignment are:
    
- [regex_grammar.txt](regex_grammar.txt) contains the grammar for a regex in LL(1).
- [parser.c](src/parser/parser.c) contains the parse() function to check whether a given regex is valid, and generate an automaton if it is.
- [minigrep_utils.c](src/minigrep_utils.c) contains the minigrep() function to check whether a given string is accepted by a given regex.

### Tests

The following tests can be run via ctest:

    cd build
    ctest

or by changing into the [bin/](bin) directory and executing them.

- [test_parser](tests/test_parser.c) test that the parser accepts valid regexs and rejects invalid ones.
- [test_parser_build_automaton](tests/test_parser_build_automaton.c) test that the parser builds correct automata.
- [test_minigrep](tests/test_minigrep.c) Test that minigrep accepts and rejects the correct strings, given a regex.

## The project

The main files are:

- [automata.c](src/automata/automata.c) contains an implementation for Non-Deterministic Finite Automata.
- [automata_ops.c](src/automata/automata_ops.c) contains the operations of union, concatenation and Kleene closure for automata.
- [read_automata.c](src/automataio/read_automata.c) and [write_automata.c](src/automataio/write_automata.c) contain functions to read and write automata to a file.


The main tests can be executed by changing into the bin/ dir and running:
    
- test_integrity: tests reading, writing, determinizing and acceptance of strings in a single integral test, corresponding to the 1<sup>st</sup> PA ([tests/test_integrity.c](tests/test_integrity.c))
- test_integrity2: tests union, concatenation, kleene closure and minimization, corresponding to the 2<sup>nd</sup> PA ([tests/test_integrity2.c](tests/test_integrity2.c))

Other tests can be found in the bin/ directory, like some unit tests.
