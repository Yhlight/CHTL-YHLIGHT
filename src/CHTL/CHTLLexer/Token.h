#pragma once

#include <string>
#include <variant>
#include <ostream>

namespace CHTL {

enum class TokenType {
    // Symbols
    LEFT_BRACE, RIGHT_BRACE,     // { }
    LEFT_PAREN, RIGHT_PAREN,     // ( )
    LEFT_BRACKET, RIGHT_BRACKET, // [ ]
    COLON, COLON_COLON, SEMICOLON, COMMA, DOT, // : ; , .
    EQUAL, EQUAL_EQUAL,          // = ==
    BANG, BANG_EQUAL,            // ! !=
    PLUS, MINUS, STAR, SLASH, PERCENT, // + - * / %
    STAR_STAR,                  // **
    GREATER, GREATER_EQUAL,      // > >=
    LESS, LESS_EQUAL,            // < <=
    AMPERSAND,                  // &
    AMPERSAND_AMPERSAND,        // &&
    PIPE_PIPE,                  // ||
    MINUS_GREATER,              // ->
    QUESTION,                   // ?
    AT,                         // @
    HASH,                       // #

    // Literals
    IDENTIFIER,
    STRING, // "..." or '...'
    NUMBER,
    UNQUOTED_LITERAL,

    // Keywords
    TEXT, STYLE, SCRIPT,
    IF, ELSE, ELSE_IF,
    DELETE, INSERT, AFTER, BEFORE, REPLACE,
    FROM, AS, EXCEPT, USE, HTML5, INHERIT,

    // Special Keywords / Phrases that might need special handling
    // e.g., "at top", "at bottom"
    // The lexer can tokenize them as identifiers, and the parser can combine them.

    // Meta Keywords (often inside brackets)
    // The lexer will tokenize '[', 'Custom', ']', '@', 'Style' separately.
    // The parser will be responsible for understanding these sequences.

    // Comments
    SINGLE_LINE_COMMENT, // //...
    MULTI_LINE_COMMENT,  // /*...*/
    GENERATOR_COMMENT,   // #...

    // End of file
    END_OF_FILE,

    // Unknown token
    UNKNOWN
};

// Helper function to convert token type to string for debugging
inline std::string tokenTypeToString(TokenType type) {
    switch (type) {
        case TokenType::LEFT_BRACE: return "LEFT_BRACE";
        case TokenType::RIGHT_BRACE: return "RIGHT_BRACE";
        case TokenType::LEFT_PAREN: return "LEFT_PAREN";
        case TokenType::RIGHT_PAREN: return "RIGHT_PAREN";
        case TokenType::LEFT_BRACKET: return "LEFT_BRACKET";
        case TokenType::RIGHT_BRACKET: return "RIGHT_BRACKET";
        case TokenType::COLON: return "COLON";
        case TokenType::COLON_COLON: return "COLON_COLON";
        case TokenType::SEMICOLON: return "SEMICOLON";
        case TokenType::COMMA: return "COMMA";
        case TokenType::DOT: return "DOT";
        case TokenType::EQUAL: return "EQUAL";
        case TokenType::EQUAL_EQUAL: return "EQUAL_EQUAL";
        case TokenType::BANG: return "BANG";
        case TokenType::BANG_EQUAL: return "BANG_EQUAL";
        case TokenType::PLUS: return "PLUS";
        case TokenType::MINUS: return "MINUS";
        case TokenType::STAR: return "STAR";
        case TokenType::SLASH: return "SLASH";
        case TokenType::PERCENT: return "PERCENT";
        case TokenType::STAR_STAR: return "STAR_STAR";
        case TokenType::GREATER: return "GREATER";
        case TokenType::GREATER_EQUAL: return "GREATER_EQUAL";
        case TokenType::LESS: return "LESS";
        case TokenType::LESS_EQUAL: return "LESS_EQUAL";
        case TokenType::AMPERSAND: return "AMPERSAND";
        case TokenType::AMPERSAND_AMPERSAND: return "AMPERSAND_AMPERSAND";
        case TokenType::PIPE_PIPE: return "PIPE_PIPE";
        case TokenType::MINUS_GREATER: return "MINUS_GREATER";
        case TokenType::QUESTION: return "QUESTION";
        case TokenType::AT: return "AT";
        case TokenType::HASH: return "HASH";
        case TokenType::IDENTIFIER: return "IDENTIFIER";
        case TokenType::STRING: return "STRING";
        case TokenType::NUMBER: return "NUMBER";
        case TokenType::UNQUOTED_LITERAL: return "UNQUOTED_LITERAL";
        case TokenType::TEXT: return "TEXT";
        case TokenType::STYLE: return "STYLE";
        case TokenType::SCRIPT: return "SCRIPT";
        case TokenType::IF: return "IF";
        case TokenType::ELSE: return "ELSE";
        case TokenType::ELSE_IF: return "ELSE_IF";
        case TokenType::DELETE: return "DELETE";
        case TokenType::INSERT: return "INSERT";
        case TokenType::AFTER: return "AFTER";
        case TokenType::BEFORE: return "BEFORE";
        case TokenType::REPLACE: return "REPLACE";
        case TokenType::FROM: return "FROM";
        case TokenType::AS: return "AS";
        case TokenType::EXCEPT: return "EXCEPT";
        case TokenType::USE: return "USE";
        case TokenType::HTML5: return "HTML5";
        case TokenType::INHERIT: return "INHERIT";
        case TokenType::SINGLE_LINE_COMMENT: return "SINGLE_LINE_COMMENT";
        case TokenType::MULTI_LINE_COMMENT: return "MULTI_LINE_COMMENT";
        case TokenType::GENERATOR_COMMENT: return "GENERATOR_COMMENT";
        case TokenType::END_OF_FILE: return "END_OF_FILE";
        case TokenType::UNKNOWN: return "UNKNOWN";
        default: return "UNKNOWN";
    }
}


struct Token {
    TokenType type;
    std::string lexeme;
    int line = 0;
    int column = 0;

    // Overload the << operator for easy printing of tokens
    friend std::ostream& operator<<(std::ostream& os, const Token& token) {
        os << "Token(" << tokenTypeToString(token.type) << ", '" << token.lexeme << "', L" << token.line << ":C" << token.column << ")";
        return os;
    }
};

} // namespace CHTL
