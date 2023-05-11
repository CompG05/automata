project(automata)
cmake_minimum_required(VERSION 3.0)

add_subdirectory(src)
include_directories(include)

add_library(utils src/utils/intlist.c src/utils/intset.c)
add_library(automata src/automata/automata.c)
