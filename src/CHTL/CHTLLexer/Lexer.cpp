#include "Lexer.h"

#include <vector>

namespace CHTL {

Lexer::Lexer(std::string_view source) : m_source(source) {}

std::vector<Token> Lexer::scanTokens() {
    while (!isAtEnd()) {
        m_start = m_current;
        scanToken();
    }
    m_tokens.emplace_back(Token{TokenType::Eof, "", m_line, 0});
    return m_tokens;
}

bool Lexer::isAtEnd() { return m_current >= m_source.length(); }

char Lexer::advance() { return m_source[m_current++]; }

void Lexer::addToken(TokenType type) {
    addToken(type, m_source.substr(m_start, m_current - m_start));
}

void Lexer::addToken(TokenType type, std::string_view lexeme) {
    m_tokens.emplace_back(Token{type, lexeme, m_line, m_start});
}

char Lexer::peek() {
    if (isAtEnd()) return '\0';
    return m_source[m_current];
}

char Lexer::peekNext() {
    if (m_current + 1 >= m_source.length()) return '\0';
    return m_source[m_current + 1];
}


void Lexer::scanToken() {
    char c = advance();
    switch (c) {
    case '(':
        addToken(TokenType::LeftParen);
        break;
    case ')':
        addToken(TokenType::RightParen);
        break;
    case '{':
        addToken(TokenType::LeftBrace);
        break;
    case '}':
        addToken(TokenType::RightBrace);
        break;
    case '[':
        if (isalpha(peek())) {
            blockKeyword();
        } else {
            addToken(TokenType::LeftBracket);
        }
        break;
    case ']':
        addToken(TokenType::RightBracket);
        break;
    case ',':
        addToken(TokenType::Comma);
        break;
    case '.':
        addToken(TokenType::Dot);
        break;
    case '-':
        addToken(TokenType::Minus);
        break;
    case '+':
        addToken(TokenType::Plus);
        break;
    case ';':
        addToken(TokenType::Semicolon);
        break;
    case '*':
        addToken(TokenType::Star);
        break;
    case '=':
        addToken(TokenType::Equals);
        break;
    case ':':
        addToken(TokenType::Colon);
        break;
    case '/':
        if (peek() == '/') {
            while (peek() != '\n' && !isAtEnd()) advance();
            addToken(TokenType::LineComment);
        } else if (peek() == '*') {
            advance(); // consume '*'
            while (!(peek() == '*' && peekNext() == '/') && !isAtEnd()) {
                if (peek() == '\n') m_line++;
                advance();
            }
            if (isAtEnd()) {
                addToken(TokenType::Unexpected, "Unterminated block comment.");
            } else {
                advance(); // consume '*'
                advance(); // consume '/'
                addToken(TokenType::BlockComment);
            }
        } else {
            addToken(TokenType::Slash);
        }
        break;
    case '#':
        while (peek() != '\n' && !isAtEnd()) advance();
        addToken(TokenType::GeneratorComment);
        break;
    case '%':
        addToken(TokenType::Percent);
        break;
    case ' ':
    case '\r':
    case '\t':
        // Ignore whitespace.
        break;

    case '\n':
        m_line++;
        break;

    case '"':
        string();
        break;
    default:
        if (isdigit(c)) {
            number();
        } else if (isalpha(c) || c == '_' || c == '@') {
            identifier();
        } else {
            addToken(TokenType::Unexpected, "Unexpected character.");
        }
        break;
    }
}

TokenType Lexer::identifierType() {
    auto identifier = m_source.substr(m_start, m_current - m_start);
    if (identifier == "text") return TokenType::Text;
    if (identifier == "style") return TokenType::Style;
    if (identifier == "@Style") return TokenType::AtStyle;
    if (identifier == "@Element") return TokenType::AtElement;
    if (identifier == "@Var") return TokenType::AtVar;
    if (identifier == "@Html") return TokenType::AtHtml;
    if (identifier == "@JavaScript") return TokenType::AtJavaScript;

    return TokenType::Identifier;
}

void Lexer::identifier() {
    while (isalnum(peek()) || peek() == '_' || peek() == '-')
        advance();
    addToken(identifierType());
}

void Lexer::blockKeyword() {
    while (peek() != ']' && !isAtEnd()) {
        advance();
    }
    if (isAtEnd()) {
        addToken(TokenType::Unexpected, "Unterminated block keyword.");
        return;
    }
    advance(); // consume ']'
    addToken(TokenType::BlockKeyword);
}

void Lexer::number() {
    while (isdigit(peek()))
        advance();

    if (peek() == '.' && isdigit(peekNext())) {
        advance();
        while (isdigit(peek()))
            advance();
    }
    addToken(TokenType::Number);
}

void Lexer::string() {
    while (peek() != '"' && !isAtEnd()) {
        if (peek() == '\n')
            m_line++;
        advance();
    }

    if (isAtEnd()) {
        addToken(TokenType::Unexpected, "Unterminated string.");
        return;
    }
    advance();
    addToken(TokenType::String);
}


} // namespace CHTL
