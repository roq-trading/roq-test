/* Copyright (c) 2017-2020, Hans Erik Thrane */

#include "roq/test/strategy.h"

#include <cassert>
#include <utility>

#include "roq/logging.h"

#include "roq/test/wait_market_ready_state.h"
#include "roq/test/options.h"

namespace roq {
namespace test {

template <typename T>
inline bool update(T& lhs, const T& rhs) {  // XXX make utility
  if (lhs == rhs)  // note! too simplistic for T == double
    return false;
  lhs = rhs;
    return true;
}

Strategy::Strategy(client::Dispatcher& dispatcher)
    : _dispatcher(dispatcher),
      _depth_builder(
          client::DepthBuilder::create(
              "test",
              _depth)),
      _state(
          std::make_unique<WaitMarketReadyState>(*this)) {
}

uint32_t Strategy::create_order() {
  auto side = Side::BUY;
  auto price =
    price_from_side(_depth[0], side) -
    sign(side) * _reference_data.tick_size * FLAGS_tick_offset_1;
  CreateOrder create_order {
    .account = FLAGS_account,
    .order_id = ++_order_id,
    .exchange = FLAGS_exchange,
    .symbol = FLAGS_symbol,
    .side = side,
    .quantity = _reference_data.min_trade_vol,
    .order_type = OrderType::LIMIT,
    .price = price,
    .time_in_force = TimeInForce::GTC,
    .position_effect = PositionEffect::UNDEFINED,
    .order_template = "",
  };
  LOG(INFO)(
      FMT_STRING("create_order={}"),
      create_order);
  _dispatcher.send(
      create_order,
      uint8_t{0});
  return _order_id;
}

void Strategy::modify_order(uint32_t order_id) {
  auto side = Side::BUY;
  auto price =
    price_from_side(_depth[0], side) -
    sign(side) * _reference_data.tick_size * FLAGS_tick_offset_2;
  ModifyOrder modify_order {
    .account = FLAGS_account,
    .order_id = order_id,
    .quantity = _reference_data.min_trade_vol,
    .price = price,
  };
  LOG(INFO)(
      FMT_STRING("modify_order={}"),
      modify_order);
  _dispatcher.send(
      modify_order,
      uint8_t{0});
}

void Strategy::cancel_order(uint32_t order_id) {
  CancelOrder cancel_order {
    .account = FLAGS_account,
    .order_id = order_id,
  };
  LOG(INFO)(
      FMT_STRING("cancel_order={}"),
      cancel_order);
  _dispatcher.send(
      cancel_order,
      uint8_t{0});
}

// State::Handler

void Strategy::operator()(std::unique_ptr<State>&& state) {
  _state = std::move(state);
}

void Strategy::stop() {
  LOG(INFO)("*** FINISHED ***");
  _stop = true;
  _state.reset();
}

bool Strategy::ready() {
  return _depth_ready;
}

// client::Handler

void Strategy::operator()(const TimerEvent& event) {
  if (_stop)
    _dispatcher.stop();
  if (_state)
    (*_state)(event.now);
}

void Strategy::operator()(const ConnectionStatusEvent& event) {
  switch (event.connection_status) {
    case ConnectionStatus::DISCONNECTED:
      LOG(INFO)("Disconnected");
      reset();
      break;
    case ConnectionStatus::CONNECTED:
      LOG(INFO)("Connected");
      break;
  }
  check_ready();
}

void Strategy::operator()(const DownloadBeginEvent& event) {
  if (event.download_begin.account.empty()) {
    LOG(INFO)("Downloading market data ...");
    _market_data.download = true;
  } else {
    LOG(INFO)("Downloading account data ...");
    _order_manager.download = true;
  }
  check_ready();
}

void Strategy::operator()(const DownloadEndEvent& event) {
  LOG(INFO)(
      FMT_STRING("download_end={}"),
      event.download_end);
  if (event.download_end.account.empty()) {
    LOG(INFO)("Download market data has COMPLETED");
    _market_data.download = false;
  } else {
    LOG(INFO)("Download account data has COMPLETED");
    _order_manager.download = false;
    _order_id = std::max(
        _order_id,
        event.download_end.max_order_id);
  }
  check_ready();
}

void Strategy::operator()(const MarketDataStatusEvent& event) {
  switch (event.market_data_status.status) {
    case GatewayStatus::READY:
      LOG(INFO)("Market data is READY");
      _market_data.ready = true;
      break;
    default:
      LOG(INFO)("Market data is UNAVAILABLE");
      _market_data.ready = false;
  }
  check_ready();
}

void Strategy::operator()(const OrderManagerStatusEvent& event) {
  switch (event.order_manager_status.status) {
    case GatewayStatus::READY:
      LOG(INFO)("Order manager is READY");
      _order_manager.ready = true;
      break;
    default:
      LOG(INFO)("Order manager is UNAVAILABLE");
      _order_manager.ready = false;
  }
  check_ready();
}

void Strategy::operator()(const ReferenceDataEvent& event) {
  _depth_builder->update(event.reference_data);
  update(
      _reference_data.tick_size,
      event.reference_data.tick_size);
  update(
      _reference_data.min_trade_vol,
      event.reference_data.min_trade_vol);
  check_ready();
}

void Strategy::operator()(const MarketStatusEvent& event) {
  switch (event.market_status.trading_status) {
    case TradingStatus::OPEN:
      _reference_data.trading = true;
      break;
    default:
      _reference_data.trading = false;
  }
  check_ready();
}

void Strategy::operator()(const MarketByPriceEvent& event) {
  _depth_builder->update(event.market_by_price);
  VLOG(1)(
      FMT_STRING("depth=[{}]"),
      fmt::join(_depth, ", "));
  check_depth();
}

void Strategy::operator()(const OrderAckEvent& event) {
  LOG(INFO)(
      FMT_STRING("order_ack={}"),
      event.order_ack);
  assert(static_cast<bool>(_state) == true);
  (*_state)(event.order_ack);
}

void Strategy::operator()(const OrderUpdateEvent& event) {
  LOG(INFO)(
      FMT_STRING("order_update={}"),
      event.order_update);
  if (_ready == false)  // filter download
    return;
  // accept multiple similar order updates
  if (static_cast<bool>(_state) == true)
    (*_state)(event.order_update);
}

void Strategy::operator()(const TradeUpdateEvent& event) {
  LOG(INFO)(
      FMT_STRING("trade_update={}"),
      event.trade_update);
}

void Strategy::operator()(const PositionUpdateEvent&) {
}

void Strategy::operator()(const FundsUpdateEvent&) {
}

void Strategy::check_depth() {
  auto ready =
    _ready &&
    std::fabs(_depth[0].bid_quantity) > 0.0 &&
    std::fabs(_depth[0].ask_quantity) > 0.0;
  if (update(_depth_ready, ready) && _depth_ready)
    LOG(INFO)("*** READY TO TRADE ***");
}

void Strategy::check_ready() {
  auto ready =
    _market_data.download == false &&
    _market_data.ready == true &&
    _order_manager.download == false &&
    _order_manager.ready == true &&
    std::fabs(_reference_data.tick_size) > 0.0 &&
    std::fabs(_reference_data.min_trade_vol) > 0.0 &&
    _reference_data.trading == true;
  if (update(_ready, ready) && _ready)
    LOG(INFO)("*** INSTRUMENT READY ***");
}

void Strategy::reset() {
  _market_data = {};
  _order_manager = {};
  _reference_data = {};
  _ready = false;
  _depth_ready = false;
  _depth_builder->reset();
}

}  // namespace test
}  // namespace roq
