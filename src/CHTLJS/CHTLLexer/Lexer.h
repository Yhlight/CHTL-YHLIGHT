#pragma once

#include "Token.h"
#include <string_view>
#include <vector>

namespace CHTLJS {

class Lexer {
public:
    Lexer(std::string_view source);
    std::vector<Token> scanTokens();

private:
    std::string_view m_source;
    std::vector<Token> m_tokens;
    int m_start = 0;
    int m_current = 0;
    int m_line = 1;

    bool isAtEnd();
    char advance();
    char peek();
    char peekNext();
    bool match(char expected);
    void addToken(TokenType type);
    void addToken(TokenType type, std::string_view lexeme);
    void scanToken();
    void identifier();
    void number();
    void string();
};

} // namespace CHTLJS
