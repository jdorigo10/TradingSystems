add_executable(trade_logger_tests main.cpp)

# Set output folder for this executable
set_target_properties(trade_logger_tests PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/build
)

target_link_libraries(trade_logger_tests
    PRIVATE trade_logger
)