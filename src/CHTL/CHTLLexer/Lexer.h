#pragma once

#include "Token.h"
#include <string>
#include <vector>
#include <string_view>
#include <unordered_map>

namespace CHTL {

class Lexer {
public:
    explicit Lexer(std::string_view source);
    std::vector<Token> tokenize();

private:
    void scanToken();
    char advance();
    bool match(char expected);
    char peek() const;
    char peekNext() const;
    bool isAtEnd() const;

    void addToken(TokenType type);
    void addToken(TokenType type, const std::string& literal);

    void stringLiteral();
    void numberLiteral();
    void identifierOrKeyword();
    // CHTL allows unquoted literals in many places, which can be complex.
    // For now, we will treat them like identifiers and the parser will give them context.
    void singleLineComment();
    void multiLineComment();
    void generatorComment();


    std::string_view m_source;
    std::vector<Token> m_tokens;
    size_t m_start = 0;
    size_t m_current = 0;
    int m_line = 1;
    int m_column = 1;

    static const std::unordered_map<std::string, TokenType> keywords;
};

} // namespace CHTL
