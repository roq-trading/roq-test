/* Copyright (c) 2017-2021, Hans Erik Thrane */

#pragma once

#include <chrono>
#include <memory>

#include "roq/client.h"

#include "roq/test/state.h"

namespace roq {
namespace test {

class Strategy final : public client::Handler {
 public:
  explicit Strategy(client::Dispatcher &);

  Strategy(Strategy &&) = default;
  Strategy(const Strategy &) = delete;

  void operator()(std::unique_ptr<State> &&);

  void stop();

  bool ready();

  uint32_t create_order();

  void modify_order(uint32_t order_id);
  void cancel_order(uint32_t order_id);

 protected:
  void operator()(const Event<Timer> &) override;
  void operator()(const Event<Connection> &) override;
  void operator()(const Event<DownloadBegin> &) override;
  void operator()(const Event<DownloadEnd> &) override;
  void operator()(const Event<MarketDataStatus> &) override;
  void operator()(const Event<OrderManagerStatus> &) override;
  void operator()(const Event<ReferenceData> &) override;
  void operator()(const Event<MarketStatus> &) override;
  void operator()(const Event<MarketByPriceUpdate> &) override;
  void operator()(const Event<OrderAck> &) override;
  void operator()(const Event<OrderUpdate> &) override;
  void operator()(const Event<TradeUpdate> &) override;
  void operator()(const Event<PositionUpdate> &) override;
  void operator()(const Event<FundsUpdate> &) override;

  void check_ready();
  void check_depth();

  void reset();

  void check(std::chrono::nanoseconds now);

 private:
  client::Dispatcher &dispatcher_;
  // state
  struct {
    bool download = false;
    bool ready = false;
  } market_data_;
  struct {
    bool download = false;
    bool ready = false;
  } order_manager_;
  struct {
    double tick_size = {};
    double min_trade_vol = {};
    bool trading = false;
  } reference_data_;
  uint32_t order_id_ = {};
  bool ready_ = false;
  std::array<Layer, 2> depth_;
  std::unique_ptr<client::DepthBuilder> depth_builder_;
  bool depth_ready_ = false;
  // finite state machine
  std::unique_ptr<State> state_;
  bool stop_ = false;
};

}  // namespace test
}  // namespace roq
