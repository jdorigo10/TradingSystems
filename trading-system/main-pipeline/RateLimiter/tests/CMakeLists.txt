add_executable(rate_limiter_tests main.cpp)

# Set output folder for this executable
set_target_properties(rate_limiter_tests PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/build
)

target_link_libraries(rate_limiter_tests
    PRIVATE rate_limiter
)