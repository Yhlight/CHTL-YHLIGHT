#pragma once

#include "Token.h"
#include <string>
#include <vector>

class Lexer {
public:
    Lexer(const std::string& source);
    std::vector<Token> tokenize();

private:
    std::string source;
    size_t start = 0;
    size_t current = 0;

    Token nextToken();
    bool isAtEnd();
    char advance();
    char peek();
    char peekNext();
    void skipWhitespace();
    Token string();
    Token identifier();
    Token valueLiteral();
};
