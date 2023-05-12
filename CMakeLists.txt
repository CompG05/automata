cmake_minimum_required(VERSION 3.0)
project(Automata)

include_directories(include)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${Automata_SOURCE_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${Automata_SOURCE_DIR}/lib)

set(test_dir ${Automata_SOURCE_DIR}/tests)

add_library(utils src/utils/intlist.c src/utils/intset.c)
add_library(automata src/automata/automata.c)

add_executable(test_intlist ${test_dir}/test_intlist.c)
target_link_libraries(test_intlist utils)

add_executable(test_intset ${test_dir}/test_intset.c)
target_link_libraries(test_intset utils)

enable_testing()

add_test(NAME test_intlist COMMAND $<TARGET_FILE:test_intlist>)
add_test(NAME test_intset COMMAND $<TARGET_FILE:test_intset>)

