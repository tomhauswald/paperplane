cmake_minimum_required(VERSION 3.10)

project(paperplane)

add_executable(pppcli
  ./source/ppp_cli.cc
  ./source/ppp_db.cc
  ./source/ppp_fs.cc
)

target_compile_options(pppcli
  PRIVATE -std=c++20
  PRIVATE -Ofast
  PRIVATE -march=native
  PRIVATE -Wall
  PRIVATE -Wextra
  PRIVATE -Wno-unused-variable
  PRIVATE -Werror
)

target_link_libraries(pppcli
  PRIVATE -lstdc++fs
)
