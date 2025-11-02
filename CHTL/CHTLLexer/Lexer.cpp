#include "Lexer.h"
#include <iostream>
#include <cctype>
#include <map>

static const std::map<std::string, TokenType> keywords = {
    {"inherit", TokenType::InheritKeyword},
    {"delete", TokenType::DeleteKeyword},
    {"insert", TokenType::InsertKeyword},
    {"after", TokenType::AfterKeyword},
    {"before", TokenType::BeforeKeyword},
    {"replace", TokenType::ReplaceKeyword},
    {"from", TokenType::FromKeyword},
    {"as", TokenType::AsKeyword},
    {"except", TokenType::ExceptKeyword},
    {"use", TokenType::UseKeyword},
    {"html5", TokenType::Html5Keyword},
    {"text", TokenType::TextKeyword},
    {"style", TokenType::StyleKeyword},
    {"script", TokenType::ScriptKeyword},
};

Lexer::Lexer(std::string source) : source(std::move(source)) {}

std::vector<Token> Lexer::tokenize() {
    std::vector<Token> tokens;
    Token token = nextToken();
    while(token.type != TokenType::EndOfFile) {
        tokens.push_back(token);
        token = nextToken();
    }
    tokens.push_back(token); // push the EOF token
    return tokens;
}

Token Lexer::nextToken() {
    skipWhitespace();
    size_t start_pos = current;

    if (isAtEnd()) {
        return {TokenType::EndOfFile, "", line, col, start_pos};
    }

    char c = advance();

    if (c == '/' && peek() == '/') {
        std::string comment;
        while (peek() != '\n' && !isAtEnd()) {
            comment += advance();
        }
        return {TokenType::LineComment, comment, line, col, start_pos};
    }

    if (c == '/' && peek() == '*') {
        std::string comment;
        advance();
        while (!isAtEnd()) {
            if (peek() == '*' && source[current + 1] == '/') {
                break;
            }
            comment += advance();
        }
        advance();
        advance();
        return {TokenType::BlockComment, comment, line, col, start_pos};
    }

    if (c == '#') {
        std::string comment;
        while (peek() != '\n' && !isAtEnd()) {
            comment += advance();
        }
        return {TokenType::GeneratorComment, comment, line, col, start_pos};
    }
    if (c == '[') {
        std::string value;
        while (peek() != ']' && !isAtEnd()) {
            value += advance();
        }
        if (peek() == ']') {
            advance(); // consume ']'
            if (value == "Template") return {TokenType::TemplateKeyword, "[Template]", line, col, start_pos};
            if (value == "Custom") return {TokenType::CustomKeyword, "[Custom]", line, col, start_pos};
            if (value == "Origin") return {TokenType::OriginKeyword, "[Origin]", line, col, start_pos};
            if (value == "Import") return {TokenType::ImportKeyword, "[Import]", line, col, start_pos};
            if (value == "Namespace") return {TokenType::NamespaceKeyword, "[Namespace]", line, col, start_pos};
            if (value == "Configuration") return {TokenType::ConfigSpecifier, "[Configuration]", line, col, start_pos};
        }
        return {TokenType::Identifier, "[" + value + (isAtEnd() ? "" : "]"), line, col, start_pos};
    }
    if (c == '@') {
        std::string value;
        while (isalpha(peek())) {
            value += advance();
        }
        if (value == "Style") return {TokenType::StyleSpecifier, "@Style", line, col, start_pos};
        if (value == "Element") return {TokenType::ElementSpecifier, "@Element", line, col, start_pos};
        if (value == "Var") return {TokenType::VarSpecifier, "@Var", line, col, start_pos};
        if (value == "Html") return {TokenType::HtmlSpecifier, "@Html", line, col, start_pos};
        if (value == "JavaScript") return {TokenType::JavaScriptSpecifier, "@JavaScript", line, col, start_pos};
        if (value == "Chtl") return {TokenType::ChtlSpecifier, "@Chtl", line, col, start_pos};
        if (value == "CJmod") return {TokenType::CjmodSpecifier, "@CJmod", line, col, start_pos};
        return {TokenType::Identifier, "@" + value, line, col, start_pos};
    }

    if (c == '"' || c == '\'') {
        char quote_type = c;
        std::string value;
        while (peek() != quote_type && !isAtEnd()) {
            value += advance();
        }
        advance(); // consume the closing quote
        return {TokenType::StringLiteral, value, line, col, start_pos};
    }

    if (isalpha(c)) {
        std::string value;
        value += c;
        while (isalnum(peek()) || peek() == '-') {
            value += advance();
        }
        if (value == "at") {
            size_t saved_current = current;
            size_t saved_line = line;
            size_t saved_col = col;
            skipWhitespace();
            if (peek() == 't' && source.substr(current, 3) == "top") {
                advance(); advance(); advance();
                return {TokenType::AtTopKeyword, "at top", line, col, start_pos};
            }
            if (peek() == 'b' && source.substr(current, 6) == "bottom") {
                advance(); advance(); advance(); advance(); advance(); advance();
                return {TokenType::AtBottomKeyword, "at bottom", line, col, start_pos};
            }
            current = saved_current;
            line = saved_line;
            col = saved_col;
        }

        if (keywords.count(value)) {
            return {keywords.at(value), value, line, col, start_pos};
        }
        return {TokenType::Identifier, value, line, col, start_pos};
    }

    if (isdigit(c)) {
        std::string value;
        value += c;
        while (isdigit(peek()) || isalpha(peek())) {
            value += advance();
        }
        return {TokenType::NumericLiteral, value, line, col, start_pos};
    }

    switch (c) {
        case '{':
            if (peek() == '{') {
                advance();
                return {TokenType::OpenBrace, "{{", line, col, start_pos};
            }
            return {TokenType::OpenBrace, "{", line, col, start_pos};
        case '}':
            if (peek() == '}') {
                advance();
                return {TokenType::CloseBrace, "}}", line, col, start_pos};
            }
            return {TokenType::CloseBrace, "}", line, col, start_pos};
        case '(': return {TokenType::OpenParen, "(", line, col, start_pos};
        case ')': return {TokenType::CloseParen, ")", line, col, start_pos};
        case ';': return {TokenType::Semicolon, ";", line, col, start_pos};
        case ':': return {TokenType::Colon, ":", line, col, start_pos};
        case '=': return {TokenType::Equals, "=", line, col, start_pos};
        case ',': return {TokenType::Comma, ",", line, col, start_pos};
        case '&': return {TokenType::Ampersand, "&", line, col, start_pos};
        case '+': return {TokenType::Plus, "+", line, col, start_pos};
        case '-': return {TokenType::Minus, "-", line, col, start_pos};
        case '*': {
            if (peek() == '*') {
                advance();
                return {TokenType::DoubleStar, "**", line, col, start_pos};
            }
            return {TokenType::Star, "*", line, col, start_pos};
        }
        case '/': return {TokenType::Slash, "/", line, col, start_pos};
        case '%': return {TokenType::Percent, "%", line, col, start_pos};
    }

    return {TokenType::Identifier, std::string(1, c), line, col, start_pos};
}

char Lexer::advance() {
    col++;
    return source[current++];
}

char Lexer::peek() {
    if (isAtEnd()) return '\0';
    return source[current];
}

bool Lexer::isAtEnd() {
    return current >= source.length();
}

void Lexer::skipWhitespace() {
    while (!isAtEnd()) {
        char c = peek();
        switch (c) {
            case ' ':
            case '\r':
            case '\t':
                advance();
                break;
            case '\n':
                line++;
                col = 1;
                advance();
                break;
            default:
                return;
        }
    }
}
