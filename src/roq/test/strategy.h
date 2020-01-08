/* Copyright (c) 2017-2020, Hans Erik Thrane */

#include <chrono>
#include <memory>

#include "roq/client.h"

namespace roq {
namespace test {

class Strategy final : public client::Handler {
 public:
  explicit Strategy(client::Dispatcher& dispatcher);

  Strategy(const Strategy&) = delete;
  Strategy(Strategy&&) = default;

  void operator=(const Strategy&) = delete;
  void operator=(Strategy&&) = delete;

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

  void create_order(uint32_t order_id, Side side) const;
  void cancel_order(uint32_t order_id) const;

 private:
  client::Dispatcher& _dispatcher;
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
  bool _depth_ready = false;
  std::array<Layer, 2> _depth;
  std::unique_ptr<client::DepthBuilder> _depth_builder;
  enum class State {
    CREATE_ORDER,
    CREATE_ORDER_ACK,
    CANCEL_ORDER,
    CANCEL_ORDER_ACK,
    DONE,
  } _state = State::CREATE_ORDER;
  std::chrono::nanoseconds _next_update = {};
};

}  // namespace test
}  // namespace roq
