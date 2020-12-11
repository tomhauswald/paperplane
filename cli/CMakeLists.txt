cmake_minimum_required(VERSION 3.13)

project(paperplane-cli)

add_compile_options(
  -std=c++20
  -Ofast
  -march=native
  -Wall
  -Wextra
  -Werror
)

add_executable(paperplane
  ./source/fs.cc
  ./source/db.cc
  ./source/main.cc
)

target_link_libraries(paperplane
  -lstdc++fs
)
