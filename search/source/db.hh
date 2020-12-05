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

struct Query {
  std::vector<std::string> tokens;
};

Database LoadDatabase(std::string const &path);
void DumpDatabase(Database const &db);
void DumpQuery(Query const &query);
float ComputeDocumentMatch(Document const &doc, Query const &query);
std::pair<Document const *, float> FindBestMatch(Database const &db,
                                                 Query const &query);

} // namespace ppp::db
