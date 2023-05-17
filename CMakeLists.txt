cmake_minimum_required(VERSION 3.0)
project(Automata)

include_directories(include)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${Automata_SOURCE_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${Automata_SOURCE_DIR}/lib)

set(test_dir ${Automata_SOURCE_DIR}/tests)

set(automataio_sources src/automataio/read_automata.c src/automataio/write_automata.c)

add_library(utils src/utils/intlist.c src/utils/intset.c)
add_library(automata src/automata/automata.c)
target_link_libraries(automata m)

add_library(automataio ${automataio_sources})

add_executable(main src/main.c)
target_link_libraries(main automataio automata utils)

add_executable(test_intlist ${test_dir}/test_intlist.c)
target_link_libraries(test_intlist utils)

add_executable(test_intset ${test_dir}/test_intset.c)
target_link_libraries(test_intset utils)

add_executable(test_automata ${test_dir}/test_automata.c)
target_link_libraries(test_automata automata utils)

enable_testing()

add_test(NAME test_intlist COMMAND $<TARGET_FILE:test_intlist>)
add_test(NAME test_intset COMMAND $<TARGET_FILE:test_intset>)
add_test(NAME test_automata COMMAND $<TARGET_FILE:test_automata>)

