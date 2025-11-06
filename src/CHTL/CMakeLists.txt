# CHTL Compiler Library
add_library(CHTL STATIC
    CHTLLexer/Lexer.cpp
    CHTLParser/Parser.cpp
)

target_include_directories(CHTL PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
