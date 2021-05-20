/* Copyright (c) 2017-2021, Hans Erik Thrane */

#include "roq/test/strategy.h"

#include <algorithm>
#include <cassert>
#include <utility>

#include "roq/logging.h"

#include "roq/utils/common.h"
#include "roq/utils/compare.h"
#include "roq/utils/mask.h"
#include "roq/utils/update.h"

#include "roq/test/flags.h"
#include "roq/test/wait_market_ready_state.h"

using namespace roq::literals;

namespace roq {
namespace test {

Strategy::Strategy(client::Dispatcher &dispatcher)
    : dispatcher_(dispatcher),
      depth_builder_(client::DepthBuilderFactory::create("test"_sv, depth_)),
      state_(std::make_unique<WaitMarketReadyState>(*this)) {
}

uint32_t Strategy::create_order() {
  auto side = Side::BUY;
  auto price = utils::price_from_side(depth_[0], side) -
               utils::sign(side) * reference_data_.tick_size * Flags::tick_offset_1();
  CreateOrder create_order{
      .account = Flags::account(),
      .order_id = ++order_id_,
      .exchange = Flags::exchange(),
      .symbol = Flags::symbol(),
      .side = side,
      .quantity = reference_data_.min_trade_vol,
      .order_type = OrderType::LIMIT,
      .price = price,
      .time_in_force = TimeInForce::GTC,
      .position_effect = PositionEffect::UNDEFINED,
      .execution_instruction = ExecutionInstruction::UNDEFINED,
      .stop_price = NaN,
      .max_show_quantity = NaN,
      .order_template = {},
      .routing_id = Flags::routing_id(),
  };
  log::info("create_order={}"_fmt, create_order);
  dispatcher_.send(create_order, 0u);
  return order_id_;
}

void Strategy::modify_order(uint32_t order_id) {
  auto side = Side::BUY;
  auto price = utils::price_from_side(depth_[0], side) -
               utils::sign(side) * reference_data_.tick_size * Flags::tick_offset_2();
  ModifyOrder modify_order{
      .account = Flags::account(),
      .order_id = order_id,
      .quantity = reference_data_.min_trade_vol,
      .price = price,
      .routing_id = {},
  };
  log::info("modify_order={}"_fmt, modify_order);
  dispatcher_.send(modify_order, 0u);
}

void Strategy::cancel_order(uint32_t order_id) {
  CancelOrder cancel_order{
      .account = Flags::account(),
      .order_id = order_id,
      .routing_id = {},
  };
  log::info("cancel_order={}"_fmt, cancel_order);
  dispatcher_.send(cancel_order, 0u);
}

// State::Handler

void Strategy::operator()(std::unique_ptr<State> &&state) {
  state_ = std::move(state);
}

void Strategy::stop() {
  log::info("*** FINISHED ***"_sv);
  stop_ = true;
  state_.reset();
}

bool Strategy::ready() {
  return depth_ready_;
}

// client::Handler

void Strategy::operator()(const Event<Timer> &event) {
  if (stop_)
    dispatcher_.stop();
  if (state_)
    (*state_)(event.value.now);
}

void Strategy::operator()(const Event<Connected> &) {
  log::info("Connected"_sv);
  check_ready();
}

void Strategy::operator()(const Event<Disconnected> &) {
  log::info("Disconnected"_sv);
  reset();
  check_ready();
}

void Strategy::operator()(const Event<DownloadBegin> &event) {
  if (event.value.account.empty()) {
    log::info("Downloading market data ..."_sv);
    market_data_.download = true;
  } else {
    log::info("Downloading account data ..."_sv);
    order_management_.download = true;
  }
  check_ready();
}

void Strategy::operator()(const Event<DownloadEnd> &event) {
  log::info("download_end={}"_fmt, event.value);
  if (event.value.account.empty()) {
    log::info("Download market data has COMPLETED"_sv);
    market_data_.download = false;
  } else {
    log::info("Download account data has COMPLETED"_sv);
    order_management_.download = false;
    order_id_ = std::max(order_id_, event.value.max_order_id);
  }
  check_ready();
}

void Strategy::operator()(const Event<GatewayStatus> &event) {
  auto &gateway_status = event.value;
  log::info("gateway_status={}"_fmt, gateway_status);
  auto account = gateway_status.account;
  utils::Mask<SupportType> available(gateway_status.available),
      unavailable(gateway_status.unavailable);
  if (account.empty()) {
    static const utils::Mask<SupportType> required{
        SupportType::REFERENCE_DATA,
        SupportType::MARKET_STATUS,
        SupportType::MARKET_BY_PRICE,
    };
    auto market_data = available.has_all(required) && unavailable.has_none(required);
    if (utils::update(market_data_.ready, market_data))
      log::info("Market data is {}READY"_fmt, market_data_.ready ? ""_sv : "NOT "_sv);
    if (!market_data_.ready) {
      auto missing = required & ~available;
      log::debug("missing={:#x}"_fmt, missing.get());
    }
  } else if (account.compare(Flags::account()) == 0) {
    // note!
    // - should maybe check for modified order here (depends on flags)
    // - should only sometimes check for positions and funds (not always available from exchange)
    static const utils::Mask<SupportType> required{
        SupportType::CREATE_ORDER,
        SupportType::CANCEL_ORDER,
        SupportType::ORDER_ACK,
        SupportType::ORDER,
        SupportType::TRADE,
    };
    auto order_management = available.has_all(required) && unavailable.has_none(required);
    if (utils::update(order_management_.ready, order_management))
      log::info("Order management is {}READY"_fmt, order_management_.ready ? ""_sv : "NOT "_sv);
    if (!order_management_.ready) {
      auto missing = required & ~available;
      log::debug("missing={:#x}"_fmt, missing.get());
    }
  }
  check_ready();
}

void Strategy::operator()(const Event<ReferenceData> &event) {
  depth_builder_->update(event.value);
  utils::update(reference_data_.tick_size, event.value.tick_size);
  utils::update(reference_data_.min_trade_vol, event.value.min_trade_vol);
  check_ready();
}

void Strategy::operator()(const Event<MarketStatus> &event) {
  switch (event.value.trading_status) {
    case TradingStatus::OPEN:
      reference_data_.trading = true;
      break;
    default:
      reference_data_.trading = false;
  }
  check_ready();
}

void Strategy::operator()(const Event<MarketByPriceUpdate> &event) {
  depth_builder_->update(event.value);
  log::trace_1("depth=[{}]"_fmt, roq::join(depth_, ", "_sv));
  check_depth();
}

void Strategy::operator()(const Event<OrderAck> &event) {
  log::info("order_ack={}"_fmt, event.value);
  assert(static_cast<bool>(state_));
  (*state_)(event.value);
}

void Strategy::operator()(const Event<OrderUpdate> &event) {
  log::info("order_update={}"_fmt, event.value);
  if (!ready_)  // filter download
    return;
  // accept multiple similar order updates
  if (static_cast<bool>(state_))
    (*state_)(event.value);
}

void Strategy::operator()(const Event<TradeUpdate> &event) {
  log::info("trade_update={}"_fmt, event.value);
}

void Strategy::operator()(const Event<PositionUpdate> &) {
}

void Strategy::operator()(const Event<FundsUpdate> &) {
}

void Strategy::check_depth() {
  auto ready = ready_ && utils::compare(depth_[0].bid_quantity, 0.0) > 0 &&
               utils::compare(depth_[0].ask_quantity, 0.0) > 0;
  if (utils::update(depth_ready_, ready) && depth_ready_)
    log::info("*** READY TO TRADE ***"_sv);
}

void Strategy::check_ready() {
  log::debug(
      "market_data={{download={}, ready={}}}, "
      "order_management={{download={}, ready={}}}, "
      "reference_data={{tick_size={}, min_trade_vol={}, trading={}}}"_fmt,
      market_data_.download,
      market_data_.ready,
      order_management_.download,
      order_management_.ready,
      reference_data_.tick_size,
      reference_data_.min_trade_vol,
      reference_data_.trading);
  auto ready = !market_data_.download && market_data_.ready && !order_management_.download &&
               order_management_.ready && utils::compare(reference_data_.tick_size, 0.0) > 0 &&
               utils::compare(reference_data_.min_trade_vol, 0.0) > 0 && reference_data_.trading;
  if (utils::update(ready_, ready) && ready_)
    log::info("*** INSTRUMENT READY ***"_sv);
  log::debug("ready={}"_fmt, ready_);
}

void Strategy::reset() {
  market_data_ = {};
  order_management_ = {};
  reference_data_ = {};
  ready_ = false;
  depth_ready_ = false;
  depth_builder_->reset();
}

}  // namespace test
}  // namespace roq
