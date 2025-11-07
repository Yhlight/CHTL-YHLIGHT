# CHTL Compiler Library
add_library(CHTL STATIC
    CHTLLexer/Lexer.cpp
    CHTLParser/Parser.cpp
    CHTLGenerator/Generator.cpp
    CHTLNode/ElementNode.cpp
    CHTLNode/NodeCloning.cpp
)

target_include_directories(CHTL PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
