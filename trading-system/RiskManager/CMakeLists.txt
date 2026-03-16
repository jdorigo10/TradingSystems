add_library(risk_manager
    src/RiskManager.cpp
)

target_include_directories(risk_manager
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(risk_manager
    PUBLIC common
)

add_subdirectory(tests)