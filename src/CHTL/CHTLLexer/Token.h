#pragma once

#include <string>
#include <string_view>

namespace CHTL {

enum class TokenType {
    // Special
    Eof,
    Unexpected,

    // Literals
    Identifier,
    String,
    Number,

    // Punctuation
    LeftBrace,   // {
    RightBrace,  // }
    LeftParen,   // (
    RightParen,  // )
    LeftBracket, // [
    RightBracket,// ]
    Colon,       // :
    Semicolon,   // ;
    Comma,       // ,
    Dot,         // .
    Equals,      // =

    // Operators
    Plus,
    Minus,
    Star,
    Slash,
    Percent,
    DoubleStar,

    // Comments
    LineComment,
    BlockComment,
    GeneratorComment,

    // Keywords
    Text,
};

struct Token {
    TokenType type;
    std::string_view lexeme;
    int line;
    int start_pos;
};

}
