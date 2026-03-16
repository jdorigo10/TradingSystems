add_executable(strategy_engine_tests main.cpp)

# Set output folder for this executable
set_target_properties(strategy_engine_tests PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/build
)

target_link_libraries(strategy_engine_tests
    PRIVATE strategy_engine
)