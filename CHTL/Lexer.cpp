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

char Lexer::peekChar() {
    if (m_position + 1 >= m_source.length()) {
        return '\0';
    }
    return m_source[m_position + 1];
}

void Lexer::advance(int count) {
    for (int i = 0; i < count; ++i) {
        if (currentChar() == '\n') {
            m_line++;
            m_column = 1;
        } else {
            m_column++;
        }
        m_position++;
    }
}

Token Lexer::makeToken(TokenType type, const std::string& value) {
    return Token{type, value, m_line, m_column, (int)m_position};
}

Token Lexer::nextToken() {
    while (true) {
        while (isspace(currentChar())) {
            advance();
        }

        if (currentChar() == '/' && peekChar() == '/') {
            while (currentChar() != '\n' && currentChar() != '\0') {
                advance();
            }
            continue; // Restart the loop to skip whitespace and find next token
        }

        if (currentChar() == '/' && peekChar() == '*') {
            advance(); // Skip '/'
            advance(); // Skip '*'
            while (currentChar() != '\0' && (currentChar() != '*' || peekChar() != '/')) {
                advance();
            }
            advance(); // Skip '*'
            advance(); // Skip '/'
            continue; // Restart the loop
        }

        if (currentChar() == '#') {
            std::string value;
            while (currentChar() != '\n' && currentChar() != '\0') {
                value += currentChar();
                advance();
            }
            return makeToken(TokenType::GeneratorComment, value);
        }

        break; // No more whitespace or comments, exit the loop
    }

    char c = currentChar();

    if (c == '\0') {
        return makeToken(TokenType::EndOfFile, "");
    }

    if (c == '[') {
        if (m_source.substr(m_position, 10) == "[Template]") {
            advance(10);
            return makeToken(TokenType::TemplateKeyword, "[Template]");
        }
        if (m_source.substr(m_position, 8) == "[Origin]") {
            advance(8);
            return makeToken(TokenType::OriginKeyword, "[Origin]");
        }
    }

    if (isalpha(c) || c == '@') {
        std::string value;
        while (isalnum(currentChar()) || currentChar() == '@' || currentChar() == '-') {
            value += currentChar();
            advance();
        }

        if (value == "inherit") {
            return makeToken(TokenType::InheritKeyword, value);
        }

        return makeToken(TokenType::Identifier, value);
    }

    if (c == '"' || c == '\'') {
        char quote_type = c;
        std::string value;
        advance(); // Skip the opening quote
        while (currentChar() != quote_type && currentChar() != '\0') {
            value += currentChar();
            advance();
        }
        advance(); // Skip the closing quote
        return makeToken(TokenType::String, value);
    }

    switch (c) {
        case '{': advance(); return makeToken(TokenType::OpenBrace, "{");
        case '}': advance(); return makeToken(TokenType::CloseBrace, "}");
        case ';': advance(); return makeToken(TokenType::Semicolon, ";");
        case ':': advance(); return makeToken(TokenType::Colon, ":");
        case '=': advance(); return makeToken(TokenType::Colon, "="); // CE Equivalence
    }

    std::string unexpected_char;
    unexpected_char += c;
    advance();
    return makeToken(TokenType::Unexpected, unexpected_char);
}

Token Lexer::peek() {
    Lexer tempLexer = *this;
    return tempLexer.nextToken();
}

} // namespace CHTL
