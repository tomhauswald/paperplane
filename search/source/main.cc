#include <cassert>
#include <cstdio>
#include <unordered_map>

#include "db.hh"
#include "fs.hh"
#include "hash.hh"

/*
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
} */

int main(int argc, char const **argv) {

  if (argc < 3) {
    printf("Usage: ./search /path/to/db queryToken1 ... queryTokenN\n");
    return 1;
  }

  auto db = ppp::db::LoadDatabase(argv[1]);
  ppp::db::DumpDatabase(db);

  /*
  std::vector<std::string> query(argc - 2);
  for (size_t i = 2; i < static_cast<size_t>(argc); ++i)
    query[i - 2] = std::string(argv[i]);

  printf("Query:");
  for (auto const &token : query)
    printf(" %s", token.c_str());
  printf("\n");

  for (auto const &[image_sha1, score] : ScoreDatabaseWrtQuery(db, query)) {
    printf("%s: %f\n", image_sha1.c_str(), score);
  }
  */

  return 0;
}
