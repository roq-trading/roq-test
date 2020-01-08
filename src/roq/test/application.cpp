/* Copyright (c) 2017-2020, Hans Erik Thrane */

#include "roq/test/application.h"

#include <string>
#include <vector>

#include "roq/test/config.h"
#include "roq/test/strategy.h"

namespace roq {
namespace test {

int Application::main(int argc, char **argv) {
  if (argc == 1)
    throw std::runtime_error("Expected exactly one argument");
  Config config;
  std::vector<std::string> connections(
      argv + 1,
      argv + argc);
  client::Trader(config, connections).dispatch<Strategy>();
  return EXIT_SUCCESS;
}

}  // namespace test
}  // namespace roq
