add_library(order_tracker
    src/OrderTracker.cpp
)

target_include_directories(order_tracker
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(order_tracker
    PUBLIC common
)

add_subdirectory(tests)