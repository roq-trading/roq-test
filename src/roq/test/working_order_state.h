/* Copyright (c) 2017-2021, Hans Erik Thrane */

#pragma once

#include "roq/test/state.h"

namespace roq {
namespace test {

class WorkingOrderState final : public State {
 public:
  WorkingOrderState(Strategy &, uint32_t order_id);

  void operator()(std::chrono::nanoseconds) override;
  void operator()(const OrderAck &) override;
  void operator()(const OrderUpdate &) override;

 private:
  const uint32_t order_id_;
  std::chrono::nanoseconds next_state_transition_ = {};
};

}  // namespace test
}  // namespace roq
