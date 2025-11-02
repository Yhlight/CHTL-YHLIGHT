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

    if (isAtEnd()) {
        return {TokenType::EndOfFile, "", line, column};
    }

    char c = advance();

    if (c == '/' && peek() == '/') {
        std::string comment;
        while (peek() != '\n' && !isAtEnd()) {
            comment += advance();
        }
        return {TokenType::LineComment, comment, line, column};
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
        return {TokenType::BlockComment, comment, line, column};
    }

    if (c == '#') {
        std::string comment;
        while (peek() != '\n' && !isAtEnd()) {
            comment += advance();
        }
        return {TokenType::GeneratorComment, comment, line, column};
    }
    if (c == '[') {
        std::string value;
        while (peek() != ']' && !isAtEnd()) {
            value += advance();
        }
        if (peek() == ']') {
            advance(); // consume ']'
            if (value == "Template") return {TokenType::TemplateKeyword, "[Template]", line, column};
            if (value == "Custom") return {TokenType::CustomKeyword, "[Custom]", line, column};
            if (value == "Origin") return {TokenType::OriginKeyword, "[Origin]", line, column};
            if (value == "Import") return {TokenType::ImportKeyword, "[Import]", line, column};
            if (value == "Namespace") return {TokenType::NamespaceKeyword, "[Namespace]", line, column};
            if (value == "Configuration") return {TokenType::ConfigurationKeyword, "[Configuration]", line, column};
        }
        return {TokenType::Unknown, "[" + value + (isAtEnd() ? "" : "]"), line, column};
    }
    if (c == '@') {
        std::string value;
        while (isalpha(peek())) {
            value += advance();
        }
        if (value == "Style") return {TokenType::StyleSpecifier, "@Style", line, column};
        if (value == "Element") return {TokenType::ElementSpecifier, "@Element", line, column};
        if (value == "Var") return {TokenType::VarSpecifier, "@Var", line, column};
        if (value == "Html") return {TokenType::HtmlSpecifier, "@Html", line, column};
        if (value == "JavaScript") return {TokenType::JavaScriptSpecifier, "@JavaScript", line, column};
        if (value == "Chtl") return {TokenType::ChtlSpecifier, "@Chtl", line, column};
        if (value == "CJmod") return {TokenType::CJmodSpecifier, "@CJmod", line, column};
        return {TokenType::At, "@" + value, line, column};
    }

    if (c == '"' || c == '\'') {
        char quote_type = c;
        std::string value;
        while (peek() != quote_type && !isAtEnd()) {
            value += advance();
        }
        advance(); // consume the closing quote
        return {TokenType::StringLiteral, value, line, column};
    }

    if (isalpha(c)) {
        std::string value;
        value += c;
        while (isalnum(peek())) {
            value += advance();
        }
        if (value == "at") {
            size_t saved_current = current;
            size_t saved_line = line;
            size_t saved_column = column;
            skipWhitespace();
            if (peek() == 't' && source.substr(current, 3) == "top") {
                advance(); advance(); advance();
                return {TokenType::AtTopKeyword, "at top", line, column};
            }
            if (peek() == 'b' && source.substr(current, 6) == "bottom") {
                advance(); advance(); advance(); advance(); advance(); advance();
                return {TokenType::AtBottomKeyword, "at bottom", line, column};
            }
            current = saved_current;
            line = saved_line;
            column = saved_column;
        }

        if (keywords.count(value)) {
            return {keywords.at(value), value, line, column};
        }
        return {TokenType::Identifier, value, line, column};
    }

    if (isdigit(c)) {
        std::string value;
        value += c;
        while (isdigit(peek())) {
            value += advance();
        }
        return {TokenType::NumericLiteral, value, line, column};
    }

    switch (c) {
        case '{': return {TokenType::OpenBrace, "{", line, column};
        case '}': return {TokenType::CloseBrace, "}", line, column};
        case '(': return {TokenType::OpenParen, "(", line, column};
        case ')': return {TokenType::CloseParen, ")", line, column};
        case ';': return {TokenType::Semicolon, ";", line, column};
        case ':': return {TokenType::Colon, ":", line, column};
        case '=': return {TokenType::Equals, "=", line, column};
        case ',': return {TokenType::Comma, ",", line, column};
        case '&': return {TokenType::Ampersand, "&", line, column};
        case '+': return {TokenType::Plus, "+", line, column};
        case '-': return {TokenType::Minus, "-", line, column};
        case '*': {
            if (peek() == '*') {
                advance();
                return {TokenType::DoubleAsterisk, "**", line, column};
            }
            return {TokenType::Asterisk, "*", line, column};
        }
        case '/': return {TokenType::Slash, "/", line, column};
        case '%': return {TokenType::Percent, "%", line, column};
    }

    return {TokenType::Unknown, std::string(1, c), line, column};
}

char Lexer::advance() {
    column++;
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
                column = 1;
                advance();
                break;
            default:
                return;
        }
    }
}
