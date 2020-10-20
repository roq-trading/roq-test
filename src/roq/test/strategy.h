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
  explicit Strategy(client::Dispatcher &dispatcher);

  Strategy(const Strategy &) = delete;
  Strategy(Strategy &&) = default;

  void operator()(std::unique_ptr<State> &&);

  void stop();

  bool ready();

  uint32_t create_order();
  void modify_order(uint32_t order_id);
  void cancel_order(uint32_t order_id);

 protected:
  void operator()(const Event<Timer> &event) override;
  void operator()(const Event<Connection> &event) override;
  void operator()(const Event<DownloadBegin> &event) override;
  void operator()(const Event<DownloadEnd> &event) override;
  void operator()(const Event<MarketDataStatus> &event) override;
  void operator()(const Event<OrderManagerStatus> &event) override;
  void operator()(const Event<ReferenceData> &event) override;
  void operator()(const Event<MarketStatus> &event) override;
  void operator()(const Event<MarketByPriceUpdate> &event) override;
  void operator()(const Event<OrderAck> &event) override;
  void operator()(const Event<OrderUpdate> &event) override;
  void operator()(const Event<TradeUpdate> &event) override;
  void operator()(const Event<PositionUpdate> &event) override;
  void operator()(const Event<FundsUpdate> &event) override;

  void check_ready();
  void check_depth();

  void reset();

  void check(std::chrono::nanoseconds now);

 private:
  client::Dispatcher &_dispatcher;
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
