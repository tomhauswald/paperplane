#include "ppp_fs.h"

#include <filesystem>
#include <fstream>

namespace ppp::fs {

bool FileExists(std::string const &path) {
  return std::filesystem::is_regular_file(path);
}

bool DirectoryExists(std::string const &path) {
  return std::filesystem::is_directory(path);
}

std::string GetBasename(std::string const &path) {
  ppp_assert(path.size() > 2);
  ppp_assert(path.at(path.size() - 1) != '/');
  return path.substr(path.find_last_of('/') + 1);
}

std::string ToAbsolutePath(std::string const &path) {
  return std::filesystem::canonical(path).string();
}

std::vector<std::string> GetSubdirectories(std::string const &path) {
  std::vector<std::string> subdirs;
  for (auto const &entry : std::filesystem::directory_iterator(path))
    subdirs.push_back(entry.path().string());
  return subdirs;
}

std::vector<std::string> ReadLinesFromFile(std::string const &path) {
  std::vector<std::string> lines;
  std::string line;

  std::ifstream file(path);
  ppp_assert_msg(file.good(), "File must exist.");

  while (getline(file, line))
    lines.push_back(line);

  return lines;
}

std::string ReadFirstLineFromFile(std::string const &path) {
  std::string line;
  std::ifstream file(path);
  ppp_assert_msg(file.good(), "File must exist.");
  ppp_assert_msg(getline(file, line), "File must contain a line.");
  return line;
}

} // namespace ppp::fs
