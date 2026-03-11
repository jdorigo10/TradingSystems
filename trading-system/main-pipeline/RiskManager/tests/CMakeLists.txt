add_executable(risk_manager_tests main.cpp)

# Set output folder for this executable
set_target_properties(risk_manager_tests PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/build
)

target_link_libraries(risk_manager_tests
    PRIVATE risk_manager
)