set(TARGET_NAME "${PROJECT_NAME}")

add_executable("${TARGET_NAME}"
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
  main.cpp
)

target_include_directories("${TARGET_NAME}" PUBLIC
  ${ROQ_LOGGING_INCLUDE_DIRS}
  ${ROQ_API_INCLUDE_DIRS}
  ${ROQ_CLIENT_INCLUDE_DIRS}
)

target_link_libraries("${TARGET_NAME}"
  ${ROQ_CLIENT_LIBRARIES}
  ${ROQ_LOGGING_LIBRARIES}
  fmt::fmt
  gflags
)

install(TARGETS ${TARGET_NAME})
