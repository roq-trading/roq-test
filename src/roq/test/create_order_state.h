/* Copyright (c) 2017-2020, Hans Erik Thrane */

#pragma once

#include "roq/test/state.h"

namespace roq {
namespace test {

class CreateOrderState final : public State {
 public:
  explicit CreateOrderState(Strategy&);

  void operator()(std::chrono::nanoseconds) override;
  void operator()(const OrderAck&) override;
  void operator()(const OrderUpdate&) override;

 private:
  const uint32_t _order_id;
  bool _gateway_ack = false;
  bool _exchange_ack = false;
};

}  // namespace test
}  // namespace roq
