/* Copyright (c) 2017-2020, Hans Erik Thrane */

#pragma once

#include "roq/test/state.h"

namespace roq {
namespace test {

class WorkingOrderState final : public State {
 public:
  WorkingOrderState(Strategy &strategy, uint32_t order_id);

  void operator()(std::chrono::nanoseconds) override;
  void operator()(const OrderAck &) override;
  void operator()(const OrderUpdate &) override;

 private:
  const uint32_t _order_id;
  std::chrono::nanoseconds _next_state_transition = {};
};

}  // namespace test
}  // namespace roq
