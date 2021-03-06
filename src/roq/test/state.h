/* Copyright (c) 2017-2021, Hans Erik Thrane */

#pragma once

#include <memory>

#include "roq/client.h"

namespace roq {
namespace test {

class Strategy;

class State {
 public:
  explicit State(Strategy &);

  State(State &&) = default;
  State(const State &) = delete;

  virtual ~State() {}

  virtual void operator()(std::chrono::nanoseconds now) = 0;
  virtual void operator()(const OrderAck &) = 0;
  virtual void operator()(const OrderUpdate &) = 0;

 protected:
  Strategy &strategy_;
};

}  // namespace test
}  // namespace roq
