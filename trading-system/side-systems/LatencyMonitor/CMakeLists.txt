add_library(latency_monitor
    src/LatencyMonitor.cpp
)

target_include_directories(latency_monitor
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(latency_monitor
    PUBLIC common
)

add_subdirectory(tests)