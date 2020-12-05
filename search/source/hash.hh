#pragma once

#include <cstdint>
#include <string>

namespace ppp::hash {

struct Sha1Sum {
  uint8_t bytes[20];
};

Sha1Sum ParseSha1Sum(std::string const &str);
std::string Sha1SumToString(Sha1Sum const &sha);
int CompareSha1Sums(Sha1Sum const &lhs, Sha1Sum const &rhs);

} // namespace ppp::hash
