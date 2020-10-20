/* Copyright (c) 2017-2020, Hans Erik Thrane */

#include "roq/test/modify_order_state.h"

#include <memory>

#include "roq/logging.h"

#include "roq/test/strategy.h"
#include "roq/test/working_order_state_2.h"

namespace roq {
namespace test {

ModifyOrderState::ModifyOrderState(Strategy &strategy, uint32_t order_id)
    : State(strategy), _order_id(order_id) {
  _strategy.modify_order(order_id);
}

void ModifyOrderState::operator()(std::chrono::nanoseconds) {
  // TODO(thraneh): check timeout
}

void ModifyOrderState::operator()(const OrderAck &order_ack) {
  LOG_IF(FATAL, order_ack.type != RequestType::MODIFY_ORDER)("Unexpected");
  switch (order_ack.origin) {
    case Origin::GATEWAY:
      switch (order_ack.status) {
        case RequestStatus::FORWARDED:
          LOG_IF(FATAL, _exchange_ack)("Unexpected");
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
          LOG_IF(FATAL, _gateway_ack == false)("Unexpected request status");
          _exchange_ack = true;
          break;
        default:
          LOG(FATAL)("Unexpected request status");
          break;
      }
    default:
      break;
  }
  check();
}

void ModifyOrderState::operator()(const OrderUpdate &order_update) {
  LOG_IF(WARNING, order_update.order_id != _order_id)("Unexpected");
  if (roq::is_order_complete(order_update.status)) {
    _strategy.stop();
  } else {
    _order_update = true;
    check();
  }
}

void ModifyOrderState::check() {
  if (_gateway_ack && _exchange_ack && _order_update) {
    _strategy(std::make_unique<WorkingOrderState2>(_strategy, _order_id));
  }
}

}  // namespace test
}  // namespace roq
