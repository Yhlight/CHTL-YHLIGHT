#ifndef CHTL_LEXER_H
#define CHTL_LEXER_H

#include "Token.h"
#include <string_view>
#include <vector>

namespace CHTL {

class Lexer {
public:
    Lexer(std::string_view source);
    std::vector<Token> scanTokens();

private:
    std::string_view source_;
    std::vector<Token> tokens_;
    size_t start_ = 0;
    size_t current_ = 0;
    int line_ = 1;

    bool isAtEnd();
    void scanToken();
    char advance();
    bool match(char expected);
    char peek();
    char peekNext();
    void addToken(TokenType type);
    void addToken(TokenType type, std::string_view lexeme);

    void string(char quote);
    void number();
    void identifier();
    void blockKeyword();

    // Helper functions for identifying character types
    static bool isDigit(char c);
    static bool isAlpha(char c);
    static bool isAlphaNumeric(char c);
};

} // namespace CHTL

#endif // CHTL_LEXER_H
