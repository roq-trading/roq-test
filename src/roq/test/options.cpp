/* Copyright (c) 2017-2020, Hans Erik Thrane */

#include "roq/test/options.h"

DEFINE_string(exchange, "deribit", "exchange");

DEFINE_string(symbol, "BTC-27MAR20", "symbol (regex)");

DEFINE_string(account, "A1", "account");

DEFINE_string(currencies, "BTC|USD", "currencies (regex)");

DEFINE_bool(enable_trading, false, "trading must explicitly be enabled!");

DEFINE_int32(
    tick_offset_1,
    int32_t{5},
    "initial offset against best price (tick_size units)");

DEFINE_bool(
    enable_modify_order, false, "enable testing of modify order messages");

DEFINE_int32(
    tick_offset_2,
    int32_t{2},
    "later offset against best price (tick_size units)");

DEFINE_uint32(
    volume_multiplier,
    uint32_t{1},
    "initial volument multiplier (min_trade_vol units)");

DEFINE_uint32(
    wait_time_secs,
    uint32_t{30},
    "wait time before next test is initiated (seconds)");
