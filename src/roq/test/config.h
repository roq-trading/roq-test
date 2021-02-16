/* Copyright (c) 2017-2021, Hans Erik Thrane */

#pragma once

#include "roq/client.h"

namespace roq {
namespace test {

class Config final : public client::Config {
 public:
  Config() {}

  Config(Config &&) = default;
  Config(const Config &) = delete;

 protected:
  void dispatch(Handler &handler) const override;
};

}  // namespace test
}  // namespace roq
