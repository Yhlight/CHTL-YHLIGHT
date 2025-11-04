#pragma once

#include "Token.h"
#include <string_view>

namespace CHTL {

class Lexer {
public:
    Lexer(std::string_view source);
    Token scanToken();

private:
    bool isAtEnd() const;
    char advance();
    char peek() const;
    char peekNext() const;
    bool match(char expected);

    Token makeToken(TokenType type) const;
    Token errorToken(const char* message) const;

    void skipWhitespace();

    Token string(char quote);
    Token number();
    Token identifier();
    TokenType identifierType();
    Token blockKeyword();

    std::string_view source_;
    size_t start_ = 0;
    size_t current_ = 0;
    size_t line_ = 1;
};

} // namespace CHTL
