#pragma once

#include "Token.h"
#include <string>

namespace CHTL {

class Lexer {
public:
    Lexer(const std::string& source);

    Token nextToken();
    Token peek();

private:
    std::string m_source;
    size_t m_position;
    int m_line;
    int m_column;

    char currentChar();
    char peekChar();
    void advance(int count = 1);
    Token makeToken(TokenType type, const std::string& value);
};

} // namespace CHTL
