/* Copyright (c) 2017-2021, Hans Erik Thrane */

#include "roq/test/application.h"

#include "roq/api.h"

using namespace std::literals;  // NOLINT

namespace {
static const auto DESCRIPTION = "Roq Test"sv;
}  // namespace

int main(int argc, char **argv) {
  return roq::test::Application(argc, argv, DESCRIPTION, ROQ_VERSION).run();
}
