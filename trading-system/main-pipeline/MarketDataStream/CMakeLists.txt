add_library(market_data_stream
    src/DataStream.cpp
)

target_include_directories(market_data_stream
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(market_data_stream
    PUBLIC common
)

add_subdirectory(tests)