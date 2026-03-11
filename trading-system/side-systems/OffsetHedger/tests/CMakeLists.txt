add_executable(offset_hedger_tests main.cpp)

# Set output folder for this executable
set_target_properties(offset_hedger_tests PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/build
)

target_link_libraries(offset_hedger_tests
    PRIVATE offset_hedger
)