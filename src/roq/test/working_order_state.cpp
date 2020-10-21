/* Copyright (c) 2017-2020, Hans Erik Thrane */

#include "roq/test/working_order_state.h"

#include <memory>

#include "roq/logging.h"

#include "roq/test/cancel_order_state.h"
#include "roq/test/modify_order_state.h"
#include "roq/test/options.h"
#include "roq/test/strategy.h"

namespace roq {
namespace test {

WorkingOrderState::WorkingOrderState(Strategy &strategy, uint32_t order_id)
    : State(strategy), _order_id(order_id) {
}

void WorkingOrderState::operator()(std::chrono::nanoseconds now) {
  if (_next_state_transition.count() == 0) {
    _next_state_transition = now + std::chrono::seconds{FLAGS_wait_time_secs};
  } else if (_next_state_transition < now) {
    if (FLAGS_enable_modify_order)
      _strategy(std::make_unique<ModifyOrderState>(_strategy, _order_id));
    else
      _strategy(std::make_unique<CancelOrderState>(_strategy, _order_id));
  }
}

void WorkingOrderState::operator()(const OrderAck &) {
  LOG(FATAL)("Unexpected");
}

void WorkingOrderState::operator()(const OrderUpdate &order_update) {
  LOG_IF(WARNING, order_update.order_id != _order_id)("Unexpected");
  if (roq::is_order_complete(order_update.status)) _strategy.stop();
}

}  // namespace test
}  // namespace roq
