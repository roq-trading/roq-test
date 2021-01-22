set(TARGET_NAME "${PROJECT_NAME}")

add_executable(
  "${TARGET_NAME}"
  application.cpp
  cancel_order_state.cpp
  config.cpp
  create_order_state.cpp
  modify_order_state.cpp
  options.cpp
  state.cpp
  strategy.cpp
  wait_market_ready_state.cpp
  working_order_state.cpp
  working_order_state_2.cpp
  main.cpp)

target_link_libraries("${TARGET_NAME}" roq-client::roq-client roq-logging::roq-logging absl::flags fmt::fmt)

install(TARGETS ${TARGET_NAME})
