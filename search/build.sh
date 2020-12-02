#!/bin/bash

sources="\
  main.cc \
"

set -o pipefail
set -x

mkdir -p obj
for src in ${sources}; do
  clang++-10 -std=c++20 -Ofast -march=native -c -o obj/${src%.*}.o ${src}
done

mkdir -p bin
clang++-10 -std=c++20 -Ofast -march=native -o bin/search obj/*.o
