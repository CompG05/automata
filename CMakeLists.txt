cmake_minimum_required(VERSION 3.0)
project(Automata)

include_directories(include)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${Automata_SOURCE_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${Automata_SOURCE_DIR}/lib)

set(test_dir ${Automata_SOURCE_DIR}/tests)

set(automata_sources src/automata/automata.c src/automata/automata_ops.c)
set(utils_sources src/utils/intlist.c src/utils/intset.c)
set(automataio_sources src/automataio/read_automata.c src/automataio/write_automata.c)

add_library(utils ${utils_sources})
add_library(automata ${automata_sources})
target_link_libraries(automata m)

add_library(automataio ${automataio_sources})

add_library(parser src/parser/parser.c)
target_link_libraries(parser automata utils)

add_library(minigrep_utils src/minigrep_utils.c)
target_link_libraries(minigrep_utils parser)

add_executable(parse src/parse.c)
target_link_libraries(parse parser automataio)

add_executable(minigrep src/minigrep.c)
target_link_libraries(minigrep minigrep_utils)

add_executable(test_intlist ${test_dir}/test_intlist.c)
target_link_libraries(test_intlist utils)

add_executable(test_intset ${test_dir}/test_intset.c)
target_link_libraries(test_intset utils)

add_executable(test_automata ${test_dir}/test_automata.c)
target_link_libraries(test_automata automata utils)

add_executable(test_integrity ${test_dir}/test_integrity.c)
target_link_libraries(test_integrity automataio automata utils)

add_executable(test_automata_ops ${test_dir}/test_automata_ops.c)
target_link_libraries(test_automata_ops automata automataio utils)

add_executable(test_minimize ${test_dir}/test_minimize.c)
target_link_libraries(test_minimize automata automataio utils)

add_executable(test_integrity2 ${test_dir}/test_integrity2.c)
target_link_libraries(test_integrity2 automataio automata utils)

add_executable(test_parser ${test_dir}/test_parser.c)
target_link_libraries(test_parser parser)

add_executable(test_parser_build_automaton ${test_dir}/test_parser_build_automaton.c)
target_link_libraries(test_parser_build_automaton parser automataio)

add_executable(test_minigrep ${test_dir}/test_minigrep.c)
target_link_libraries(test_minigrep minigrep_utils)

enable_testing()

add_test(NAME test_intlist COMMAND $<TARGET_FILE:test_intlist>)
add_test(NAME test_intset COMMAND $<TARGET_FILE:test_intset>)
add_test(NAME test_automata COMMAND $<TARGET_FILE:test_automata>)
add_test(NAME test_integrity COMMAND $<TARGET_FILE:test_integrity>)
add_test(NAME test_automata_ops COMMAND $<TARGET_FILE:test_automata_ops>)
add_test(NAME test_integrity2 COMMAND $<TARGET_FILE:test_integrity2>)
add_test(NAME test_parser_build_automaton COMMAND $<TARGET_FILE:test_parser_build_automaton>)
add_test(NAME test_minigrep COMMAND $<TARGET_FILE:test_minigrep>)
