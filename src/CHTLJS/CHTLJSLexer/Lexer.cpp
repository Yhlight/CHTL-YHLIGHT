#include "Lexer.h"
#include <unordered_map>

namespace CHTLJS {

Lexer::Lexer(std::string_view source) : source_(source) {}

bool Lexer::isAtEnd() const {
    return current_ >= source_.length();
}

char Lexer::advance() {
    return source_[current_++];
}

char Lexer::peek() const {
    if (isAtEnd()) return '\0';
    return source_[current_];
}

char Lexer::peekNext() const {
    if (current_ + 1 >= source_.length()) return '\0';
    return source_[current_ + 1];
}

bool Lexer::match(char expected) {
    if (isAtEnd() || source_[current_] != expected) {
        return false;
    }
    current_++;
    return true;
}

Token Lexer::makeToken(TokenType type) const {
    return {type, source_.substr(start_, current_ - start_), line_};
}

Token Lexer::errorToken(const char* message) const {
    return {TokenType::UNRECOGNIZED, message, line_};
}

void Lexer::skipWhitespace() {
    for (;;) {
        char c = peek();
        switch (c) {
            case ' ':
            case '\r':
            case '\t':
                advance();
                break;
            case '\n':
                line_++;
                advance();
                break;
            case '/':
                if (peekNext() == '/') {
                    while (peek() != '\n' && !isAtEnd()) advance();
                } else if (peekNext() == '*') {
                    advance();
                    advance();
                    while (!(peek() == '*' && peekNext() == '/') && !isAtEnd()) {
                        if (peek() == '\n') line_++;
                        advance();
                    }
                    if (!isAtEnd()) advance();
                    if (!isAtEnd()) advance();
                }
                else {
                    return;
                }
                break;
            default:
                return;
        }
    }
}

Token Lexer::string() {
    char quote_type = source_[start_];
    while (peek() != quote_type && !isAtEnd()) {
        if (peek() == '\n') line_++;
        advance();
    }

    if (isAtEnd()) return errorToken("Unterminated string.");

    advance();
    return makeToken(TokenType::STRING);
}

bool isDigit(char c) {
    return c >= '0' && c <= '9';
}

bool isAlpha(char c) {
    return (c >= 'a' && c <= 'z') ||
           (c >= 'A' && c <= 'Z') ||
            c == '_';
}

Token Lexer::number() {
    while (isDigit(peek())) advance();

    if (peek() == '.' && isDigit(peekNext())) {
        advance();
        while (isDigit(peek())) advance();
    }

    return makeToken(TokenType::NUMBER);
}

TokenType Lexer::identifierType() {
    std::string_view lexeme = source_.substr(start_, current_ - start_);
    static const std::unordered_map<std::string_view, TokenType> keywords = {
        {"Listen", TokenType::KEYWORD_LISTEN},
        {"Delegate", TokenType::KEYWORD_DELEGATE},
        {"Vir", TokenType::KEYWORD_VIR},
        {"Animate", TokenType::KEYWORD_ANIMATE},
        {"Router", TokenType::KEYWORD_ROUTER},
    };

    auto it = keywords.find(lexeme);
    if (it != keywords.end()) {
        return it->second;
    }

    return TokenType::IDENTIFIER;
}

Token Lexer::identifier() {
    while (isAlpha(peek()) || isDigit(peek()) || peek() == '-') advance();
    return makeToken(identifierType());
}


Token Lexer::scanToken() {
    skipWhitespace();
    start_ = current_;

    if (isAtEnd()) return makeToken(TokenType::END_OF_FILE);

    char c = advance();

    if (isAlpha(c)) return identifier();
    if (isDigit(c)) return number();


    switch (c) {
        case '#': return makeToken(TokenType::HASH);
        case '(': return makeToken(TokenType::LEFT_PAREN);
        case ')': return makeToken(TokenType::RIGHT_PAREN);
        case '{': return match('{') ? makeToken(TokenType::LEFT_DOUBLE_BRACE) : makeToken(TokenType::LEFT_BRACE);
        case '}': return match('}') ? makeToken(TokenType::RIGHT_DOUBLE_BRACE) : makeToken(TokenType::RIGHT_BRACE);
        case '[': return makeToken(TokenType::LEFT_BRACKET);
        case ']': return makeToken(TokenType::RIGHT_BRACKET);
        case ';': return makeToken(TokenType::SEMICOLON);
        case ',': return makeToken(TokenType::COMMA);
        case '.': return makeToken(TokenType::DOT);
        case '-': return match('>') ? makeToken(TokenType::ARROW) : makeToken(TokenType::MINUS);
        case '&': return match('-') && match('>') ? makeToken(TokenType::AMPERSAND_ARROW_DASH) : errorToken("Unexpected character.");
        case '+': return makeToken(TokenType::PLUS);
        case '/': return makeToken(TokenType::SLASH);
        case '*': return makeToken(TokenType::STAR);
        case '!': return makeToken(match('=') ? TokenType::BANG_EQUAL : TokenType::BANG);
        case '=': return makeToken(match('=') ? TokenType::EQUAL_EQUAL : TokenType::EQUAL);
        case '<': return makeToken(match('=') ? TokenType::LESS_EQUAL : TokenType::LESS);
        case '>': return makeToken(match('=') ? TokenType::GREATER_EQUAL : TokenType::GREATER);
        case '"':
        case '\'': return string();
        case ':': return makeToken(TokenType::COLON);
    }

    return errorToken("Unexpected character.");
}

}
