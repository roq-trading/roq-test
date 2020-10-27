/* Copyright (c) 2017-2020, Hans Erik Thrane */

#pragma once

#include "roq/test/state.h"

namespace roq {
namespace test {

class CancelOrderState final : public State {
 public:
  CancelOrderState(Strategy &strategy, uint32_t order_id);

  void operator()(std::chrono::nanoseconds) override;
  void operator()(const OrderAck &) override;
  void operator()(const OrderUpdate &) override;

 private:
  const uint32_t order_id_;
  bool gateway_ack_ = false;
  bool exchange_ack_ = false;
};

}  // namespace test
}  // namespace roq
