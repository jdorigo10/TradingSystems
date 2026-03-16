add_library(position_tracker
    src/PositionTracker.cpp
)

target_include_directories(position_tracker
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(position_tracker
    PUBLIC common
)

add_subdirectory(tests)