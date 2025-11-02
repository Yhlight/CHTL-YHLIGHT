#pragma once

#include <string>
#include <utility>

enum class TokenType {
    // Keywords
    TemplateKeyword,
    CustomKeyword,
    OriginKeyword,
    ImportKeyword,
    NamespaceKeyword,
    ConfigurationKeyword,

    // Identifiers and literals
    Identifier,
    StringLiteral,
    NumericLiteral,

    // Punctuation
    OpenBrace,
    CloseBrace,
    OpenParen,
    CloseParen,
    Semicolon,
    Colon,
    Equals,
    Comma,

    // Comments
    LineComment,
    BlockComment,
    GeneratorComment,

    // Special Symbols
    At,
    Hash,
    Ampersand,

    // Operators
    Plus,
    Minus,
    Asterisk,
    Slash,
    Percent,
    DoubleAsterisk,

    // End-of-file
    EndOfFile,

    // Unknown
    Unknown,
};

struct Token {
    TokenType type;
    std::string value;
    size_t line;
    size_t column;
};
