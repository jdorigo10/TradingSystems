add_library(market_data_manager
    src/DataManager.cpp
)

target_include_directories(market_data_manager
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(market_data_manager
    PUBLIC common
)

add_subdirectory(tests)