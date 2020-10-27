/* Copyright (c) 2017-2020, Hans Erik Thrane */

#include "roq/test/wait_market_ready_state.h"

#include <memory>

#include "roq/logging.h"

#include "roq/test/create_order_state.h"
#include "roq/test/strategy.h"

namespace roq {
namespace test {

void WaitMarketReadyState::operator()(std::chrono::nanoseconds) {
  if (strategy_.ready()) {
    strategy_(std::make_unique<CreateOrderState>(strategy_));
  }
}

void WaitMarketReadyState::operator()(const OrderAck &) {
  LOG(FATAL)("Unexpected");
}

void WaitMarketReadyState::operator()(const OrderUpdate &) {
  // note! could be download
}

}  // namespace test
}  // namespace roq
