/* Copyright (c) 2017-2021, Hans Erik Thrane */

#include "roq/test/create_order_state.h"

#include <memory>

#include "roq/logging.h"

#include "roq/utils/common.h"

#include "roq/test/strategy.h"
#include "roq/test/working_order_state.h"

using namespace roq::literals;

namespace roq {
namespace test {

CreateOrderState::CreateOrderState(Strategy &strategy) : State(strategy), order_id_(strategy_.create_order()) {
}

void CreateOrderState::operator()(std::chrono::nanoseconds) {
  // TODO(thraneh): check timeout
}

void CreateOrderState::operator()(const OrderAck &order_ack) {
  if (order_ack.type != RequestType::CREATE_ORDER)
    log::fatal("Unexpected"_sv);
  switch (order_ack.origin) {
    case Origin::GATEWAY:
      switch (order_ack.status) {
        case RequestStatus::FORWARDED:
          gateway_ack_ = true;
          break;
        default:
          log::fatal("Unexpected request status"_sv);
          break;
      }
      break;
    case Origin::EXCHANGE:
      switch (order_ack.status) {
        case RequestStatus::ACCEPTED:
          if (!gateway_ack_)
            log::fatal("Unexpected request status"_sv);
          exchange_ack_ = true;
          break;
        default:
          log::fatal("Unexpected request status"_sv);
          break;
      }
    default:
      break;
  }
}

void CreateOrderState::operator()(const OrderUpdate &order_update) {
  if (order_update.order_id != order_id_)
    log::warn("Unexpected"_sv);
  if (!exchange_ack_)
    log::fatal("Unexpected"_sv);
  if (utils::is_order_complete(order_update.status)) {
    strategy_.stop();
  } else {
    strategy_(std::make_unique<WorkingOrderState>(strategy_, order_id_));
  }
}

}  // namespace test
}  // namespace roq
