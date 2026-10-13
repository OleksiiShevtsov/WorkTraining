#include "app/client_app.h"

#include <cstdint>
#include <cstring>
#include <iostream>

namespace {

constexpr std::uint16_t kPort = 54545;

void print_usage(const char* argv0) {
  std::cout << "Usage: " << argv0 << " [--smoke]\n"
            << "  (default) open SFML window, Move ↔ WorldState loop\n"
            << "  --smoke   no window; validate WorldState → SceneObject\n";
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

  if (smoke) {
    return botf::app::ClientApp::run_smoke();
  }

  botf::app::ClientApp app;
  return app.run(kPort);
}
