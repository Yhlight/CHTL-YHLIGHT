add_library(libchtl INTERFACE)

target_include_directories(libchtl INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/../Util
)

target_link_libraries(libchtl INTERFACE
    CHTLLexer
    CHTLParser
    CHTLGenerator
    Util
)

add_subdirectory(CHTL)
add_subdirectory(CHTLJS)
add_subdirectory(Shared)
add_subdirectory(Util)
