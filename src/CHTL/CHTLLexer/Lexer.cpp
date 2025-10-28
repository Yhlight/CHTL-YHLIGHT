#include "Lexer.h"
#include <iostream>

namespace CHTL {

const std::unordered_map<std::string, TokenType> Lexer::keywords = {
    {"text", TokenType::TEXT},
    {"style", TokenType::STYLE},
    {"script", TokenType::SCRIPT},
    {"if", TokenType::IF},
    {"else", TokenType::ELSE},
    // Note: "else if" is handled by the parser consuming an "else" then an "if".
    {"delete", TokenType::DELETE},
    {"insert", TokenType::INSERT},
    {"after", TokenType::AFTER},
    {"before", TokenType::BEFORE},
    {"replace", TokenType::REPLACE},
    {"from", TokenType::FROM},
    {"as", TokenType::AS},
    {"except", TokenType::EXCEPT},
    {"use", TokenType::USE},
    {"html5", TokenType::HTML5},
    {"inherit", TokenType::INHERIT}
};

Lexer::Lexer(std::string_view source) : m_source(source) {}

std::vector<Token> Lexer::tokenize() {
    while (!isAtEnd()) {
        m_start = m_current;
        scanToken();
    }
    m_tokens.push_back({TokenType::END_OF_FILE, "", m_line, m_column});
    return m_tokens;
}

void Lexer::scanToken() {
    char c = advance();
    switch (c) {
        case '{': addToken(TokenType::LEFT_BRACE); break;
        case '}': addToken(TokenType::RIGHT_BRACE); break;
        case '(': addToken(TokenType::LEFT_PAREN); break;
        case ')': addToken(TokenType::RIGHT_PAREN); break;
        case '[': addToken(TokenType::LEFT_BRACKET); break;
        case ']': addToken(TokenType::RIGHT_BRACKET); break;
        case ':': addToken(TokenType::COLON); break;
        case ';': addToken(TokenType::SEMICOLON); break;
        case ',': addToken(TokenType::COMMA); break;
        case '.': addToken(TokenType::DOT); break;
        case '?': addToken(TokenType::QUESTION); break;
        case '@': addToken(TokenType::AT); break;

        case '=': addToken(TokenType::EQUAL); break;
        case '+': addToken(TokenType::PLUS); break;
        case '>': addToken(TokenType::GREATER); break;
        case '<': addToken(TokenType::LESS); break;

        case '-':
            addToken(match('>') ? TokenType::MINUS_GREATER : TokenType::MINUS);
            break;

        case '*':
            addToken(match('*') ? TokenType::STAR_STAR : TokenType::STAR);
            break;
        case '/':
            if (match('/')) {
                singleLineComment();
            } else if (match('*')) {
                multiLineComment();
            } else {
                addToken(TokenType::SLASH);
            }
            break;
        case '%': addToken(TokenType::PERCENT); break;

        case '&':
            addToken(match('&') ? TokenType::AMPERSAND_AMPERSAND : TokenType::AMPERSAND);
            break;
        case '|':
            if (match('|')) {
                addToken(TokenType::PIPE_PIPE);
            } else {
                // Single '|' is not a CHTL operator, treat as UNKNOWN or part of unquoted literal.
                // For now, let's be strict.
                addToken(TokenType::UNKNOWN);
            }
            break;

        case '#':
            if (peek() == ' ') {
                generatorComment();
            } else {
                addToken(TokenType::HASH);
            }
            break;

        case '"':
        case '\'':
            stringLiteral();
            break;

        // Whitespace
        case ' ':
        case '\r':
        case '\t':
            // Ignore whitespace
            break;
        case '\n':
            m_line++;
            m_column = 1;
            break;

        default:
            if (isdigit(c)) {
                numberLiteral();
            } else if (isalpha(c) || c == '_') {
                identifierOrKeyword();
            } else {
                // CHTL's unquoted literals are tricky.
                // A simple approach is to consume characters until a delimiter is found.
                // This will be context-dependent in the parser.
                // For now, we'll treat them like identifiers.
                identifierOrKeyword();
            }
            break;
    }
}

char Lexer::advance() {
    char c = m_source[m_current++];
    m_column++;
    return c;
}

bool Lexer::match(char expected) {
    if (isAtEnd() || m_source[m_current] != expected) {
        return false;
    }
    m_current++;
    m_column++;
    return true;
}

char Lexer::peek() const {
    if (isAtEnd()) return '\0';
    return m_source[m_current];
}

char Lexer::peekNext() const {
    if (m_current + 1 >= m_source.length()) return '\0';
    return m_source[m_current + 1];
}

bool Lexer::isAtEnd() const {
    return m_current >= m_source.length();
}

void Lexer::addToken(TokenType type) {
    std::string text(m_source.substr(m_start, m_current - m_start));
    m_tokens.push_back({type, text, m_line, m_column});
}

void Lexer::addToken(TokenType type, const std::string& literal) {
    m_tokens.push_back({type, literal, m_line, m_column});
}

void Lexer::stringLiteral() {
    char quote_type = m_source[m_start]; // ' or "
    while (peek() != quote_type && !isAtEnd()) {
        if (peek() == '\n') {
            m_line++;
            m_column = 1;
        }
        advance();
    }

    if (isAtEnd()) {
        // Unterminated string. The parser should handle this error.
        addToken(TokenType::UNKNOWN);
        return;
    }

    // The closing quote.
    advance();

    // Trim the surrounding quotes.
    std::string value(m_source.substr(m_start + 1, m_current - m_start - 2));
    addToken(TokenType::STRING, value);
}

void Lexer::numberLiteral() {
    while (isdigit(peek())) advance();

    // Look for a fractional part.
    if (peek() == '.' && isdigit(peekNext())) {
        // Consume the "."
        advance();
        while (isdigit(peek())) advance();
    }

    addToken(TokenType::NUMBER);
}

void Lexer::identifierOrKeyword() {
    // For CHTL, identifiers and unquoted literals can contain a wide range of characters.
    // A simple rule for now: consume until a character that cannot be part of it.
    while (isalnum(peek()) || peek() == '_' || peek() == '-') {
        advance();
    }

    // For unquoted literals, we might need a more sophisticated check in the future,
    // for example, stopping at '{', '}', ';', etc.
    // This is a good starting point.

    std::string text(m_source.substr(m_start, m_current - m_start));
    TokenType type;
    if (keywords.count(text)) {
        type = keywords.at(text);
    } else {
        type = TokenType::IDENTIFIER; // Or potentially UNQUOTED_LITERAL
    }
    addToken(type);
}


void Lexer::singleLineComment() {
    // A // comment goes until the end of the line.
    while (peek() != '\n' && !isAtEnd()) {
        advance();
    }
    // We don't add a token for whitespace or comments
}

void Lexer::multiLineComment() {
    // A /* comment goes until */
    while (!(peek() == '*' && peekNext() == '/') && !isAtEnd()) {
        if (peek() == '\n') {
            m_line++;
            m_column = 1;
        }
        advance();
    }

    if (isAtEnd()) {
        // Unterminated multi-line comment. Parser should handle this.
        return;
    }

    // Consume the */
    advance();
    advance();
}

void Lexer::generatorComment() {
    // A # comment goes until the end of the line.
    advance(); // consume the space
    while (peek() != '\n' && !isAtEnd()) {
        advance();
    }
    // We don't add a token for generator comments for now.
    // The spec says they are recognized, so we might want to emit them.
    // For now, let's treat them like other comments.
}


} // namespace CHTL
