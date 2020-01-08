/* Copyright (c) 2017-2020, Hans Erik Thrane */

#include "roq/test/config.h"

#include "roq/test/options.h"

namespace roq {
namespace test {

void Config::dispatch(Handler& handler) const {
  // accounts
  handler(
      client::Account {
        .regex = FLAGS_account,
      });
  // symbols
  handler(
      client::Symbol {
        .regex = FLAGS_symbol,
        .exchange = FLAGS_exchange,
      });
  // currencies
  handler(
      client::Symbol {
        .regex = FLAGS_currencies,
      });
}

}  // namespace test
}  // namespace roq
