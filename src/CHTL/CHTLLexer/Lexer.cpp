#include "Lexer.h"
#include <cctype>

namespace CHTL {

Lexer::Lexer(const std::string& source) : source(source), position(0) {}

void Lexer::skipWhitespaceAndComments() {
    while (position < source.length()) {
        if (std::isspace(source[position])) {
            position++;
            continue;
        }

        if (source[position] == '/' && position + 1 < source.length()) {
            if (source[position + 1] == '/') {
                // Single-line comment
                position += 2;
                while (position < source.length() && source[position] != '\n') {
                    position++;
                }
                continue;
            } else if (source[position + 1] == '*') {
                // Multi-line comment
                position += 2;
                while (position + 1 < source.length() && (source[position] != '*' || source[position + 1] != '/')) {
                    position++;
                }
                if (position + 1 < source.length()) {
                    position += 2;
                }
                continue;
            }
        }
        break; // Not whitespace or a comment
    }
}


Token Lexer::getNextToken() {
    if (hasPeeked) {
        hasPeeked = false;
        return currentToken;
    }

    skipWhitespaceAndComments();

    if (position >= source.length()) {
        return {TokenType::Eof, ""};
    }

    char currentChar = source[position];

    // Handle strings
    if (currentChar == '\'' || currentChar == '"') {
        char quoteType = currentChar;
        position++; // Consume the opening quote
        size_t start = position;
        while (position < source.length() && source[position] != quoteType) {
            position++;
        }

        if (position >= source.length() || source[position] != quoteType) {
            // Unterminated string
            return {TokenType::Unknown, ""};
        }

        std::string value = source.substr(start, position - start);
        position++; // Consume the closing quote
        return {TokenType::String, value};
    }

    // Handle symbols
    switch (currentChar) {
        case '{':
            position++;
            return {TokenType::OpenBrace, "{"};
        case '}':
            position++;
            return {TokenType::CloseBrace, "}"};
        case ':':
            position++;
            return {TokenType::Colon, ":"};
        case ';':
            position++;
            return {TokenType::Semicolon, ";"};
        case '=':
            position++;
            return {TokenType::Equal, "="};
    }

    // Handle identifiers and unquoted literals (including numbers and selectors)
    if (std::isalpha(currentChar) || std::isdigit(currentChar) || currentChar == '.') {
        size_t start = position;
        while (position < source.length() && (std::isalnum(source[position]) || source[position] == '.')) {
            position++;
        }
        return {TokenType::Identifier, source.substr(start, position - start)};
    }

    // For now, return Unknown for anything else
    return {TokenType::Unknown, std::string(1, source[position++])};
}

Token Lexer::peek() {
    if (!hasPeeked) {
        currentToken = getNextToken();
        hasPeeked = true;
    }
    return currentToken;
}

} // namespace CHTL
