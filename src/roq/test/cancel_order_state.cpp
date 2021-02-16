/* Copyright (c) 2017-2021, Hans Erik Thrane */

#include "roq/test/cancel_order_state.h"

#include "roq/logging.h"

#include "roq/test/strategy.h"

using namespace roq::literals;

namespace roq {
namespace test {

CancelOrderState::CancelOrderState(Strategy &strategy, uint32_t order_id)
    : State(strategy), order_id_(order_id) {
  strategy_.cancel_order(order_id);
}

void CancelOrderState::operator()(std::chrono::nanoseconds) {
  // TODO(thraneh): check timeout
}

void CancelOrderState::operator()(const OrderAck &order_ack) {
  LOG_IF(FATAL, order_ack.type != RequestType::CANCEL_ORDER)("Unexpected"_sv);
  switch (order_ack.origin) {
    case Origin::GATEWAY:
      switch (order_ack.status) {
        case RequestStatus::FORWARDED:
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
          if (!gateway_ack_)
            LOG(FATAL)("Unexpected request status"_sv);
          exchange_ack_ = true;
          break;
        default:
          LOG(FATAL)("Unexpected request status"_sv);
          break;
      }
    default:
      break;
  }
}

void CancelOrderState::operator()(const OrderUpdate &order_update) {
  LOG_IF(WARNING, order_update.order_id != order_id_)("Unexpected"_sv);
  LOG_IF(FATAL, !exchange_ack_)("Unexpected"_sv);
  if (roq::is_order_complete(order_update.status))
    strategy_.stop();
}

}  // namespace test
}  // namespace roq
