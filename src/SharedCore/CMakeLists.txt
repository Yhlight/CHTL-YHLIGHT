add_library(libsharedcore STATIC)

target_sources(libsharedcore
    PRIVATE
    SymbolTable.cpp
)

target_include_directories(libsharedcore
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
    PUBLIC ${CMAKE_SOURCE_DIR}/src
)
