/* Copyright (c) 2017-2021, Hans Erik Thrane */

#include "roq/test/working_order_state.h"

#include <memory>

#include "roq/logging.h"

#include "roq/utils/common.h"

#include "roq/test/cancel_order_state.h"
#include "roq/test/flags.h"
#include "roq/test/modify_order_state.h"
#include "roq/test/strategy.h"

using namespace roq::literals;

namespace roq {
namespace test {

WorkingOrderState::WorkingOrderState(Strategy &strategy, uint32_t order_id)
    : State(strategy), order_id_(order_id) {
}

void WorkingOrderState::operator()(std::chrono::nanoseconds now) {
  if (next_state_transition_ == next_state_transition_.zero()) {
    next_state_transition_ = now + std::chrono::seconds{Flags::wait_time_secs()};
  } else if (next_state_transition_ < now) {
    if (Flags::enable_modify_order())
      strategy_(std::make_unique<ModifyOrderState>(strategy_, order_id_));
    else
      strategy_(std::make_unique<CancelOrderState>(strategy_, order_id_));
  }
}

void WorkingOrderState::operator()(const OrderAck &) {
  log::fatal("Unexpected"_sv);
}

void WorkingOrderState::operator()(const OrderUpdate &order_update) {
  if (order_update.order_id != order_id_)
    log::fatal("Unexpected"_sv);
  if (utils::is_order_complete(order_update.status))
    strategy_.stop();
}

}  // namespace test
}  // namespace roq
