/* Copyright (c) 2017-2020, Hans Erik Thrane */

#include "roq/test/create_order_state.h"

#include <cassert>
#include <memory>

#include "roq/logging.h"

#include "roq/test/strategy.h"
#include "roq/test/working_order_state.h"

namespace roq {
namespace test {

CreateOrderState::CreateOrderState(Strategy& strategy)
    : State(strategy),
      _order_id(_strategy.create_order()) {
}

void CreateOrderState::operator()(std::chrono::nanoseconds) {
  // TODO(thraneh): check timeout
}

void CreateOrderState::operator()(const OrderAck& order_ack) {
  LOG_IF(FATAL, order_ack.type != RequestType::CREATE_ORDER)("Unexpected");
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
          if (_gateway_ack == false)
            LOG(FATAL)("Unexpected request status");
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

void CreateOrderState::operator()(const OrderUpdate&) {
  LOG_IF(FATAL, _exchange_ack == false)("Unexpected");
  _strategy(
      std::make_unique<WorkingOrderState>(
          _strategy,
          _order_id));
}

}  // namespace test
}  // namespace roq
