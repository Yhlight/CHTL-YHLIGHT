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
    EQUAL, AT,

    // One or two character tokens.
    BANG, BANG_EQUAL,
    EQUAL_EQUAL,
    GREATER, GREATER_EQUAL,
    LESS, LESS_EQUAL,
    ARROW_DASH,
    COLON_COLON,

    // Literals.
    IDENTIFIER, STRING, NUMBER,

    // Keywords.
    KEYWORD_STYLE,
    KEYWORD_SCRIPT,
    KEYWORD_TEXT,
    KEYWORD_TEMPLATE,
    KEYWORD_CUSTOM,
    KEYWORD_ORIGIN,
    KEYWORD_IMPORT,
    KEYWORD_NAMESPACE,
    KEYWORD_CONFIGURATION,
    KEYWORD_FROM,
    KEYWORD_AS,

    // Misc
    HASH_COMMENT, // #
    UNRECOGNIZED,
    END_OF_FILE
};

struct Token {
    TokenType type;
    std::string_view lexeme;
    size_t line;
    size_t start_pos;
};
