#include "Lexer.h"
#include <cctype>

namespace CHTL {

Lexer::Lexer(const std::string& source) : source(source), position(0) {}

Token Lexer::getNextToken() {
    // Skip whitespace
    while (position < source.length() && std::isspace(source[position])) {
        position++;
    }

    if (position >= source.length()) {
        return {TokenType::Eof, ""};
    }

    char currentChar = source[position];

    if (currentChar == '\'') {
        position++; // Consume the opening quote
        size_t start = position;
        while (position < source.length() && source[position] != '\'') {
            position++;
        }
        std::string value = source.substr(start, position - start);
        if (position < source.length() && source[position] == '\'') {
            position++; // Consume the closing quote
        }
        return {TokenType::String, value};
    }

    // For now, return Unknown for anything else
    return {TokenType::Unknown, std::string(1, source[position++])};
}

} // namespace CHTL
