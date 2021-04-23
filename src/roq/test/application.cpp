/* Copyright (c) 2017-2021, Hans Erik Thrane */

#include "roq/test/application.h"

#include <vector>

#include "roq/exceptions.h"

#include "roq/test/config.h"
#include "roq/test/strategy.h"

using namespace roq::literals;

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
  assert(!args.empty());
  if (args.size() == 1)
    throw RuntimeErrorException("Expected arguments"_sv);
  if (args.size() != 2)
    throw RuntimeErrorException("Expected exactly one argument"_sv);
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
