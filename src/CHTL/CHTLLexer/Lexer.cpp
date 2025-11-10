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
        std::string value;
        while (position < source.length()) {
            if (source[position] == '\\' && position + 1 < source.length()) {
                // Handle escaped characters
                value += source.substr(start, position - start);
                value += source[position + 1];
                position += 2;
                start = position;
            } else if (source[position] == quoteType) {
                break; // End of string
            } else {
                position++;
            }
        }
        value += source.substr(start, position - start);

        if (position >= source.length() || source[position] != quoteType) {
            // Unterminated string
            return {TokenType::Unknown, ""};
        }
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
            if (position + 1 < source.length() && source[position + 1] == '=') {
                position += 2;
                return {TokenType::DoubleEqual, "=="};
            }
            position++;
            return {TokenType::Equal, "="};
        case '&':
            if (position + 1 < source.length() && source[position + 1] == '&') {
                position += 2;
                return {TokenType::DoubleAnd, "&&"};
            }
            position++;
            return {TokenType::Ampersand, "&"};
        case '[':
            position++;
            return {TokenType::OpenBracket, "["};
        case ']':
            position++;
            return {TokenType::CloseBracket, "]"};
        case '@':
            position++;
            return {TokenType::At, "@"};
        case '(':
            position++;
            return {TokenType::OpenParen, "("};
        case ')':
            position++;
            return {TokenType::CloseParen, ")"};
        case ',':
            position++;
            return {TokenType::Comma, ","};
        case '<':
            if (position + 1 < source.length() && source[position + 1] == '=') {
                position += 2;
                return {TokenType::LessEqual, "<="};
            }
            position++;
            return {TokenType::LessThan, "<"};
        case '>':
            if (position + 1 < source.length() && source[position + 1] == '=') {
                position += 2;
                return {TokenType::GreaterEqual, ">="};
            }
            position++;
            return {TokenType::GreaterThan, ">"};
        case '!':
            if (position + 1 < source.length() && source[position + 1] == '=') {
                position += 2;
                return {TokenType::NotEqual, "!="};
            }
            // Or handle single '!' if it has a meaning
            break;
        case '|':
            if (position + 1 < source.length() && source[position + 1] == '|') {
                position += 2;
                return {TokenType::DoubleOr, "||"};
            }
            break;
        case '+':
            position++;
            return {TokenType::Plus, "+"};
        case '-':
            position++;
            return {TokenType::Minus, "-"};
        case '*':
            if (position + 1 < source.length() && source[position + 1] == '*') {
                position += 2;
                return {TokenType::DoubleAsterisk, "**"};
            }
            position++;
            return {TokenType::Asterisk, "*"};
        case '/':
            position++;
            return {TokenType::Slash, "/"};
        case '%':
            position++;
            return {TokenType::Percent, "%"};
        case '?':
            position++;
            return {TokenType::QuestionMark, "?"};
    }

    // Handle identifiers and unquoted literals (including numbers and selectors)
    if (std::isalpha(currentChar) || std::isdigit(currentChar) || currentChar == '.' || currentChar == '#') {
        size_t start = position;
        while (position < source.length() && (std::isalnum(source[position]) || source[position] == '.' || source[position] == '#' || source[position] == '-' || source[position] == '_')) {
            position++;
        }
        std::string value = source.substr(start, position - start);
        if (value == "delete") return {TokenType::Delete, "delete"};
        if (value == "insert") return {TokenType::Insert, "insert"};
        if (value == "after") return {TokenType::After, "after"};
        if (value == "before") return {TokenType::Before, "before"};
        if (value == "replace") return {TokenType::Replace, "replace"};
        if (value == "at") return {TokenType::At, "at"};
        if (value == "top") return {TokenType::Top, "top"};
        if (value == "bottom") return {TokenType::Bottom, "bottom"};
        if (value == "if") return {TokenType::If, "if"};
        if (value == "else") return {TokenType::Else, "else"};
        if (value == "from") return {TokenType::From, "from"};
        if (value == "as") return {TokenType::As, "as"};
        if (value == "except") return {TokenType::Except, "except"};
        if (value == "use") return {TokenType::Use, "use"};
        return {TokenType::Identifier, value};
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
