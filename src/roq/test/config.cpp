/* Copyright (c) 2017-2021, Hans Erik Thrane */

#include "roq/test/config.h"

#include "roq/test/flags.h"

namespace roq {
namespace test {

void Config::dispatch(Handler &handler) const {
  // settings
  handler(client::Settings{
      .order_cancel_policy = OrderCancelPolicy::BY_ACCOUNT,
      .order_management = {},
  });
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
