add_executable(position_pnl_tracker_tests main.cpp)

# Set output folder for this executable
set_target_properties(position_pnl_tracker_tests PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/build
)

target_link_libraries(position_pnl_tracker_tests
    PRIVATE position_pnl_tracker
)