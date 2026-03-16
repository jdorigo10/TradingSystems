add_executable(order_gateway_tests main.cpp)

# Set output folder for this executable
set_target_properties(order_gateway_tests PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/build
)

target_link_libraries(order_gateway_tests
    PRIVATE order_gateway
)