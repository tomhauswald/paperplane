#include "fs.hh"

#include <cassert>
#include <filesystem>
#include <fstream>

namespace ppp::fs {

bool DirectoryExists(std::string const &path) {
  return std::filesystem::is_directory(path);
}

std::vector<std::string> ReadLinesFromFile(std::string const &path) {
  std::vector<std::string> lines;
  std::string line;

  std::ifstream file(path);
  assert(file.good() && "File must exist.");

  while (getline(file, line))
    lines.push_back(line);

  return lines;
}

std::string ReadFirstLineFromFile(std::string const &path) {
  std::string line;
  std::ifstream file(path);
  assert(file.good() && "File must exist.");
  assert(getline(file, line) && "File must contain a line.");
  return line;
}

std::vector<std::string> GetSubdirectories(std::string const &path) {
  std::vector<std::string> subdirs;
  for (auto const &entry : std::filesystem::directory_iterator(path))
    subdirs.push_back(entry.path().string());
  return subdirs;
}

} // namespace ppp::fs
