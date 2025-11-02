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
    Token advance();
    Token peek();
    bool isAtEnd();
};
