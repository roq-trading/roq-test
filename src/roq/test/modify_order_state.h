/* Copyright (c) 2017-2020, Hans Erik Thrane */

#pragma once

#include "roq/test/state.h"

namespace roq {
namespace test {

class ModifyOrderState final : public State {
 public:
  ModifyOrderState(Strategy &strategy, uint32_t order_id);

  void operator()(std::chrono::nanoseconds) override;
  void operator()(const OrderAck &) override;
  void operator()(const OrderUpdate &) override;

 protected:
  void check();

 private:
  const uint32_t order_id_;
  bool gateway_ack_ = false;
  bool exchange_ack_ = false;
  bool order_update_ = false;
  bool done_ = false;
};

}  // namespace test
}  // namespace roq
