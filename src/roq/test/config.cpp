/* Copyright (c) 2017-2020, Hans Erik Thrane */

#include "roq/test/config.h"

#include <absl/flags/flag.h>

#include "roq/test/options.h"

namespace roq {
namespace test {

void Config::dispatch(Handler &handler) const {
  // accounts
  handler(client::Account{
      .regex = absl::GetFlag(FLAGS_account),
  });
  // symbols
  handler(client::Symbol{
      .regex = absl::GetFlag(FLAGS_symbol),
      .exchange = absl::GetFlag(FLAGS_exchange),
  });
  // currencies
  handler(client::Symbol{
      .regex = absl::GetFlag(FLAGS_currencies),
  });
}

}  // namespace test
}  // namespace roq
