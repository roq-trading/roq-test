set(TARGET_NAME "${PROJECT_NAME}")

add_subdirectory(flags)

add_executable(
  "${TARGET_NAME}"
  application.cpp
  cancel_order_state.cpp
  config.cpp
  create_order_state.cpp
  modify_order_state.cpp
  flags.cpp
  state.cpp
  strategy.cpp
  wait_market_ready_state.cpp
  working_order_state.cpp
  working_order_state_2.cpp
  main.cpp)

target_link_libraries("${TARGET_NAME}" ${TARGET_NAME}-flags roq-client::roq-client roq-logging::roq-logging absl::flags
                      fmt::fmt)

target_compile_features("${TARGET_NAME}" PUBLIC cxx_std_17)

install(TARGETS ${TARGET_NAME})
