#include "Lexer.h"
#include <unordered_map>

Lexer::Lexer(std::string_view source) : source_(source) {}

Token Lexer::peekToken() const {
    Lexer lexer = *this;
    return lexer.scanToken();
}

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
    return {type, source_.substr(start_, current_ - start_), line_, start_};
}

Token Lexer::errorToken(const char* message) const {
    return {TokenType::UNRECOGNIZED, message, line_, start_};
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
                    advance(); // Consume '/'
                    advance(); // Consume '*'
                    while (!(peek() == '*' && peekNext() == '/') && !isAtEnd()) {
                        if (peek() == '\n') line_++;
                        advance();
                    }
                    if (!isAtEnd()) advance(); // Consume '*'
                    if (!isAtEnd()) advance(); // Consume '/'
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

    advance(); // The closing quote.
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
    // A simple keyword checker
    std::string_view lexeme = source_.substr(start_, current_ - start_);
    static const std::unordered_map<std::string_view, TokenType> keywords = {
        {"style", TokenType::KEYWORD_STYLE},
        {"script", TokenType::KEYWORD_SCRIPT},
        {"text", TokenType::KEYWORD_TEXT},
        {"[Template]", TokenType::KEYWORD_TEMPLATE},
        {"[Custom]", TokenType::KEYWORD_CUSTOM},
        {"[Origin]", TokenType::KEYWORD_ORIGIN},
        {"[Import]", TokenType::KEYWORD_IMPORT},
        {"[Namespace]", TokenType::KEYWORD_NAMESPACE},
        {"[Configuration]", TokenType::KEYWORD_CONFIGURATION},
        {"from", TokenType::KEYWORD_FROM},
        {"as", TokenType::KEYWORD_AS},
        {"@Style", TokenType::BLOCK_AT_STYLE},
        {"@Element", TokenType::BLOCK_AT_ELEMENT},
        {"@Var", TokenType::BLOCK_AT_VAR},
        {"@Html", TokenType::BLOCK_AT_HTML},
        {"@JavaScript", TokenType::BLOCK_AT_JAVASCRIPT},
        {"@Chtl", TokenType::BLOCK_AT_CHTL},
        {"@Config", TokenType::BLOCK_AT_CONFIG}
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
        case '(': return makeToken(TokenType::LEFT_PAREN);
        case ')': return makeToken(TokenType::RIGHT_PAREN);
        case '{': return makeToken(TokenType::LEFT_BRACE);
        case '}': return makeToken(TokenType::RIGHT_BRACE);
        case '[': return makeToken(TokenType::LEFT_BRACKET);
        case ']': return makeToken(TokenType::RIGHT_BRACKET);
        case ';': return makeToken(TokenType::SEMICOLON);
        case ',': return makeToken(TokenType::COMMA);
        case '.': return makeToken(TokenType::DOT);
        case '-': return makeToken(TokenType::MINUS);
        case '+': return makeToken(TokenType::PLUS);
        case '/': return makeToken(TokenType::SLASH);
        case '*': return makeToken(TokenType::STAR);
        case '!': return makeToken(match('=') ? TokenType::BANG_EQUAL : TokenType::BANG);
        case '=': return makeToken(match('=') ? TokenType::EQUAL_EQUAL : TokenType::EQUAL);
        case '<': return makeToken(match('=') ? TokenType::LESS_EQUAL : TokenType::LESS);
        case '>': return makeToken(match('=') ? TokenType::GREATER_EQUAL : TokenType::GREATER);
        case '"':
        case '\'': return string();
        case '#': return makeToken(TokenType::HASH_COMMENT);
        case ':': return makeToken(match(':') ? TokenType::COLON_COLON : TokenType::COLON);

    }

    return errorToken("Unexpected character.");
}
