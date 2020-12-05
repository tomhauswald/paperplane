#include "db.hh"
#include "hash.hh"

#include <algorithm>

namespace ppp::db {

Database LoadDatabase(std::string const &path) {
  Database db;
  for (auto const &dir : fs::GetSubdirectories(path)) {
    auto image_hash = hash::ParseSha1Sum(dir.substr(dir.find_last_of('/') + 1));
    auto pdf_hash =
        hash::ParseSha1Sum(fs::ReadFirstLineFromFile(dir + "/PDF_SHA1"));
    auto tokens = fs::ReadLinesFromFile(dir + "/TOKENS");
    db.documents.push_back({image_hash, pdf_hash, tokens});
  }
  return db;
}

void DumpDatabase(Database const &db) {
  for (auto const &doc : db.documents) {
    auto image_hash = hash::Sha1SumToString(doc.image_hash);
    auto pdf_hash = hash::Sha1SumToString(doc.pdf_hash);
    printf("+-- Image Hash %s\n", image_hash.c_str());
    printf("|    |\n");
    printf("|    +-- PDF Hash %s\n", pdf_hash.c_str());
    printf("|    +-- Unique Token Count: %zu\n", doc.tokens.size());
    printf("|\n");
  }
}

void DumpQuery(Query const &query) {
  printf("Query: '");
  for (auto const &token : query.tokens)
    printf("%s ", token.c_str());
  printf("'\n");
}

float ComputeDocumentMatch(Document const &doc, Query const &query) {
  auto score = 0.0f;
  for (auto const &dtok : doc.tokens) {
    for (auto const &qtok : query.tokens) {
      if (dtok.find(qtok) != std::string::npos) {
        score += static_cast<float>(qtok.size());
      }
    }
  }
  return score;
}

std::pair<Document const *, float> FindBestMatch(Database const &db,
                                                 Query const &query) {
  float max_score = -1.0f;
  Document const *max_doc = nullptr;

  for (auto const &doc : db.documents) {
    auto score = ComputeDocumentMatch(doc, query);
    if (score > max_score) {
      max_score = score;
      max_doc = &doc;
    }
  }

  return {max_doc, max_score};
}

} // namespace ppp::db
