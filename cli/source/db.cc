#include "db.hh"

#include <algorithm>

namespace ppp::db {

Database LoadDatabase(std::string const &path) {
  auto db = Database{.path = path};

  for (auto const &dir : fs::GetSubdirectories(path)) {
    db.documents.push_back(Document{
        .id = dir.substr(dir.find_last_of('/') + 1),
        .image_checksum = fs::ReadFirstLineFromFile(dir + "/image-checksum"),
        .pdf_checksum = fs::ReadFirstLineFromFile(dir + "/pdf-checksum"),
        .tokens = fs::ReadLinesFromFile(dir + "/tokens")});
  }

  return db;
}

void DumpDatabase(Database const &db) {
  printf("Database %s\n", db.path.c_str());
  printf(" |\n");
  for (auto const &doc : db.documents) {
    auto doc_path = db.path + "/" + doc.id;

    printf(" +-- Document %s\n", doc.id.c_str());
    printf(" |    |\n");
    printf(" |    +-- PDF Path           : %s\n", (doc_path + "/pdf").c_str());
    printf(" |    +-- PDF Checksum       : %s\n", doc.pdf_checksum.c_str());
    printf(" |    +-- Image Path         : %s\n",
           (doc_path + "/image").c_str());
    printf(" |    +-- Image Checksum     : %s\n", doc.image_checksum.c_str());
    printf(" |    +-- Unique Token Count : %zu\n", doc.tokens.size());
    printf(" |\n");
  }
}

void DumpQuery(Query const &query) {
  printf("Query:");
  for (auto const &token : query.tokens)
    printf(" %s ", token.c_str());
  printf("\n");
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

std::optional<Match> FindBestMatch(Database const &db, Query const &query) {
  if (db.documents.empty())
    return std::nullopt;

  float max_score = -1.0f;
  Document const *max_doc = nullptr;

  for (auto const &doc : db.documents) {
    auto score = ComputeDocumentMatch(doc, query);
    if (score > max_score) {
      max_score = score;
      max_doc = &doc;
    }
  }

  return Match{.document = *max_doc, .score = max_score};
}

} // namespace ppp::db
