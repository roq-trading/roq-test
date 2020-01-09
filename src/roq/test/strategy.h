/* Copyright (c) 2017-2020, Hans Erik Thrane */

#pragma once

#include <chrono>
#include <memory>

#include "roq/client.h"

#include "roq/test/state.h"

namespace roq {
namespace test {

class Strategy final : public client::Handler {
 public:
  explicit Strategy(client::Dispatcher& dispatcher);

  Strategy(const Strategy&) = delete;
  Strategy(Strategy&&) = default;

  void operator=(const Strategy&) = delete;
  void operator=(Strategy&&) = delete;

  void operator()(std::unique_ptr<State>&&);

  void stop();

  bool ready();

  uint32_t create_order();
  void cancel_order(uint32_t order_id);

 protected:
  void operator()(const TimerEvent& event) override;
  void operator()(const ConnectionStatusEvent& event) override;
  void operator()(const DownloadBeginEvent& event) override;
  void operator()(const DownloadEndEvent& event) override;
  void operator()(const MarketDataStatusEvent& event) override;
  void operator()(const OrderManagerStatusEvent& event) override;
  void operator()(const ReferenceDataEvent& event) override;
  void operator()(const MarketStatusEvent& event) override;
  void operator()(const MarketByPriceEvent& event) override;
  void operator()(const OrderAckEvent& event) override;
  void operator()(const OrderUpdateEvent& event) override;
  void operator()(const TradeUpdateEvent& event) override;
  void operator()(const PositionUpdateEvent& event) override;
  void operator()(const FundsUpdateEvent& event) override;

  void check_ready();
  void check_depth();

  void reset();

  void check(std::chrono::nanoseconds now);

 private:
  client::Dispatcher& _dispatcher;
  // state
  struct {
    bool download = false;
    bool ready = false;
  } _market_data;
  struct {
    bool download = false;
    bool ready = false;
  } _order_manager;
  struct {
    double tick_size = 0.0;
    double min_trade_vol = 0.0;
    bool trading = false;
  } _reference_data;
  uint32_t _order_id = 0;
  bool _ready = false;
  std::array<Layer, 2> _depth;
  std::unique_ptr<client::DepthBuilder> _depth_builder;
  bool _depth_ready = false;
  // finite state machine
  std::unique_ptr<State> _state;
  bool _stop = false;
};

}  // namespace test
}  // namespace roq
