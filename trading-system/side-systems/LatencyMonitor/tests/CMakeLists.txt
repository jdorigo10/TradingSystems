add_executable(latency_monitor_tests main.cpp)

# Set output folder for this executable
set_target_properties(latency_monitor_tests PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/build
)

target_link_libraries(latency_monitor_tests
    PRIVATE latency_monitor
)