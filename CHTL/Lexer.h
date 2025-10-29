#pragma once

#include "Token.h"
#include <string>

namespace CHTL {

enum class LexerState {
    NORMAL,
    IN_RAW_BLOCK
};

class Lexer {
public:
    Lexer(const std::string& source);

    Token nextToken();
    Token peek();

    const std::string& getSource() const;
    size_t getPosition() const { return m_position; }
    void setPosition(size_t position) { m_position = position; }

private:
    const std::string& m_source;
    size_t m_position;
    int m_line;
    int m_column;
    LexerState m_state;

    char currentChar();
    char peekChar();
    void advance(int count = 1);
    Token makeToken(TokenType type, const std::string& value, size_t start_pos);
    Token scanRawContent();
    TokenType isKeyword(const std::string& value);
};

} // namespace CHTL
