#pragma once

#include <string>

enum class TokenType {
    Identifier,
    OpenBrace,
    CloseBrace,
    Colon,
    Semicolon,
    StringLiteral,
    Equal,
    Dot,
    At,
    TemplateKeyword,
    OpenParen,
    CloseParen,
    EndOfFile,
};

struct Token {
    TokenType type;
    std::string value;
    int line;
    int column;
    int pos;
};
