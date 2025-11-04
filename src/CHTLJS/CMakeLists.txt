add_library(libchtljs STATIC)

target_sources(libchtljs PRIVATE
    Dummy.cpp
)

target_include_directories(libchtljs PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(libchtljs PRIVATE libsharedcore)
