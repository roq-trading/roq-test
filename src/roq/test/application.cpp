/* Copyright (c) 2017-2021, Hans Erik Thrane */

#include "roq/test/application.h"

#include <vector>

#include "roq/test/config.h"
#include "roq/test/strategy.h"

using namespace std::literals;  // NOLINT

namespace roq {
namespace test {

int Application::main(int argc, char **argv) {
  // wrap arguments (prefer to not work with raw pointers)
  std::vector<std::string_view> args;
  args.reserve(argc);
  for (int i = 0; i < argc; ++i)
    args.emplace_back(argv[i]);
  return main_helper(args);
}

int Application::main_helper(const roq::span<std::string_view> &args) {
  assert(args.empty() == false);
  if (args.size() == 1)
    throw std::runtime_error("Expected arguments"s);
  if (args.size() != 2)
    throw std::runtime_error("Expected exactly one argument"s);
  Config config;
  // note!
  //   absl::flags will have removed all flags and we're left with arguments
  //   arguments should be a list of unix domain sockets
  auto connections = args.subspan(1);
  client::Trader(config, connections).dispatch<Strategy>();
  return EXIT_SUCCESS;
}

}  // namespace test
}  // namespace roq
