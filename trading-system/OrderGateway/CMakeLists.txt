add_library(order_gateway
    src/OrderGateway.cpp
)

target_include_directories(order_gateway
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(order_gateway
    PUBLIC common
)

add_subdirectory(tests)