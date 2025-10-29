#include "Lexer.h"

namespace CHTL {

Lexer::Lexer(const std::string& source)
    : m_source(source), m_position(0), m_line(1), m_column(1), m_state(LexerState::NORMAL) {}

const std::string& Lexer::getSource() const {
    return m_source;
}

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

Token Lexer::makeToken(TokenType type, const std::string& value, size_t start_pos) {
    return Token{type, value, m_line, m_column, start_pos};
}

Token Lexer::nextToken() {
    size_t start_pos = m_position;

    while (true) {
        start_pos = m_position;
        while (isspace(currentChar())) {
            advance();
        }

        if (currentChar() == '/' && peekChar() == '/') {
            while (currentChar() != '\n' && currentChar() != '\0') {
                advance();
            }
            continue;
        }

        if (currentChar() == '/' && peekChar() == '*') {
            advance(2);
            while (currentChar() != '\0' && (currentChar() != '*' || peekChar() != '/')) {
                advance();
            }
            advance(2);
            continue;
        }

        if (currentChar() == '#') {
            std::string value;
            while (currentChar() != '\n' && currentChar() != '\0') {
                value += currentChar();
                advance();
            }
            return makeToken(TokenType::GeneratorComment, value, start_pos);
        }

        break;
    }

    char c = currentChar();
    start_pos = m_position;

    if (c == '\0') {
        return makeToken(TokenType::EndOfFile, "", start_pos);
    }

    if (c == '[') {
        if (m_source.substr(m_position, 10) == "[Template]") {
            advance(10);
            return makeToken(TokenType::TemplateKeyword, "[Template]", start_pos);
        }
        if (m_source.substr(m_position, 8) == "[Origin]") {
            advance(8);
            m_state = LexerState::IN_RAW_BLOCK;
            return makeToken(TokenType::OriginKeyword, "[Origin]", start_pos);
        }
        if (m_source.substr(m_position, 8) == "[Import]") {
            advance(8);
            return makeToken(TokenType::ImportKeyword, "[Import]", start_pos);
        }
    }

    if (m_state == LexerState::IN_RAW_BLOCK) {
        // The next token could be the type (e.g., @Html), a name, or the raw content
        if (currentChar() == '@' || isalpha(currentChar())) {
            std::string value;
            while (isalnum(currentChar()) || currentChar() == '@' || currentChar() == '-' || currentChar() == '_') {
                value += currentChar();
                advance();
            }
            return makeToken(TokenType::Identifier, value, start_pos);
        }
        return scanRawContent();
    }

    if (isalpha(c) || c == '@' || c == '.' || c == '#') {
        std::string value;
        while (isalnum(currentChar()) || currentChar() == '@' || currentChar() == '-' || currentChar() == '.' || currentChar() == '#') {
            value += currentChar();
            advance();
        }

        if (value == "inherit") {
            return makeToken(TokenType::InheritKeyword, value, start_pos);
        }
        if (value == "as") {
            return makeToken(TokenType::AsKeyword, value, start_pos);
        }
        if (value == "from") {
            return makeToken(TokenType::FromKeyword, value, start_pos);
        }

        return makeToken(TokenType::Identifier, value, start_pos);
    }

    if (c == '"' || c == '\'') {
        char quote_type = c;
        std::string value;
        advance();
        while (currentChar() != quote_type && currentChar() != '\0') {
            value += currentChar();
            advance();
        }
        advance();
        return makeToken(TokenType::String, value, start_pos);
    }

    switch (c) {
        case '{': advance(); return makeToken(TokenType::OpenBrace, "{", start_pos);
        case '}': advance(); return makeToken(TokenType::CloseBrace, "}", start_pos);
        case ';': advance(); return makeToken(TokenType::Semicolon, ";", start_pos);
        case ':': advance(); return makeToken(TokenType::Colon, ":", start_pos);
        case '=': advance(); return makeToken(TokenType::Colon, "=", start_pos);
    }

    std::string unexpected_char;
    unexpected_char += c;
    advance();
    return makeToken(TokenType::Unexpected, unexpected_char, start_pos);
}

Token Lexer::peek() {
    Lexer tempLexer = *this;
    return tempLexer.nextToken();
}

Token Lexer::scanRawContent() {
    size_t start_pos = m_position;

    // Skip whitespace and find the opening brace
    while (isspace(currentChar())) {
        advance();
    }

    // It's possible to have an identifier (the name) before the brace
    // This is now handled in nextToken()


    if (currentChar() != '{') {
        return makeToken(TokenType::Unexpected, std::string(1, currentChar()), start_pos);
    }
    advance(); // Eat '{'

    std::string content;
    int brace_count = 1;
    start_pos = m_position;

    while (brace_count > 0 && currentChar() != '\0') {
        if (currentChar() == '{') {
            brace_count++;
        } else if (currentChar() == '}') {
            brace_count--;
        }

        if (brace_count > 0) {
            content += currentChar();
            advance();
        }
    }

    advance(); // Eat '}'
    m_state = LexerState::NORMAL; // Revert state
    return makeToken(TokenType::RAW_CONTENT, content, start_pos);
}

} // namespace CHTL
