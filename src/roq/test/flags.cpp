/* Copyright (c) 2017-2021, Hans Erik Thrane */

#include "roq/test/flags.h"

#include <absl/flags/flag.h>

#include <string>

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

namespace roq {
namespace test {

std::string_view Flags::exchange() {
  static const std::string result = absl::GetFlag(FLAGS_exchange);
  return result;
}

std::string_view Flags::symbol() {
  static const std::string result = absl::GetFlag(FLAGS_symbol);
  return result;
}

std::string_view Flags::account() {
  static const std::string result = absl::GetFlag(FLAGS_account);
  return result;
}

std::string_view Flags::currencies() {
  static const std::string result = absl::GetFlag(FLAGS_currencies);
  return result;
}

bool Flags::enable_trading() {
  static const bool result = absl::GetFlag(FLAGS_enable_trading);
  return result;
}

int32_t Flags::tick_offset_1() {
  static const int32_t result = absl::GetFlag(FLAGS_tick_offset_1);
  return result;
}

bool Flags::enable_modify_order() {
  static const bool result = absl::GetFlag(FLAGS_enable_modify_order);
  return result;
}

int32_t Flags::tick_offset_2() {
  static const int32_t result = absl::GetFlag(FLAGS_tick_offset_2);
  return result;
}

uint32_t Flags::volume_multiplier() {
  static const uint32_t result = absl::GetFlag(FLAGS_volume_multiplier);
  return result;
}

uint32_t Flags::wait_time_secs() {
  static const uint32_t result = absl::GetFlag(FLAGS_wait_time_secs);
  return result;
}

}  // namespace test
}  // namespace roq
