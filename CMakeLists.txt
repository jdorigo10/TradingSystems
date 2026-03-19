cmake_minimum_required(VERSION 3.16)

project(TradingSystem)

set(CMAKE_CXX_STANDARD 17)

add_subdirectory(common)
add_subdirectory(exchange)
add_subdirectory(trading-system)
add_subdirectory(monitoring)

add_executable(trading_system main.cpp)

# Set output folder for this executable
set_target_properties(trading_system PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/build
)

target_link_libraries(trading_system
    # Shared Types
    common

    # Exchange
    exchange

    # Trading System
    market_feed_handler
    order_book_manager
    market_data_manager
    order_tracker
    position_tracker
    strategy_engine
    offset_hedger
    risk_manager
    rate_limiter
    order_gateway

    # Monitoring
    trade_logger
    latency_monitor
)
