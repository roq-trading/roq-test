/* Copyright (c) 2017-2020, Hans Erik Thrane */

#include "roq/test/state.h"

#include "roq/test/strategy.h"

namespace roq {
namespace test {

State::State(Strategy& strategy)
    : _strategy(strategy) {
}

}  // namespace test
}  // namespace roq
