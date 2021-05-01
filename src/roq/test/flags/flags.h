/* Copyright (c) 2017-2021, Hans Erik Thrane */

#pragma once

#include <cstdint>
#include <string_view>

namespace roq {
namespace test {
namespace flags {

struct Flags final {
  static std::string_view exchange();
  static std::string_view symbol();
  static std::string_view account();
  static std::string_view currencies();
  static int32_t tick_offset_1();
  static bool enable_modify_order();
  static int32_t tick_offset_2();
  static uint32_t volume_multiplier();
  static uint32_t wait_time_secs();
  static std::string_view routing_id();
};

}  // namespace flags
}  // namespace test
}  // namespace roq
