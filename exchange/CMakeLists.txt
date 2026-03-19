add_subdirectory(MatchingEngine)

add_library(exchange
    src/Exchange.cpp
)

target_include_directories(exchange
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(exchange
    PUBLIC 
        common
        matching_engine
)