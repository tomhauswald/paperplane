cmake_minimum_required(VERSION 3.13)

project(paperplane-search)

add_compile_options(
  -std=c++20
  -Ofast
  -march=native
  -Wall
  -Wextra
  -Werror
)

add_executable(paperplane-search
  ./source/main.cc
)

target_link_libraries(paperplane-search
  -lstdc++fs
)
