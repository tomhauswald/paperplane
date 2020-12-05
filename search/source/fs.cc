#include "fs.hh"

#include <cassert>
#include <filesystem>
#include <fstream>

std::vector<std::string> ppp::fs::ReadLinesFromFile(std::string const &path) {
  std::vector<std::string> lines;
  std::string line;

  std::ifstream file(path);
  assert(file.good() && "File must exist.");

  while (getline(file, line))
    lines.push_back(line);

  return lines;
}

std::string ppp::fs::ReadFirstLineFromFile(std::string const &path) {
  std::string line;
  std::ifstream file(path);
  assert(file.good() && "File must exist.");
  assert(getline(file, line) && "File must contain a line.");
  return line;
}

std::vector<std::string> ppp::fs::GetSubdirectories(std::string const &path) {
  std::vector<std::string> subdirs;
  for (auto const &entry : std::filesystem::directory_iterator(path))
    subdirs.push_back(entry.path().string());
  return subdirs;
}
