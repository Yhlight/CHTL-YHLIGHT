#pragma once

#include <string>
#include <string_view>

enum class TokenType {
    // Single-character tokens.
    LEFT_PAREN, RIGHT_PAREN,
    LEFT_BRACE, RIGHT_BRACE,
    LEFT_BRACKET, RIGHT_BRACKET,
    COMMA, DOT, MINUS, PLUS,
    SEMICOLON, SLASH, STAR, COLON,
    EQUAL, HASH,

    // One or two character tokens.
    BANG, BANG_EQUAL,
    EQUAL_EQUAL,
    GREATER, GREATER_EQUAL,
    LESS, LESS_EQUAL,
    ARROW,
    AMPERSAND_ARROW_DASH,
    LEFT_DOUBLE_BRACE,
    RIGHT_DOUBLE_BRACE,

    // Literals.
    IDENTIFIER, STRING, NUMBER,

    // Keywords.
    KEYWORD_LISTEN,
    KEYWORD_DELEGATE,
    KEYWORD_VIR,
    KEYWORD_ANIMATE,
    KEYWORD_ROUTER,

    // Misc
    UNRECOGNIZED,
    END_OF_FILE
};

struct Token {
    TokenType type;
    std::string_view lexeme;
    size_t line;
};
