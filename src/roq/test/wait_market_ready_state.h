/* Copyright (c) 2017-2020, Hans Erik Thrane */

#pragma once

#include "roq/test/state.h"

namespace roq {
namespace test {

class WaitMarketReadyState final : public State {
 public:
  using State::State;

  void operator()(std::chrono::nanoseconds) override;
  void operator()(const OrderAck &) override;
  void operator()(const OrderUpdate &) override;
};

}  // namespace test
}  // namespace roq
