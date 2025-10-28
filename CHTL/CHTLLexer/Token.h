#pragma once

#include <string>

enum class TokenType {
    // Keywords
    Text,

    // Literals
    String,

    // Punctuation
    LBrace,
    RBrace,

    // Other
    EndOfFile,
    Unknown
};

struct Token {
    TokenType type;
    std::string value;
};
