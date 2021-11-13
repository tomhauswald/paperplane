#include "ppp_db.h"

#include <algorithm>

namespace ppp::db {

std::string TrimWhitespace(std::string const &str) {
  const size_t begin = str.find_first_not_of(' ');
  const size_t end = str.find_last_not_of(' ');
  return str.substr(begin, end - begin + 1);
}

std::vector<std::string> Split(std::string const &str, char sep) {
  std::vector<std::string> parts;
  size_t start = str.find_first_not_of(sep);
  while (start < str.size()) {
    size_t end = str.find(sep, start);
    parts.emplace_back(str.substr(start, end - start));
    start = str.find_first_not_of(sep, end);
  }
  return parts;
}

Database LoadDatabase(std::string const &path) {
  std::vector<Document> documents;

  for (auto const &dir : fs::GetSubdirectories(path)) {

    std::unordered_map<std::string, size_t> token_occurrences;
    for (auto const &token_file_line : fs::ReadLinesFromFile(dir + "/tokens")) {
      const auto parts = Split(TrimWhitespace(token_file_line), ' ');
      ppp_assert(parts.size() == 2);
      token_occurrences.emplace(parts[1], std::stoull(parts[0]));
    }

    documents.emplace_back(
        Document{.image_hash = fs::GetBasename(dir),
                 .pdf_hash = fs::ReadFirstLineFromFile(dir + "/pdf-hash"),
                 .token_occurrences = std::move(token_occurrences)});
  }

  return Database{.path = fs::ToAbsolutePath(path),
                  .documents = std::move(documents)};
}

std::string GetDocumentPath(Database const &db, Document const &doc) {
  return db.path + "/" + doc.image_hash;
}

size_t GetTokenOccurrences(Document const &doc, std::string const &token) {
  const auto it = doc.token_occurrences.find(token);
  return ((it == doc.token_occurrences.end()) ? 0 : it->second);
}

char CharToLower(char c) {
  return (((c >= 'A') && (c <= 'Z')) ? (c - 'A') : (c));
}

std::string StrToLower(std::string const &str) {
  std::string lower;
  std::transform(std::cbegin(str), std::cend(str), std::back_inserter(lower),
                 CharToLower);
  return lower;
}

Query CreateQuery(std::vector<std::string> const &words) {
  Query query;
  std::transform(std::cbegin(words), std::cend(words),
                 std::back_inserter(query.tokens), StrToLower);
  return query;
}

std::vector<std::pair<std::string, size_t>>
GetMostFrequentTokens(Document const &doc, size_t n) {
  std::vector<std::pair<std::string, size_t>> top(n);
  for (auto const &[token, count] : doc.token_occurrences) {
    for (size_t i = 0; i < n; ++i) {
      if (count > top[i].second) {
        for (size_t j = n - 1; j >= i + 1; --j) {
          top[j] = top[j - 1];
        }
        top[i] = {token, count};
        break;
      }
    }
  }
  return top;
}

void DumpDatabase(Database const &db) {
  printf("Database %s\n", db.path.c_str());
  printf(" |\n");
  for (auto const &doc : db.documents) {
    auto path = GetDocumentPath(db, doc);

    printf(" +-- Document (Image Hash = %s)\n", doc.image_hash.c_str());
    printf(" |    |\n");
    printf(" |    +-- PDF Path      : %s\n", (path + "/pdf").c_str());
    printf(" |    +-- PDF Hash      : %s\n", doc.pdf_hash.c_str());
    printf(" |    +-- Image Path    : %s\n", (path + "/image").c_str());
    printf(" |    +-- Unique Tokens : %zu\n", doc.token_occurrences.size());
    printf(" |    +-- Most Frequent\n");
    printf(" |         |\n");
    for (auto const &[token, count] : GetMostFrequentTokens(doc, 5)) {
      printf(" |         +-- %s (%zux)\n", token.c_str(), count);
    }
  }
}

void DumpQuery(Query const &query) {
  printf("Query:");
  for (auto const &token : query.tokens)
    printf(" %s ", token.c_str());
  printf("\n");
}

float ComputeScore(Document const &doc, Query const &query) {
  auto score = 0.0f;
  for (auto const &[dtok, count] : doc.token_occurrences) {
    for (auto const &qtok : query.tokens) {
      if (dtok.find(qtok) != std::string::npos) {
        score += 1.0f * qtok.size() * count;
      }
      if (qtok.find(dtok) != std::string::npos) {
        score += 1.0f * dtok.size() * count;
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
    auto score = ComputeScore(doc, query);
    if (score > max_score) {
      max_score = score;
      max_doc = &doc;
    }
  }

  return Match{.document = *max_doc, .score = max_score};
}

} // namespace ppp::db
