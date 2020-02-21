/* Copyright (c) 2017-2020, Hans Erik Thrane */

#pragma once

#include <gflags/gflags.h>

DECLARE_string(exchange);
DECLARE_string(symbol);
DECLARE_string(account);
DECLARE_string(currencies);
DECLARE_bool(enable_trading);
DECLARE_int32(tick_offset_1);
DECLARE_bool(enable_modify_order);
DECLARE_int32(tick_offset_2);
DECLARE_uint32(volume_multiplier);
DECLARE_uint32(wait_time_secs);
