add_library(order_book_builder
    src/BookBuilder.cpp
)

target_include_directories(order_book_builder
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(order_book_builder
    PUBLIC common
)

add_subdirectory(tests)