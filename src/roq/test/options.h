/* Copyright (c) 2017-2020, Hans Erik Thrane */

#pragma once

#include <absl/flags/declare.h>

#include <cstdint>
#include <string>

ABSL_DECLARE_FLAG(std::string, exchange);
ABSL_DECLARE_FLAG(std::string, symbol);
ABSL_DECLARE_FLAG(std::string, account);
ABSL_DECLARE_FLAG(std::string, currencies);
ABSL_DECLARE_FLAG(bool, enable_trading);
ABSL_DECLARE_FLAG(int32_t, tick_offset_1);
ABSL_DECLARE_FLAG(bool, enable_modify_order);
ABSL_DECLARE_FLAG(int32_t, tick_offset_2);
ABSL_DECLARE_FLAG(uint32_t, volume_multiplier);
ABSL_DECLARE_FLAG(uint32_t, wait_time_secs);
