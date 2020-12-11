#pragma once

#include <string>
#include <vector>

namespace ppp::fs {

bool DirectoryExists(std::string const& path);
std::vector<std::string> ReadLinesFromFile(std::string const &path);
std::string ReadFirstLineFromFile(std::string const &path);
std::vector<std::string> GetSubdirectories(std::string const &path);

} // namespace ppp::fs
