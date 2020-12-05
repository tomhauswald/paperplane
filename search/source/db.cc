#include "db.hh"
#include "hash.hh"

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

} // namespace ppp::db
