#pragma once

#include "Token.h"
#include <string>
#include <vector>

class Lexer {
public:
    explicit Lexer(std::string source);
    std::vector<Token> tokenize();

private:
    std::string source;
    size_t current = 0;
    size_t line = 1;
    size_t column = 1;

    Token nextToken();
    char advance();
    char peek();
    bool isAtEnd();
    void skipWhitespace();
};
