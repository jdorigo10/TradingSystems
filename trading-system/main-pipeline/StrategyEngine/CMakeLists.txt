add_library(strategy_engine
    src/StrategyEngine.cpp
)

target_include_directories(strategy_engine
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(strategy_engine
    PUBLIC
        common
        market_data_stream
        order_book_builder
)

add_subdirectory(tests)