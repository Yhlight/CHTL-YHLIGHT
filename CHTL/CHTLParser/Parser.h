#pragma once

#include "CHTLLexer/Token.h"
#include "CHTLNode/BaseNode.h"
#include <vector>

class Parser {
public:
    explicit Parser(std::vector<Token> tokens);
    AstNode parse();

private:
    std::vector<Token> tokens;
    size_t current = 0;

    AstNode parseElement();
    AstNode parseStyleBlock();
    AstNode parseScriptBlock();
    AstNode parseComment();
    Token advance();
    Token peek();
    Token peekNext();
    bool isAtEnd();
};
