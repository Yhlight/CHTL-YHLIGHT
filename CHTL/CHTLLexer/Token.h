#pragma once

#include <string>

enum class TokenType {
    // Keywords
    Text,
    Style,

    // Literals
    String,
    Comment,
    Identifier,
    Value,

    // Punctuation
    LBrace,
    RBrace,
    Colon,
    Equal,
    Semicolon,

    // Other
    EndOfFile,
    Unknown
};

struct Token {
    TokenType type;
    std::string value;
};
