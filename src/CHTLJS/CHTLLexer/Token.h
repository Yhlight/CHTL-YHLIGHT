#pragma once

#include <string>
#include <string_view>

namespace CHTLJS {

enum class TokenType {
    // Single-character tokens.
    LeftParen, RightParen, LeftBrace, RightBrace, LeftBracket, RightBracket,
    Comma, Dot, Minus, Plus, Semicolon, Slash, Star, Colon,

    // One or two character tokens.
    Bang, BangEqual,
    Equal, EqualEqual,
    Greater, GreaterEqual,
    Less, LessEqual,
    ArrowDash, // ->

    // CHTL JS specific
    LeftDoubleBrace, RightDoubleBrace,

    // Literals.
    Identifier, String, Number,

    // Keywords.
    And, Class, Else, False, Fun, For, If, Nil, Or,
    Print, Return, Super, This, True, Var, While,
    Listen, Delegate, Animate,

    Eof
};

struct Token {
    TokenType type;
    std::string_view lexeme;
    int line;
    int start_pos;
};

} // namespace CHTLJS
