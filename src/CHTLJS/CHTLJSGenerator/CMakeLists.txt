add_library(libchtljsgenerator STATIC)

target_sources(libchtljsgenerator
    PRIVATE
    Generator.cpp
)

target_include_directories(libchtljsgenerator
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
    PRIVATE ${CMAKE_SOURCE_DIR}/src/CHTLJS
)
