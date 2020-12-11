#pragma once

#include "fs.hh"
#include <optional>

namespace ppp::db {

struct Document {
  std::string id;
  std::string image_checksum;
  std::string pdf_checksum;
  std::vector<std::string> tokens;
};

struct Database {
  std::string path;
  std::vector<Document> documents;
};

struct Query {
  std::vector<std::string> tokens;
};

struct Match {
  Document const &document;
  float score;
};

Database LoadDatabase(std::string const &path);
void DumpDatabase(Database const &db);
void DumpQuery(Query const &query);
float ComputeDocumentMatch(Document const &doc, Query const &query);
std::optional<Match> FindBestMatch(Database const &db, Query const &query);

} // namespace ppp::db
