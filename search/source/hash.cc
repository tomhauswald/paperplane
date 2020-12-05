#include "hash.hh"
#include <cassert>
#include <cstring>

namespace ppp::hash {

Sha1Sum ParseSha1Sum(const std::string &str) {
  assert(str.size() == 40);

  Sha1Sum sha;
  for (size_t i = 0; i < 20; ++i)
    sha.bytes[i] = strtoul(str.substr(i * 2, 2).c_str(), nullptr, 16);
  return sha;
}

std::string Sha1SumToString(Sha1Sum const &sha) {
  char cstr[41];
  for (size_t i = 0; i < 20; ++i)
    sprintf(cstr + 2 * i, "%02x", sha.bytes[i]);
  return {cstr};
}

int CompareSha1Sums(Sha1Sum const &lhs, Sha1Sum const &rhs) {
  return memcmp(lhs.bytes, rhs.bytes, 20);
}

} // namespace ppp::hash
