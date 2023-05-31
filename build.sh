#!/bin/sh

mkdir build bin lib
cd build
cmake ..
cd ..
cmake --build build/
