add_library(libchtljs STATIC)

target_sources(libchtljs
    PRIVATE
    dummy.cpp
)

target_include_directories(libchtljs
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
    PRIVATE ${CMAKE_SOURCE_DIR}/src/SharedCore
)
