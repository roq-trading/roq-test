/* Copyright (c) 2017-2020, Hans Erik Thrane */

#include "roq/test/working_order_state_2.h"

#include <memory>

#include "roq/logging.h"

#include "roq/test/cancel_order_state.h"
#include "roq/test/options.h"
#include "roq/test/strategy.h"

namespace roq {
namespace test {

WorkingOrderState2::WorkingOrderState2(
    Strategy& strategy,
    uint32_t order_id)
    : State(strategy),
      _order_id(order_id) {
}

void WorkingOrderState2::operator()(std::chrono::nanoseconds now) {
  if (_next_state_transition.count() == 0) {
    _next_state_transition = now +
      std::chrono::seconds { FLAGS_wait_time_secs };
  } else if (_next_state_transition < now) {
    _strategy(
        std::make_unique<CancelOrderState>(
            _strategy,
            _order_id));
  }
}

void WorkingOrderState2::operator()(const OrderAck&) {
  LOG(FATAL)("Unexpected");
}

void WorkingOrderState2::operator()(const OrderUpdate& order_update) {
  LOG_IF(WARNING, order_update.order_id != _order_id)("Unexpected");
  if (roq::is_order_complete(order_update.status))
    _strategy.stop();
}

}  // namespace test
}  // namespace roq
