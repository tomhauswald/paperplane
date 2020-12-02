#include <cassert>
#include <cstdio>
#include <filesystem>
#include <fstream>
#include <string>
#include <unordered_map>
#include <vector>

std::vector<std::string> GetFileLines(char const *path) {
  std::vector<std::string> lines;
  std::ifstream file(path);
  std::string line;
  while (getline(file, line))
    lines.push_back(line);
  return lines;
}

struct Node {
  std::string pdf_sha1;
  std::vector<std::string> tokens;
};

using Database = std::unordered_map<std::string, Node>;

Database LoadDatabase(char const *path) {
  std::unordered_map<std::string, Node> db;
  for (auto const &entry : std::filesystem::directory_iterator(path)) {
    auto path = entry.path().string();
    db[GetFileLines((path + "/IMAGE_SHA1").c_str()).at(0)] = {
        GetFileLines((path + "/PDF_SHA1").c_str()).at(0),
        GetFileLines((path + "/TOKENS").c_str())};
  }
  return db;
}

void DumpDatabase(Database const &db) {
  for (auto const &[image_sha1, node] : db) {
    printf("+-- [Image] %s\n", image_sha1.c_str());
    printf("|    |\n");
    printf("|    +-- [PDF] %s\n", node.pdf_sha1.c_str());
    printf("|    +-- Unique Token Count: %zu\n", node.tokens.size());
    printf("|\n");
  }
}

float ScoreNodeWrtQuery(Node const &node,
                        std::vector<std::string> const &query) {
  auto score = 0.0f;
  for (auto const &lhs : query) {
    for (auto const &rhs : node.tokens) {
      if (lhs == rhs) {
        score += static_cast<float>(lhs.size());
      }
    }
  }
  return score;
}

std::unordered_map<std::string, float>
ScoreDatabaseWrtQuery(Database const &db,
                      std::vector<std::string> const &query) {
  std::unordered_map<std::string, float> scores;
  for (auto const &[image_sha1, node] : db) {
    scores[image_sha1] = ScoreNodeWrtQuery(node, query);
  }
  return scores;
}

int main(int argc, char const **argv) {
  assert(argc >= 3);
  auto db = LoadDatabase(argv[1]);

  std::vector<std::string> query(argc - 2);
  for (size_t i = 2; i < argc; ++i)
    query[i - 2] = std::string(argv[i]);

  printf("Query:");
  for (auto const &token : query)
    printf(" %s", token.c_str());
  printf("\n");

  for (auto const &[image_sha1, score] : ScoreDatabaseWrtQuery(db, query)) {
    printf("%s: %f\n", image_sha1.c_str(), score);
  }

  return 0;
}
