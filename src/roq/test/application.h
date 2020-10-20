/* Copyright (c) 2017-2020, Hans Erik Thrane */

#pragma once

#include <string_view>

#include "roq/service.h"

#include "roq/span.h"

namespace roq {
namespace test {

class Application final : public roq::Service {
 public:
  using Service::Service;

 protected:
  int main(int argc, char **argv) override;

  int main_helper(const roq::span<std::string_view> &args);
};

}  // namespace test
}  // namespace roq
