/* Copyright (c) 2017-2020, Hans Erik Thrane */

#include "roq/test/options.h"

#include <absl/flags/flag.h>

ABSL_FLAG(std::string, exchange, "deribit", "exchange");

ABSL_FLAG(std::string, symbol, "BTC-27MAR20", "symbol (regex)");

ABSL_FLAG(std::string, account, "A1", "account");

ABSL_FLAG(std::string, currencies, "BTC|USD", "currencies (regex)");

ABSL_FLAG(bool, enable_trading, false, "trading must explicitly be enabled!");

ABSL_FLAG(
    int32_t,
    tick_offset_1,
    int32_t{5},
    "initial offset against best price (tick_size units)");

ABSL_FLAG(
    bool,
    enable_modify_order,
    false,
    "enable testing of modify order messages");

ABSL_FLAG(
    int32_t,
    tick_offset_2,
    int32_t{2},
    "later offset against best price (tick_size units)");

ABSL_FLAG(
    uint32_t,
    volume_multiplier,
    uint32_t{1},
    "initial volument multiplier (min_trade_vol units)");

ABSL_FLAG(
    uint32_t,
    wait_time_secs,
    uint32_t{30},
    "wait time before next test is initiated (seconds)");
