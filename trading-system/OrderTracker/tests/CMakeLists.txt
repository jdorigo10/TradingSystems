add_executable(order_tracker_tests main.cpp)

# Set output folder for this executable
set_target_properties(order_tracker_tests PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/build
)

target_link_libraries(order_tracker_tests
    PRIVATE order_tracker
)