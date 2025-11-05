#pragma once

#include <string>
#include <string_view>
#include <cstddef>

namespace CHTL {

enum class TokenType {
    // Single-character tokens
    LEFT_BRACE, RIGHT_BRACE,       // { }
    LEFT_PAREN, RIGHT_PAREN,       // ( )
    LEFT_BRACKET, RIGHT_BRACKET,   // [ ]
    COLON, SEMICOLON,              // : ;
    DOT, COMMA,                    // . ,
    PLUS, MINUS, STAR, SLASH,      // + - * /
    PERCENT,                       // %
    AMPERSAND,                     // &
    QUESTION,                      // ?
    AT,                            // @

    // One or two character tokens
    EQUAL, COLON_COLON,             // = (CE equality with :)
    POWER,                         // **
    LOGICAL_AND,                   // &&
    LOGICAL_OR,                    // ||
    GREATER,                       // >
    LESS,                          // <

    // Literals
    IDENTIFIER,
    STRING,                        // "..." or '...'
    NUMBER,
    RAW_CONTENT,

    // Keywords
    TEXT, STYLE, SCRIPT,
    IF, ELSE,
    INHERIT, DELETE, INSERT,
    AFTER, BEFORE, REPLACE,
    AT_TOP, AT_BOTTOM,
    FROM, AS, EXCEPT, USE, HTML5,

    // Block Keywords
    BLOCK_TEMPLATE,      // [Template]
    BLOCK_CUSTOM,        // [Custom]
    BLOCK_ORIGIN,        // [Origin]
    BLOCK_IMPORT,        // [Import]
    BLOCK_NAMESPACE,     // [Namespace]
    BLOCK_CONFIGURATION, // [Configuration]
    BLOCK_INFO,          // [Info]
    BLOCK_EXPORT,        // [Export]
    BLOCK_NAME,          // [Name]

    // Special
    GENERATOR_COMMENT,   // # comment

    END_OF_FILE,
    ERROR
};

struct Token {
    TokenType type;
    std::string_view lexeme;
    size_t start_pos = 0;
    size_t line = 1;
};

} // namespace CHTL
