add_library(trade_logger
    src/TradeLogger.cpp
)

target_include_directories(trade_logger
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(trade_logger
    PUBLIC common
)

add_subdirectory(tests)