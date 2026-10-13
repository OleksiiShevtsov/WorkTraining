#include "engine/server_engine.h"

#include <cstring>
#include <iostream>

namespace {

constexpr std::uint16_t kPort = 54545;

void print_usage(const char* argv0) {
  std::cout << "Usage: " << argv0 << " [--smoke]\n"
            << "  (default) long-running server loop\n"
            << "  --smoke   self-test then exit\n";
}

}  // namespace

int main(int argc, char** argv) {
  bool smoke = false;
  for (int i = 1; i < argc; ++i) {
    if (std::strcmp(argv[i], "--smoke") == 0) {
      smoke = true;
    } else if (std::strcmp(argv[i], "--help") == 0 || std::strcmp(argv[i], "-h") == 0) {
      print_usage(argv[0]);
      return 0;
    } else {
      std::cerr << "Unknown argument: " << argv[i] << '\n';
      print_usage(argv[0]);
      return 1;
    }
  }

  botf::ServerEngine engine;
  return smoke ? engine.run_smoke(kPort) : engine.run(kPort);
}
