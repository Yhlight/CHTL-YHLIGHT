#pragma once

#include <string>

namespace CHTL {

enum class TokenType {
    Identifier,
    String,
    OpenBrace,
    CloseBrace,
    Colon,
    Semicolon,
    Equal,
    Ampersand,
    OpenBracket,
    CloseBracket,
    At,
    OpenParen,
    CloseParen,
    Delete,
    Comma,
    Eof,
    Unknown
};

struct Token {
    TokenType type;
    std::string value;
};

} // namespace CHTL
