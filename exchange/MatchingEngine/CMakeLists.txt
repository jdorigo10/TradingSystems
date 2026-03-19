add_library(matching_engine
    src/MatchingEngine.cpp
)

target_include_directories(matching_engine
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(matching_engine
    PUBLIC common
)

add_subdirectory(tests)