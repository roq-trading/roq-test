/* Copyright (c) 2017-2020, Hans Erik Thrane */

#include "roq/test/config.h"

#include "roq/test/flags.h"

namespace roq {
namespace test {

void Config::dispatch(Handler &handler) const {
  // accounts
  handler(client::Account{
      .regex = Flags::account(),
  });
  // symbols
  handler(client::Symbol{
      .regex = Flags::symbol(),
      .exchange = Flags::exchange(),
  });
  // currencies
  handler(client::Symbol{
      .regex = Flags::currencies(),
  });
}

}  // namespace test
}  // namespace roq
