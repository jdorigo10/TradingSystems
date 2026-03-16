add_library(rate_limiter
    src/Limiter.cpp
)

target_include_directories(rate_limiter
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(rate_limiter
    PUBLIC common
)

add_subdirectory(tests)