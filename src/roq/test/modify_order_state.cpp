/* Copyright (c) 2017-2021, Hans Erik Thrane */

#include "roq/test/modify_order_state.h"

#include <memory>

#include "roq/logging.h"

#include "roq/test/strategy.h"
#include "roq/test/working_order_state_2.h"

using namespace roq::literals;

namespace roq {
namespace test {

ModifyOrderState::ModifyOrderState(Strategy &strategy, uint32_t order_id)
    : State(strategy), order_id_(order_id) {
  strategy_.modify_order(order_id);
}

void ModifyOrderState::operator()(std::chrono::nanoseconds) {
  // TODO(thraneh): check timeout
}

void ModifyOrderState::operator()(const OrderAck &order_ack) {
  LOG_IF(FATAL, order_ack.type != RequestType::MODIFY_ORDER)("Unexpected"_sv);
  switch (order_ack.origin) {
    case Origin::GATEWAY:
      switch (order_ack.status) {
        case RequestStatus::FORWARDED:
          LOG_IF(FATAL, exchange_ack_)("Unexpected"_sv);
          gateway_ack_ = true;
          break;
        default:
          LOG(FATAL)("Unexpected request status"_sv);
          break;
      }
      break;
    case Origin::EXCHANGE:
      switch (order_ack.status) {
        case RequestStatus::ACCEPTED:
          LOG_IF(FATAL, !gateway_ack_)("Unexpected request status"_sv);
          exchange_ack_ = true;
          break;
        default:
          LOG(FATAL)("Unexpected request status"_sv);
          break;
      }
    default:
      break;
  }
  check();
}

void ModifyOrderState::operator()(const OrderUpdate &order_update) {
  LOG_IF(WARNING, order_update.order_id != order_id_)("Unexpected"_sv);
  if (roq::is_order_complete(order_update.status)) {
    strategy_.stop();
  } else {
    order_update_ = true;
    check();
  }
}

void ModifyOrderState::check() {
  if (gateway_ack_ && exchange_ack_ && order_update_) {
    strategy_(std::make_unique<WorkingOrderState2>(strategy_, order_id_));
  }
}

}  // namespace test
}  // namespace roq
