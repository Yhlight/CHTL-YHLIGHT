#pragma once

#include <string>

enum class TokenType {
    // Keywords
    Text,

    // Literals
    String,
    Comment,
    Identifier,

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
