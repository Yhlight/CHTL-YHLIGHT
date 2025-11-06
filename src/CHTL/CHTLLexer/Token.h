#pragma once

#include <string>

namespace CHTL {

enum class TokenType {
    Identifier,
    String,
    Eof,
    Unknown
};

struct Token {
    TokenType type;
    std::string value;
};

} // namespace CHTL
