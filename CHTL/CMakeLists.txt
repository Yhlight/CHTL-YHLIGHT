add_library(CHTLLexer
    CHTLLexer/Lexer.cpp
    CHTLLexer/Lexer.h
    CHTLLexer/Token.h
)

target_include_directories(CHTLLexer PUBLIC .)
