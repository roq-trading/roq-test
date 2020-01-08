/* Copyright (c) 2017-2020, Hans Erik Thrane */

#include "roq/client.h"

namespace roq {
namespace test {

class Config final : public client::Config {
 public:
  Config() {}

  Config(const Config&) = delete;
  Config(Config&&) = default;

  void operator=(const Config&) = delete;
  void operator=(Config&&) = delete;

 protected:
  void dispatch(Handler& handler) const override;
};

}  // namespace test
}  // namespace roq
