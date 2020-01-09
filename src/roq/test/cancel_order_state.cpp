/* Copyright (c) 2017-2020, Hans Erik Thrane */

#include "roq/test/cancel_order_state.h"

#include <cassert>
#include <memory>

#include "roq/logging.h"

#include "roq/test/strategy.h"

namespace roq {
namespace test {

CancelOrderState::CancelOrderState(
    Strategy& strategy,
    uint32_t order_id)
    : State(strategy),
      _order_id(order_id) {
  _strategy.cancel_order(order_id);
}

void CancelOrderState::operator()(std::chrono::nanoseconds) {
}

void CancelOrderState::operator()(const OrderAck& order_ack) {
  assert(order_ack.type == RequestType::CANCEL_ORDER);
  switch (order_ack.origin) {
    case Origin::CLIENT:
      switch (order_ack.status) {
        case RequestStatus::FORWARDED:
          break;
        case RequestStatus::UNDEFINED:
        case RequestStatus::ACCEPTED:
        case RequestStatus::REJECTED:
        case RequestStatus::TIMEOUT:
          LOG(FATAL)("Unexpected request status");
          break;
      }
    case Origin::GATEWAY:
      switch (order_ack.status) {
        case RequestStatus::FORWARDED:
          _gateway_ack = true;
          break;
        case RequestStatus::UNDEFINED:
        case RequestStatus::ACCEPTED:
        case RequestStatus::REJECTED:
        case RequestStatus::TIMEOUT:
          LOG(FATAL)("Unexpected request status");
          break;
      }
      break;
    case Origin::BROKER:
      // note! not relevant to this test
      break;
    case Origin::EXCHANGE:
      switch (order_ack.status) {
        case RequestStatus::ACCEPTED:
          if (_gateway_ack == false)
            LOG(FATAL)("Unexpected request status");
          _exchange_ack = true;
          break;
        case RequestStatus::UNDEFINED:
        case RequestStatus::FORWARDED:
        case RequestStatus::REJECTED:
        case RequestStatus::TIMEOUT:
          LOG(FATAL)("Unexpected request status");
          break;
      }
  }
}

void CancelOrderState::operator()(const OrderUpdate& order_update) {
  assert(_exchange_ack == true);
  if (order_update.status == OrderStatus::CANCELED)
    _strategy.stop();
}

}  // namespace test
}  // namespace roq
