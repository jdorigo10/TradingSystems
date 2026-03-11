add_executable(matching_engine_tests main.cpp)

# Set output folder for this executable
set_target_properties(matching_engine_tests PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/build
)

target_link_libraries(matching_engine_tests
    PRIVATE matching_engine
)