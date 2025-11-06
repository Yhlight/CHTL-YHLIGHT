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
    Eof,
    Unknown
};

struct Token {
    TokenType type;
    std::string value;
};

} // namespace CHTL
