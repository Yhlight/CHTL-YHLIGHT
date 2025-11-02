add_library(CHTLJSParser
    Parser.cpp
)

target_include_directories(CHTLJSParser PUBLIC .)
target_link_libraries(CHTLJSParser PUBLIC CHTLJSLexer CHTLJSNode)
