#pragma once

#include "Token.h"
#include <string>
#include <string_view>
#include <vector>

class Lexer {
public:
    explicit Lexer(std::string_view source);
    Token scanToken();

private:
    char advance();
    char peek() const;
    char peekNext() const;
    bool isAtEnd() const;
    bool match(char expected);

    Token makeToken(TokenType type) const;
    Token errorToken(const char* message) const;
    Token string();
    Token number();
    Token identifier();
    TokenType identifierType();

    void skipWhitespace();

    std::string_view source_;
    size_t start_ = 0;
    size_t current_ = 0;
    size_t line_ = 1;
};
