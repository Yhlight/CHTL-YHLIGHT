add_library(libchtl STATIC)

target_sources(libchtl PRIVATE
    Dummy.cpp
    CHTLLexer/Lexer.cpp
    CHTLParser/Parser.cpp
    CHTLGenerator/Generator.cpp
)

target_include_directories(libchtl PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(libchtl PRIVATE libsharedcore)
