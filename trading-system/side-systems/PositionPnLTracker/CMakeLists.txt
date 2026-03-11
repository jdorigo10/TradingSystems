add_library(position_pnl_tracker
    src/PositionTracker.cpp
)

target_include_directories(position_pnl_tracker
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(position_pnl_tracker
    PUBLIC common
)

add_subdirectory(tests)