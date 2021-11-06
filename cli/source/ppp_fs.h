#pragma once

#include "ppp_assert.h"

#include <string>
#include <vector>

namespace ppp::fs {

bool FileExists(std::string const &path);
bool DirectoryExists(std::string const &path);

std::string GetBasename(std::string const &path);
std::string ToAbsolutePath(std::string const &path);

std::vector<std::string> GetSubdirectories(std::string const &path);

std::vector<std::string> ReadLinesFromFile(std::string const &path);
std::string ReadFirstLineFromFile(std::string const &path);

} // namespace ppp::fs
