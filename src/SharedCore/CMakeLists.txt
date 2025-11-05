add_library(libsharedcore STATIC)

target_sources(libsharedcore PRIVATE
    Dummy.cpp
    SymbolTable.cpp
)

target_include_directories(libsharedcore PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)
