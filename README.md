# automata

This projects is for the 1^{st} and 2^{nd} PAs for the Automata and Languajes course

Authors:
    - D'Autilio Joel
    - Rossi Pablo

This project requires cmake for building.

To build, execute [build.sh](build.sh).

- [automata.c](src/automata/automata.c) contains an implementation for Non-Deterministic Finite Automata.
- [automata_ops.c](src/automata/automata_ops.c) contains the operations of union, concatenation and Kleene closure for automata.
- [automata_read.c](src/automataio/automata_read.c) and [automata_write.c](src/automataio/automata_write.c) contain functions to read and write automata to a file.


The main tests can be executed by changing into the bin/ dir and running:
    - test_integrity: tests reading, writing, determinizing and acceptance of strings in a single integral test, corresponding to the 1st PA ([tests/test_integrity.c](tests/test_integrity.c))
    - test_integrity2: tests union, concatenation, kleene closure and minimization, corresponding to the 2nd PA ([tests/test_integrity2.c](tests/test_integrity2.c))

Other tests can be found in the bin/ directory, like some unit tests.
