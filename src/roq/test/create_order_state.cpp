/* Copyright (c) 2017-2020, Hans Erik Thrane */

#include "roq/test/create_order_state.h"

#include <cassert>
#include <memory>

#include "roq/logging.h"

#include "roq/test/strategy.h"
#include "roq/test/working_order_state.h"

namespace roq {
namespace test {

CreateOrderState::CreateOrderState(Strategy &strategy)
    : State(strategy), order_id_(strategy_.create_order()) {
}

void CreateOrderState::operator()(std::chrono::nanoseconds) {
  // TODO(thraneh): check timeout
}

void CreateOrderState::operator()(const OrderAck &order_ack) {
  LOG_IF(FATAL, order_ack.type != RequestType::CREATE_ORDER)("Unexpected");
  switch (order_ack.origin) {
    case Origin::GATEWAY:
      switch (order_ack.status) {
        case RequestStatus::FORWARDED: gateway_ack_ = true; break;
        default: LOG(FATAL)("Unexpected request status"); break;
      }
      break;
    case Origin::EXCHANGE:
      switch (order_ack.status) {
        case RequestStatus::ACCEPTED:
          if (gateway_ack_ == false) LOG(FATAL)("Unexpected request status");
          exchange_ack_ = true;
          break;
        default: LOG(FATAL)("Unexpected request status"); break;
      }
    default: break;
  }
}

void CreateOrderState::operator()(const OrderUpdate &order_update) {
  LOG_IF(WARNING, order_update.order_id != order_id_)("Unexpected");
  LOG_IF(FATAL, exchange_ack_ == false)("Unexpected");
  if (roq::is_order_complete(order_update.status)) {
    strategy_.stop();
  } else {
    strategy_(std::make_unique<WorkingOrderState>(strategy_, order_id_));
  }
}

}  // namespace test
}  // namespace roq
