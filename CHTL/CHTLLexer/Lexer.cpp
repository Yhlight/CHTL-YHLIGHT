#include "Lexer.h"
#include <iostream>
#include <unordered_map>

static const std::unordered_map<std::string, TokenType> keywords = {
    {"text", TokenType::Text},
};

Lexer::Lexer(const std::string& source) : source(source) {}

std::vector<Token> Lexer::tokenize() {
    std::vector<Token> tokens;
    while (!isAtEnd()) {
        start = current;
        Token token = nextToken();
        if (token.type != TokenType::Unknown) {
            tokens.push_back(token);
        }
    }
    tokens.push_back({TokenType::EndOfFile, ""});
    return tokens;
}

Token Lexer::nextToken() {
    skipWhitespace();
    start = current;

    if (isAtEnd()) return {TokenType::EndOfFile, ""};

    char c = advance();

    if (isalpha(c)) return identifier();
    if (c == '"') return string();

    switch (c) {
        case '{': return {TokenType::LBrace, "{"};
        case '}': return {TokenType::RBrace, "}"};
    }

    return {TokenType::Unknown, std::string(1, c)};
}

bool Lexer::isAtEnd() {
    return current >= source.length();
}

char Lexer::advance() {
    return source[current++];
}

char Lexer::peek() {
    if (isAtEnd()) return '\0';
    return source[current];
}

void Lexer::skipWhitespace() {
    while (true) {
        char c = peek();
        switch (c) {
            case ' ':
            case '\r':
            case '\t':
            case '\n':
                advance();
                break;
            default:
                return;
        }
    }
}

Token Lexer::string() {
    while (peek() != '"' && !isAtEnd()) {
        advance();
    }

    if (isAtEnd()) {
        // Unterminated string.
        return {TokenType::Unknown, "Unterminated string."};
    }

    // The closing ".
    advance();

    return {TokenType::String, source.substr(start + 1, current - start - 2)};
}

Token Lexer::identifier() {
    while (isalnum(peek())) advance();

    std::string text = source.substr(start, current - start);
    auto it = keywords.find(text);
    if (it == keywords.end()) {
        return {TokenType::Unknown, text};
    }

    return {it->second, text};
}
