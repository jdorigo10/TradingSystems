add_executable(market_feed_handler_tests main.cpp)

# Set output folder for this executable
set_target_properties(market_feed_handler_tests PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/build
)

target_link_libraries(market_feed_handler_tests
    PRIVATE market_feed_handler
)