#pragma once

#include "fs.hh"
#include "hash.hh"

namespace ppp::db {

struct Document {
  hash::Sha1Sum image_hash;
  hash::Sha1Sum pdf_hash;
  std::vector<std::string> tokens;
};

struct Database {
  std::vector<Document> documents;
};

Database LoadDatabase(std::string const &path);
void DumpDatabase(Database const& db);

} // namespace ppp::db
