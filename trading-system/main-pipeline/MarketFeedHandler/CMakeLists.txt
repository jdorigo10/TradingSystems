add_library(market_feed_handler
    src/FeedHandler.cpp
)

target_include_directories(market_feed_handler
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(market_feed_handler
    PUBLIC common
)

add_subdirectory(tests)