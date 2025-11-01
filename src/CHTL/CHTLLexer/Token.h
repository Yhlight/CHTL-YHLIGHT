#ifndef CHTL_TOKEN_H
#define CHTL_TOKEN_H

#include <string>
#include <string_view>
#include <vector>
#include <map>

namespace CHTL {

enum class TokenType {
    // Meta
    UNKNOWN,
    END_OF_FILE,

    // Single-character tokens
    LEFT_BRACE, RIGHT_BRACE,     // { }
    LEFT_PAREN, RIGHT_PAREN,     // ( )
    LEFT_BRACKET, RIGHT_BRACKET, // [ ]
    COLON, SEMICOLON, COMMA, DOT, // : ; , .
    AMPERSAND,                   // &
    QUESTION,                    // ?

    // Operators
    EQUAL,                       // =
    PLUS, MINUS, STAR, SLASH, PERCENT, // + - * / %
    STAR_STAR,                   // **
    GREATER, LESS,               // > <

    // Logical Operators
    AMPERSAND_AMPERSAND,         // &&
    PIPE_PIPE,                   // ||

    // CHTL JS specific
    ARROW,                       // ->
    BIND_ARROW,                  // &->

    // Literals
    IDENTIFIER,
    STRING,
    NUMBER,
    UNQUOTED_LITERAL,

    // Keywords
    TEXT, STYLE, SCRIPT,
    IF, ELSE,
    INHERIT, DELETE, INSERT,
    AFTER, BEFORE, REPLACE, AT, TOP, BOTTOM,
    FROM, AS, EXCEPT, USE,
    HTML5,
    CONDITION,

    // Annotations / Special Keywords that start with '@'
    AT_STYLE, AT_ELEMENT, AT_VAR,
    AT_HTML, AT_JAVASCRIPT, AT_CHTL, AT_CONFIG,

    // Block Keywords that start with '['
    KEYWORD_CUSTOM,
    KEYWORD_TEMPLATE,
    KEYWORD_ORIGIN,
    KEYWORD_IMPORT,
    KEYWORD_NAMESPACE,
    KEYWORD_CONFIGURATION,
    KEYWORD_INFO,
    KEYWORD_EXPORT,
    KEYWORD_NAME,
    KEYWORD_ORIGIN_TYPE,


    // Comments
    COMMENT,        // // or /**/
    GEN_COMMENT     // #
};

// Function to convert TokenType to string for debugging
inline std::string TokenTypeToString(TokenType type) {
    static const std::map<TokenType, std::string> typeMap = {
        {TokenType::UNKNOWN, "UNKNOWN"}, {TokenType::END_OF_FILE, "END_OF_FILE"},
        {TokenType::LEFT_BRACE, "LEFT_BRACE"}, {TokenType::RIGHT_BRACE, "RIGHT_BRACE"},
        {TokenType::LEFT_PAREN, "LEFT_PAREN"}, {TokenType::RIGHT_PAREN, "RIGHT_PAREN"},
        {TokenType::LEFT_BRACKET, "LEFT_BRACKET"}, {TokenType::RIGHT_BRACKET, "RIGHT_BRACKET"},
        {TokenType::COLON, "COLON"}, {TokenType::SEMICOLON, "SEMICOLON"},
        {TokenType::COMMA, "COMMA"}, {TokenType::DOT, "DOT"},
        {TokenType::AMPERSAND, "AMPERSAND"}, {TokenType::QUESTION, "QUESTION"},
        {TokenType::EQUAL, "EQUAL"}, {TokenType::PLUS, "PLUS"},
        {TokenType::MINUS, "MINUS"}, {TokenType::STAR, "STAR"},
        {TokenType::SLASH, "SLASH"}, {TokenType::PERCENT, "PERCENT"},
        {TokenType::STAR_STAR, "STAR_STAR"}, {TokenType::GREATER, "GREATER"},
        {TokenType::LESS, "LESS"}, {TokenType::AMPERSAND_AMPERSAND, "AMPERSAND_AMPERSAND"},
        {TokenType::PIPE_PIPE, "PIPE_PIPE"}, {TokenType::ARROW, "ARROW"},
        {TokenType::BIND_ARROW, "BIND_ARROW"}, {TokenType::IDENTIFIER, "IDENTIFIER"},
        {TokenType::STRING, "STRING"}, {TokenType::NUMBER, "NUMBER"},
        {TokenType::UNQUOTED_LITERAL, "UNQUOTED_LITERAL"}, {TokenType::TEXT, "TEXT"},
        {TokenType::STYLE, "STYLE"}, {TokenType::SCRIPT, "SCRIPT"},
        {TokenType::IF, "IF"}, {TokenType::ELSE, "ELSE"},
        {TokenType::INHERIT, "INHERIT"}, {TokenType::DELETE, "DELETE"},
        {TokenType::INSERT, "INSERT"}, {TokenType::AFTER, "AFTER"},
        {TokenType::BEFORE, "BEFORE"}, {TokenType::REPLACE, "REPLACE"},
        {TokenType::AT, "AT"}, {TokenType::TOP, "TOP"},
        {TokenType::BOTTOM, "BOTTOM"}, {TokenType::FROM, "FROM"},
        {TokenType::AS, "AS"}, {TokenType::EXCEPT, "EXCEPT"},
        {TokenType::USE, "USE"}, {TokenType::HTML5, "HTML5"},
        {TokenType::CONDITION, "CONDITION"},
        {TokenType::AT_STYLE, "AT_STYLE"}, {TokenType::AT_ELEMENT, "AT_ELEMENT"},
        {TokenType::AT_VAR, "AT_VAR"}, {TokenType::AT_HTML, "AT_HTML"},
        {TokenType::AT_JAVASCRIPT, "AT_JAVASCRIPT"}, {TokenType::AT_CHTL, "AT_CHTL"},
        {TokenType::AT_CONFIG, "AT_CONFIG"}, {TokenType::KEYWORD_CUSTOM, "KEYWORD_CUSTOM"},
        {TokenType::KEYWORD_TEMPLATE, "KEYWORD_TEMPLATE"}, {TokenType::KEYWORD_ORIGIN, "KEYWORD_ORIGIN"},
        {TokenType::KEYWORD_IMPORT, "KEYWORD_IMPORT"}, {TokenType::KEYWORD_NAMESPACE, "KEYWORD_NAMESPACE"},
        {TokenType::KEYWORD_CONFIGURATION, "KEYWORD_CONFIGURATION"}, {TokenType::KEYWORD_INFO, "KEYWORD_INFO"},
        {TokenType::KEYWORD_EXPORT, "KEYWORD_EXPORT"}, {TokenType::KEYWORD_NAME, "KEYWORD_NAME"},
        {TokenType::KEYWORD_ORIGIN_TYPE, "KEYWORD_ORIGIN_TYPE"}, {TokenType::COMMENT, "COMMENT"},
        {TokenType::GEN_COMMENT, "GEN_COMMENT"}
    };
    auto it = typeMap.find(type);
    if (it != typeMap.end()) {
        return it->second;
    }
    return "UNKNOWN";
}


struct Token {
    TokenType type;
    std::string_view lexeme;
    int line;
    size_t start_pos; // Start position in the source string
};

} // namespace CHTL

#endif // CHTL_TOKEN_H
