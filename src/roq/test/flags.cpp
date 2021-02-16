/* Copyright (c) 2017-2021, Hans Erik Thrane */

#include "roq/test/flags.h"

#include <absl/flags/flag.h>

#include <string>

#include "roq/literals.h"

using namespace roq::literals;

ABSL_FLAG(  //
    std::string,
    exchange,
    "deribit"_s,
    "exchange"_sv);

ABSL_FLAG(  //
    std::string,
    symbol,
    "BTC-27MAR20"_s,
    "symbol (regex)"_sv);

ABSL_FLAG(  //
    std::string,
    account,
    "A1"_s,
    "account"_sv);

ABSL_FLAG(  //
    std::string,
    currencies,
    "BTC|USD"_s,
    "currencies (regex)"_sv);

ABSL_FLAG(  //
    int32_t,
    tick_offset_1,
    5,
    "initial offset against best price (tick_size units)"_sv);

ABSL_FLAG(  //
    bool,
    enable_modify_order,
    false,
    "enable testing of modify order messages"_sv);

ABSL_FLAG(  //
    int32_t,
    tick_offset_2,
    2,
    "later offset against best price (tick_size units)"_sv);

ABSL_FLAG(  //
    uint32_t,
    volume_multiplier,
    1u,
    "initial volument multiplier (min_trade_vol units)"_sv);

ABSL_FLAG(  //
    uint32_t,
    wait_time_secs,
    30u,
    "wait time before next test is initiated (seconds)"_sv);

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
