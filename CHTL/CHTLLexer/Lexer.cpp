#include "Lexer.h"
#include <iostream>
#include <cctype>

Lexer::Lexer(std::string source) : source(std::move(source)) {}

std::vector<Token> Lexer::tokenize() {
    std::vector<Token> tokens;
    Token token = nextToken();
    while(token.type != TokenType::EndOfFile) {
        tokens.push_back(token);
        token = nextToken();
    }
    tokens.push_back(token); // push the EOF token
    return tokens;
}

Token Lexer::nextToken() {
    skipWhitespace();

    if (isAtEnd()) {
        return {TokenType::EndOfFile, "", line, column};
    }

    char c = advance();

    if (c == '/' && peek() == '/') {
        std::string comment;
        while (peek() != '\n' && !isAtEnd()) {
            comment += advance();
        }
        return {TokenType::LineComment, comment, line, column};
    }

    if (c == '/' && peek() == '*') {
        std::string comment;
        advance();
        while (!isAtEnd()) {
            if (peek() == '*' && source[current + 1] == '/') {
                break;
            }
            comment += advance();
        }
        advance();
        advance();
        return {TokenType::BlockComment, comment, line, column};
    }

    if (c == '#') {
        std::string comment;
        while (peek() != '\n' && !isAtEnd()) {
            comment += advance();
        }
        return {TokenType::GeneratorComment, comment, line, column};
    }

    if (c == '"' || c == '\'') {
        char quote_type = c;
        std::string value;
        while (peek() != quote_type && !isAtEnd()) {
            value += advance();
        }
        advance(); // consume the closing quote
        return {TokenType::StringLiteral, value, line, column};
    }

    if (isalpha(c)) {
        std::string value;
        value += c;
        while (isalnum(peek())) {
            value += advance();
        }
        return {TokenType::Identifier, value, line, column};
    }

    if (isdigit(c)) {
        std::string value;
        value += c;
        while (isdigit(peek())) {
            value += advance();
        }
        return {TokenType::NumericLiteral, value, line, column};
    }

    switch (c) {
        case '{': return {TokenType::OpenBrace, "{", line, column};
        case '}': return {TokenType::CloseBrace, "}", line, column};
        case '(': return {TokenType::OpenParen, "(", line, column};
        case ')': return {TokenType::CloseParen, ")", line, column};
        case ';': return {TokenType::Semicolon, ";", line, column};
        case ':': return {TokenType::Colon, ":", line, column};
        case '=': return {TokenType::Equals, "=", line, column};
        case ',': return {TokenType::Comma, ",", line, column};
        case '@': return {TokenType::At, "@", line, column};
        case '&': return {TokenType::Ampersand, "&", line, column};
        case '+': return {TokenType::Plus, "+", line, column};
        case '-': return {TokenType::Minus, "-", line, column};
        case '*': return {TokenType::Asterisk, "*", line, column};
        case '/': return {TokenType::Slash, "/", line, column};
        case '%': return {TokenType::Percent, "%", line, column};
    }

    return {TokenType::Unknown, std::string(1, c), line, column};
}

char Lexer::advance() {
    column++;
    return source[current++];
}

char Lexer::peek() {
    if (isAtEnd()) return '\0';
    return source[current];
}

bool Lexer::isAtEnd() {
    return current >= source.length();
}

void Lexer::skipWhitespace() {
    while (!isAtEnd()) {
        char c = peek();
        switch (c) {
            case ' ':
            case '\r':
            case '\t':
                advance();
                break;
            case '\n':
                line++;
                column = 1;
                advance();
                break;
            default:
                return;
        }
    }
}
