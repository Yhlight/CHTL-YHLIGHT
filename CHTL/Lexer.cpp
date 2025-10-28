#include "Lexer.h"

namespace CHTL {

Lexer::Lexer(const std::string& source)
    : m_source(source), m_position(0), m_line(1), m_column(1) {}

char Lexer::currentChar() {
    if (m_position >= m_source.length()) {
        return '\0';
    }
    return m_source[m_position];
}

void Lexer::advance() {
    if (currentChar() == '\n') {
        m_line++;
        m_column = 1;
    } else {
        m_column++;
    }
    m_position++;
}

Token Lexer::makeToken(TokenType type, const std::string& value) {
    return Token{type, value, m_line, m_column};
}

Token Lexer::nextToken() {
    while (isspace(currentChar())) {
        advance();
    }

    char c = currentChar();

    if (c == '\0') {
        return makeToken(TokenType::EndOfFile, "");
    }

    if (isalpha(c)) {
        std::string value;
        while (isalnum(currentChar())) {
            value += currentChar();
            advance();
        }
        return makeToken(TokenType::Identifier, value);
    }

    switch (c) {
        case '{': advance(); return makeToken(TokenType::OpenBrace, "{");
        case '}': advance(); return makeToken(TokenType::CloseBrace, "}");
        case ';': advance(); return makeToken(TokenType::Semicolon, ";");
        case ':': advance(); return makeToken(TokenType::Colon, ":");
        case '=': advance(); return makeToken(TokenType::Equals, "=");
    }

    std::string unexpected_char;
    unexpected_char += c;
    advance();
    return makeToken(TokenType::Unexpected, unexpected_char);
}

} // namespace CHTL
