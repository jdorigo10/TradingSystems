add_executable(market_data_stream_tests main.cpp)

# Set output folder for this executable
set_target_properties(market_data_stream_tests PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/build
)

target_link_libraries(market_data_stream_tests
    PRIVATE market_data_stream
)