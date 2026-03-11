add_library(offset_hedger
    src/Hedger.cpp
)

target_include_directories(offset_hedger
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(offset_hedger
    PUBLIC common
)

add_subdirectory(tests)