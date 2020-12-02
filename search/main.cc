#include <cassert>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

std::vector<std::string> ReadLines(char const *path) {
  std::ifstream file(path);
  
  std::vector<std::string> lines;
  for(std::string line; getline(file, line);)
    lines.push_back(line);

  return lines; 
}

int main(int argc, char const *argv[]) {
  assert(argc == 2);
  auto tokens = ReadLines(argv[1]);
  

  return 0;
}
