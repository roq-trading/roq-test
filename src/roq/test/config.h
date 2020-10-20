/* Copyright (c) 2017-2020, Hans Erik Thrane */

#pragma once

#include "roq/client.h"

namespace roq {
namespace test {

class Config final : public client::Config {
 public:
  Config() {}

  Config(const Config &) = delete;
  Config(Config &&) = default;

 protected:
  void dispatch(Handler &handler) const override;
};

}  // namespace test
}  // namespace roq
