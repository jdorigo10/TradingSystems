add_subdirectory(common)
add_subdirectory(main-pipeline)
add_subdirectory(side-systems)

add_executable(trading_system main.cpp)

# Set output folder for this executable
set_target_properties(trading_system PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/build
)

target_link_libraries(trading_system
    # Shared Types
    common

    # Main Pipeline
    market_feed_handler
    order_book_builder
    market_data_stream
    strategy_engine
    risk_manager
    rate_limiter
    order_gateway

    # Side Systems
    order_tracker
    position_pnl_tracker
    offset_hedger
    trade_logger
    latency_monitor
)