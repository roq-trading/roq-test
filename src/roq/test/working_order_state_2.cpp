/* Copyright (c) 2017-2020, Hans Erik Thrane */

#include "roq/test/working_order_state_2.h"

#include <memory>

#include "roq/logging.h"

#include "roq/test/cancel_order_state.h"
#include "roq/test/options.h"
#include "roq/test/strategy.h"

namespace roq {
namespace test {

WorkingOrderState2::WorkingOrderState2(Strategy &strategy, uint32_t order_id)
    : State(strategy), order_id_(order_id) {
}

void WorkingOrderState2::operator()(std::chrono::nanoseconds now) {
  if (next_state_transition_.count() == 0) {
    next_state_transition_ = now + std::chrono::seconds{FLAGS_wait_time_secs};
  } else if (next_state_transition_ < now) {
    strategy_(std::make_unique<CancelOrderState>(strategy_, order_id_));
  }
}

void WorkingOrderState2::operator()(const OrderAck &) {
  LOG(FATAL)("Unexpected");
}

void WorkingOrderState2::operator()(const OrderUpdate &order_update) {
  LOG_IF(WARNING, order_update.order_id != order_id_)("Unexpected");
  if (roq::is_order_complete(order_update.status))
    strategy_.stop();
}

}  // namespace test
}  // namespace roq
