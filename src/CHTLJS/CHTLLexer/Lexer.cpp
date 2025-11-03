#include "Lexer.h"
#include <unordered_map>

namespace CHTLJS {

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

bool Lexer::match(char expected) {
    if (isAtEnd()) return false;
    if (m_source[m_current] != expected) return false;
    m_current++;
    return true;
}

void Lexer::scanToken() {
    char c = advance();
    switch (c) {
        case '(': addToken(TokenType::LeftParen); break;
        case ')': addToken(TokenType::RightParen); break;
        case '[': addToken(TokenType::LeftBracket); break;
        case ']': addToken(TokenType::RightBracket); break;
        case ':': addToken(TokenType::Colon); break;
        case '{':
            if (match('{')) {
                addToken(TokenType::LeftDoubleBrace);
            } else {
                addToken(TokenType::LeftBrace);
            }
            break;
        case '}':
            if (match('}')) {
                addToken(TokenType::RightDoubleBrace);
            } else {
                addToken(TokenType::RightBrace);
            }
            break;
        case ',': addToken(TokenType::Comma); break;
        case '.': addToken(TokenType::Dot); break;
        case '-':
            if (match('>')) {
                addToken(TokenType::ArrowDash);
            } else {
                addToken(TokenType::Minus);
            }
            break;
        case '+': addToken(TokenType::Plus); break;
        case ';': addToken(TokenType::Semicolon); break;
        case '*': addToken(TokenType::Star); break;
        case '!': addToken(match('=') ? TokenType::BangEqual : TokenType::Bang); break;
        case '=': addToken(match('=') ? TokenType::EqualEqual : TokenType::Equal); break;
        case '<': addToken(match('=') ? TokenType::LessEqual : TokenType::Less); break;
        case '>': addToken(match('=') ? TokenType::GreaterEqual : TokenType::Greater); break;
        case '/':
            if (match('/')) {
                while (peek() != '\n' && !isAtEnd()) advance();
            } else {
                addToken(TokenType::Slash);
            }
            break;
        case ' ':
        case '\r':
        case '\t':
            break;
        case '\n':
            m_line++;
            break;
        case '"': string(); break;
        case '#': identifier(); break;
        default:
            if (isdigit(c)) {
                number();
            } else if (isalpha(c) || c == '_') {
                identifier();
            } else {
                // Handle error
            }
            break;
    }
}

void Lexer::identifier() {
    static const std::unordered_map<std::string, TokenType> keywords = {
        {"and",    TokenType::And},
        {"class",  TokenType::Class},
        {"else",   TokenType::Else},
        {"false",  TokenType::False},
        {"for",    TokenType::For},
        {"fun",    TokenType::Fun},
        {"if",     TokenType::If},
        {"nil",    TokenType::Nil},
        {"or",     TokenType::Or},
        {"print",  TokenType::Print},
        {"return", TokenType::Return},
        {"super",  TokenType::Super},
        {"this",   TokenType::This},
        {"true",   TokenType::True},
        {"var",    TokenType::Var},
        {"while",  TokenType::While},
        {"Listen", TokenType::Listen},
        {"Delegate", TokenType::Delegate},
        {"Animate", TokenType::Animate},
    };
    while (isalnum(peek()) || peek() == '_') advance();
    std::string text = std::string(m_source.substr(m_start, m_current - m_start));
    if (keywords.count(text)) {
        addToken(keywords.at(text));
    } else {
        addToken(TokenType::Identifier);
    }
}

void Lexer::number() {
    while (isdigit(peek())) advance();
    if (peek() == '.' && isdigit(peekNext())) {
        advance();
        while (isdigit(peek())) advance();
    }
    addToken(TokenType::Number);
}

void Lexer::string() {
    while (peek() != '"' && !isAtEnd()) {
        if (peek() == '\n') m_line++;
        advance();
    }
    if (isAtEnd()) {
        // Handle error
        return;
    }
    advance();
    addToken(TokenType::String);
}

} // namespace CHTLJS
