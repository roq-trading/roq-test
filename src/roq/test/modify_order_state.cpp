/* Copyright (c) 2017-2021, Hans Erik Thrane */

#include "roq/test/modify_order_state.h"

#include <memory>

#include "roq/logging.h"

#include "roq/utils/common.h"

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
  if (order_ack.type != RequestType::MODIFY_ORDER)
    log::fatal("Unexpected"_sv);
  switch (order_ack.origin) {
    case Origin::GATEWAY:
      switch (order_ack.status) {
        case RequestStatus::FORWARDED:
          if (exchange_ack_)
            log::fatal("Unexpected"_sv);
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
  check();
}

void ModifyOrderState::operator()(const OrderUpdate &order_update) {
  if (order_update.order_id != order_id_)
    log::warn("Unexpected"_sv);
  if (utils::is_order_complete(order_update.status)) {
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
