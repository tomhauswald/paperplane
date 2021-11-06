#include <cstdio>

#include "ppp_db.h"
#include "ppp_fs.h"

namespace ppp {

struct Context {
  std::string db_path;
  std::string operation;
  std::optional<ppp::db::Query> query;
};

void PrintUsage() {
  printf("Usage:\n");
  printf(" (Dump Database) ./pppcli /path/to/db dump\n");
  printf(" (Run Query)     ./pppcli /path/to/db query tokens...\n");
}

std::optional<Context> ParseContext(int argc, char const **argv) {
  if (argc < 3)
    return std::nullopt;

  Context ctx{};

  ctx.db_path = argv[1];
  if (!ppp::fs::DirectoryExists(ctx.db_path)) {
    printf("Error: Directory %s does not exist.\n", ctx.db_path.c_str());
    return std::nullopt;
  }

  ctx.operation = argv[2];
  if (std::string("dump query").find(ctx.operation) == std::string::npos) {
    printf("Error: Unknown operation %s.\n", ctx.operation.c_str());
    return std::nullopt;
  }

  if (ctx.operation == "dump" && argc > 3) {
    printf("Error: Dump operation does not take arguments.\n");
    return std::nullopt;
  }

  if (ctx.operation == "query") {
    if (argc < 4) {
      printf("Error: Query operation takes at least one argument.\n");
      return std::nullopt;
    }

    std::vector<std::string> words;
    for (int arg = 3; arg < argc; ++arg)
      words.emplace_back(argv[arg]);
    ctx.query = ppp::db::CreateQuery(words);
  }

  return ctx;
}

} // namespace ppp

int main(int argc, char const **argv) {

  auto ctx = ppp::ParseContext(argc, argv);
  if (!ctx) {
    ppp::PrintUsage();
    return 1;
  }

  auto db = ppp::db::LoadDatabase(ctx->db_path);

  if (ctx->operation == "dump")
    ppp::db::DumpDatabase(db);

  else if (ctx->operation == "query") {
    ppp::db::DumpQuery(*ctx->query);
    auto match = ppp::db::FindBestMatch(db, *ctx->query);
    if (!match || match->score < 1) {
      printf("No matching document(s) found.\n");
    } else {
      printf("Best match is %s with score %0.2f.\n",
             ppp::db::GetDocumentPath(db, match->document).c_str(),
             match->score);
    }
  }

  return 0;
}
