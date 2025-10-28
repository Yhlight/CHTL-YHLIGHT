#pragma once

#include <string>

namespace CHTL {

enum class TokenType {
    // Special Tokens
    EndOfFile,
    Unexpected,

    // Literals
    Identifier,
    String,
    Number,

    // Punctuation
    OpenBrace,      // {
    CloseBrace,     // }
    OpenParen,      // (
    CloseParen,     // )
    OpenBracket,    // [
    CloseBracket,   // ]
    Semicolon,      // ;
    Colon,          // :
    Comma,          // ,
    Dot,            // .
    At,             // @

    // Operators
    Plus,           // +
    Minus,          // -
    Asterisk,       // *
    Slash,          // /
    Percent,        // %
    DoubleAsterisk, // **

    // Keywords (will be expanded)
    Text,
    Style,
    Script,
    TemplateKeyword, // [Template]
    OriginKeyword, // [Origin]
    InheritKeyword,

    // Comments
    LineComment,
    BlockComment,
    GeneratorComment,
};

struct Token {
    TokenType type;
    std::string value;
    int line;
    int column;
    int pos;
};

} // namespace CHTL
