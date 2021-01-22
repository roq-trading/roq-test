/* Copyright (c) 2017-2020, Hans Erik Thrane */

#include "roq/test/working_order_state.h"

#include <absl/flags/flag.h>

#include <memory>

#include "roq/logging.h"

#include "roq/test/cancel_order_state.h"
#include "roq/test/modify_order_state.h"
#include "roq/test/options.h"
#include "roq/test/strategy.h"

namespace roq {
namespace test {

WorkingOrderState::WorkingOrderState(Strategy &strategy, uint32_t order_id)
    : State(strategy), order_id_(order_id) {
}

void WorkingOrderState::operator()(std::chrono::nanoseconds now) {
  if (next_state_transition_.count() == 0) {
    next_state_transition_ =
        now + std::chrono::seconds{absl::GetFlag(FLAGS_wait_time_secs)};
  } else if (next_state_transition_ < now) {
    if (absl::GetFlag(FLAGS_enable_modify_order))
      strategy_(std::make_unique<ModifyOrderState>(strategy_, order_id_));
    else
      strategy_(std::make_unique<CancelOrderState>(strategy_, order_id_));
  }
}

void WorkingOrderState::operator()(const OrderAck &) {
  LOG(FATAL)("Unexpected");
}

void WorkingOrderState::operator()(const OrderUpdate &order_update) {
  LOG_IF(WARNING, order_update.order_id != order_id_)("Unexpected");
  if (roq::is_order_complete(order_update.status))
    strategy_.stop();
}

}  // namespace test
}  // namespace roq
