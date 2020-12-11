#include <cstdio>

#include "db.hh"
#include "fs.hh"

struct Context {
  std::string db_path;
  std::string operation;
  std::optional<ppp::db::Query> query;
};

void PrintUsage() {
  printf("Usage:\n");
  printf(" (Dump Database) ./paperplane /path/to/db dump\n");
  printf(" (Run Query)     ./paperplane /path/to/db query tokens...\n");
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
    ctx.query = ppp::db::Query{};
    ctx.query->tokens = std::vector<std::string>(argc - 3);
    for (int arg = 3; arg < argc; ++arg)
      ctx.query->tokens[arg - 3] = argv[arg];
  }

  return ctx;
}

int main(int argc, char const **argv) {
  auto ctx = ParseContext(argc, argv);
  if (!ctx) {
    PrintUsage();
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
      printf("Best matching document is %s with score %f.\n",
             match->document.id.c_str(), match->score);
      printf("The PDF is located here: %s.\n",
             (db.path + "/" + match->document.id + "/pdf").c_str());
    }
  }

  return 0;
}
