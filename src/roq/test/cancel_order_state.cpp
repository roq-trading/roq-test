/* Copyright (c) 2017-2020, Hans Erik Thrane */

#include "roq/test/cancel_order_state.h"

#include <memory>

#include "roq/logging.h"

#include "roq/test/strategy.h"

namespace roq {
namespace test {

CancelOrderState::CancelOrderState(Strategy &strategy, uint32_t order_id)
    : State(strategy), _order_id(order_id) {
  _strategy.cancel_order(order_id);
}

void CancelOrderState::operator()(std::chrono::nanoseconds) {
  // TODO(thraneh): check timeout
}

void CancelOrderState::operator()(const OrderAck &order_ack) {
  LOG_IF(FATAL, order_ack.type != RequestType::CANCEL_ORDER)("Unexpected");
  switch (order_ack.origin) {
    case Origin::GATEWAY:
      switch (order_ack.status) {
        case RequestStatus::FORWARDED:
          _gateway_ack = true;
          break;
        default:
          LOG(FATAL)("Unexpected request status");
          break;
      }
      break;
    case Origin::EXCHANGE:
      switch (order_ack.status) {
        case RequestStatus::ACCEPTED:
          if (_gateway_ack == false) LOG(FATAL)("Unexpected request status");
          _exchange_ack = true;
          break;
        default:
          LOG(FATAL)("Unexpected request status");
          break;
      }
    default:
      break;
  }
}

void CancelOrderState::operator()(const OrderUpdate &order_update) {
  LOG_IF(WARNING, order_update.order_id != _order_id)("Unexpected");
  LOG_IF(FATAL, _exchange_ack == false)("Unexpected");
  if (roq::is_order_complete(order_update.status)) _strategy.stop();
}

}  // namespace test
}  // namespace roq
