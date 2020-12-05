#include <cstdio>

#include "db.hh"
#include "fs.hh"
#include "hash.hh"

std::vector<std::string> GetArguments(int argc, char const **argv) {
  std::vector<std::string> args(argc - 1);
  for (int i = 0; i < argc - 1; ++i)
    args[i] = std::string(argv[i + 1]);
  return args;
}

int main(int argc, char const **argv) {
  auto args = GetArguments(argc, argv);

  if (args.size() < 2) {
    printf("Usage: ./search /path/to/db queryToken1 ... queryTokenN\n");
    return 1;
  }

  ppp::db::Query query;
  query.tokens.resize(args.size() - 1);
  for (size_t i = 0; i < args.size() - 1; ++i)
    query.tokens[i] = args.at(i + 1);
  DumpQuery(query);

  auto db = ppp::db::LoadDatabase(args.at(0));
  if (db.documents.size()) {
    auto [doc, score] = ppp::db::FindBestMatch(db, query);
    printf("Best matching document is %s with score %f.\n",
           ppp::hash::Sha1SumToString(doc->image_hash).c_str(), score);
  }

  return 0;
}
