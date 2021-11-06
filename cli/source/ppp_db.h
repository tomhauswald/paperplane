#pragma once

#include "ppp_fs.h"

#include <optional>
#include <unordered_map>

namespace ppp::db {

struct Database;

struct Document {
  std::string image_hash;
  std::string pdf_hash;
  std::unordered_map<std::string, size_t> token_occurrences;
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

std::string GetDocumentPath(Database const &db, Document const &doc);
size_t GetTokenOccurrences(Document const &doc, std::string const &token);

Query CreateQuery(std::vector<std::string> const &words);

void DumpDatabase(Database const &db);
void DumpQuery(Query const &query);

float ComputeScore(Document const &doc, Query const &query);
std::optional<Match> FindBestMatch(Database const &db, Query const &query);

} // namespace ppp::db
