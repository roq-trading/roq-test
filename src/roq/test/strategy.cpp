/* Copyright (c) 2017-2021, Hans Erik Thrane */

#include "roq/test/strategy.h"

#include <algorithm>
#include <cassert>
#include <limits>
#include <utility>

#include "roq/logging.h"

#include "roq/test/flags.h"
#include "roq/test/wait_market_ready_state.h"

using namespace roq::literals;

namespace roq {
namespace test {

template <typename T>
inline bool update(T &lhs, const T &rhs) {  // XXX make utility
  if (lhs == rhs)                           // note! too simplistic for T == double
    return false;
  lhs = rhs;
  return true;
}

Strategy::Strategy(client::Dispatcher &dispatcher)
    : dispatcher_(dispatcher),
      depth_builder_(client::DepthBuilderFactory::create("test"_sv, depth_)),
      state_(std::make_unique<WaitMarketReadyState>(*this)) {
}

uint32_t Strategy::create_order() {
  auto side = Side::BUY;
  auto price = price_from_side(depth_[0], side) -
               sign(side) * reference_data_.tick_size * Flags::tick_offset_1();
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
      .stop_price = std::numeric_limits<double>::quiet_NaN(),
      .max_show_quantity = std::numeric_limits<double>::quiet_NaN(),
      .order_template = {},
  };
  LOG(INFO)(R"(create_order={})"_fmt, create_order);
  dispatcher_.send(create_order, uint8_t{0});
  return order_id_;
}

void Strategy::modify_order(uint32_t order_id) {
  auto side = Side::BUY;
  auto price = price_from_side(depth_[0], side) -
               sign(side) * reference_data_.tick_size * Flags::tick_offset_2();
  ModifyOrder modify_order{
      .account = Flags::account(),
      .order_id = order_id,
      .quantity = reference_data_.min_trade_vol,
      .price = price,
  };
  LOG(INFO)(R"(modify_order={})"_fmt, modify_order);
  dispatcher_.send(modify_order, uint8_t{0});
}

void Strategy::cancel_order(uint32_t order_id) {
  CancelOrder cancel_order{
      .account = Flags::account(),
      .order_id = order_id,
  };
  LOG(INFO)(R"(cancel_order={})"_fmt, cancel_order);
  dispatcher_.send(cancel_order, uint8_t{0});
}

// State::Handler

void Strategy::operator()(std::unique_ptr<State> &&state) {
  state_ = std::move(state);
}

void Strategy::stop() {
  LOG(INFO)("*** FINISHED ***"_fmt);
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

void Strategy::operator()(const Event<Connection> &event) {
  switch (event.value.status) {
    case ConnectionStatus::UNDEFINED:
      LOG(FATAL)("Unexpected"_fmt);
      break;
    case ConnectionStatus::DISCONNECTED:
      LOG(INFO)("Disconnected"_fmt);
      reset();
      break;
    case ConnectionStatus::CONNECTED:
      LOG(INFO)("Connected"_fmt);
      break;
  }
  check_ready();
}

void Strategy::operator()(const Event<DownloadBegin> &event) {
  if (event.value.account.empty()) {
    LOG(INFO)("Downloading market data ..."_fmt);
    market_data_.download = true;
  } else {
    LOG(INFO)("Downloading account data ..."_fmt);
    order_manager_.download = true;
  }
  check_ready();
}

void Strategy::operator()(const Event<DownloadEnd> &event) {
  LOG(INFO)(R"(download_end={})"_fmt, event.value);
  if (event.value.account.empty()) {
    LOG(INFO)("Download market data has COMPLETED"_fmt);
    market_data_.download = false;
  } else {
    LOG(INFO)("Download account data has COMPLETED"_fmt);
    order_manager_.download = false;
    order_id_ = std::max(order_id_, event.value.max_order_id);
  }
  check_ready();
}

void Strategy::operator()(const Event<MarketDataStatus> &event) {
  switch (event.value.status) {
    case GatewayStatus::READY:
      LOG(INFO)("Market data is READY"_fmt);
      market_data_.ready = true;
      break;
    default:
      LOG(INFO)("Market data is UNAVAILABLE"_fmt);
      market_data_.ready = false;
  }
  check_ready();
}

void Strategy::operator()(const Event<OrderManagerStatus> &event) {
  switch (event.value.status) {
    case GatewayStatus::READY:
      LOG(INFO)("Order manager is READY"_fmt);
      order_manager_.ready = true;
      break;
    default:
      LOG(INFO)("Order manager is UNAVAILABLE"_fmt);
      order_manager_.ready = false;
  }
  check_ready();
}

void Strategy::operator()(const Event<ReferenceData> &event) {
  depth_builder_->update(event.value);
  update(reference_data_.tick_size, event.value.tick_size);
  update(reference_data_.min_trade_vol, event.value.min_trade_vol);
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
  VLOG(1)(R"(depth=[{}])"_fmt, roq::join(depth_, R"(, )"_sv));
  check_depth();
}

void Strategy::operator()(const Event<OrderAck> &event) {
  LOG(INFO)(R"(order_ack={})"_fmt, event.value);
  assert(static_cast<bool>(state_) == true);
  (*state_)(event.value);
}

void Strategy::operator()(const Event<OrderUpdate> &event) {
  LOG(INFO)(R"(order_update={})"_fmt, event.value);
  if (ready_ == false)  // filter download
    return;
  // accept multiple similar order updates
  if (static_cast<bool>(state_) == true)
    (*state_)(event.value);
}

void Strategy::operator()(const Event<TradeUpdate> &event) {
  LOG(INFO)(R"(trade_update={})"_fmt, event.value);
}

void Strategy::operator()(const Event<PositionUpdate> &) {
}

void Strategy::operator()(const Event<FundsUpdate> &) {
}

void Strategy::check_depth() {
  auto ready =
      ready_ && std::fabs(depth_[0].bid_quantity) > 0.0 && std::fabs(depth_[0].ask_quantity) > 0.0;
  if (update(depth_ready_, ready) && depth_ready_)
    LOG(INFO)("*** READY TO TRADE ***"_fmt);
}

void Strategy::check_ready() {
  auto ready = market_data_.download == false && market_data_.ready == true &&
               order_manager_.download == false && order_manager_.ready == true &&
               std::fabs(reference_data_.tick_size) > 0.0 &&
               std::fabs(reference_data_.min_trade_vol) > 0.0 && reference_data_.trading == true;
  if (update(ready_, ready) && ready_)
    LOG(INFO)("*** INSTRUMENT READY ***"_fmt);
}

void Strategy::reset() {
  market_data_ = {};
  order_manager_ = {};
  reference_data_ = {};
  ready_ = false;
  depth_ready_ = false;
  depth_builder_->reset();
}

}  // namespace test
}  // namespace roq
