#include "Lexer.h"
#include <unordered_map>

namespace CHTL {

Lexer::Lexer(std::string_view source)
    : source_(source) {}

Token Lexer::peekToken() {
    Lexer tempLexer = *this;
    return tempLexer.scanToken();
}

Token Lexer::scanToken() {
    skipWhitespace();
    start_ = current_;

    if (isAtEnd()) return makeToken(TokenType::END_OF_FILE);

    char c = advance();
    switch (c) {
        case '(': return makeToken(TokenType::LEFT_PAREN);
        case ')': return makeToken(TokenType::RIGHT_PAREN);
        case '{': return makeToken(TokenType::LEFT_BRACE);
        case '}': return makeToken(TokenType::RIGHT_BRACE);
        case '[':
            if (std::isalpha(peek())) {
                return blockKeyword();
            } else {
                return makeToken(TokenType::LEFT_BRACKET);
            }
        case ']': return makeToken(TokenType::RIGHT_BRACKET);
        case ';': return makeToken(TokenType::SEMICOLON);
        case ':': return makeToken(TokenType::COLON);
        case ',': return makeToken(TokenType::COMMA);
        case '.': return makeToken(TokenType::DOT);
        case '-': return makeToken(TokenType::MINUS);
        case '+': return makeToken(TokenType::PLUS);
        case '%': return makeToken(TokenType::PERCENT);
        case '?': return makeToken(TokenType::QUESTION);
        case '@': return makeToken(TokenType::AT);
        case '&': return makeToken(match('&') ? TokenType::LOGICAL_AND : TokenType::AMPERSAND);
        case '|': return makeToken(match('|') ? TokenType::LOGICAL_OR : TokenType::ERROR); // Assuming | is an error
        case '=': return makeToken(TokenType::EQUAL);
        case '*': return makeToken(match('*') ? TokenType::POWER : TokenType::STAR);
        case '/':
            if (match('/')) {
                // A comment goes until the end of the line.
                while (peek() != '\n' && !isAtEnd()) advance();
                return scanToken(); // Recursively call scanToken to get the next token
            } else if (match('*')) {
                // A block comment goes until */ is found.
                while (peek() != '*' && peekNext() != '/' && !isAtEnd()) {
                    if (peek() == '\n') line_++;
                    advance();
                }
                if (!isAtEnd()) {
                    advance(); // Consume the *
                    advance(); // Consume the /
                }
                return scanToken(); // Recursively call scanToken to get the next token
            }
            else {
                return makeToken(TokenType::SLASH);
            }
        case '#':
            if (peek() == ' ') {
                advance(); // consume the space
                while (peek() != '\n' && !isAtEnd()) advance();
                return makeToken(TokenType::GENERATOR_COMMENT);
            } else {
                 return errorToken("Generator comment must be followed by a space.");
            }

        case '"':
        case '\'':
            return string(c);
    }

    if (std::isdigit(c)) {
        return number();
    }

    if (std::isalpha(c) || c == '_') {
        return identifier();
    }

    return errorToken("Unexpected character.");
}

Token Lexer::string(char quote) {
    while (peek() != quote && !isAtEnd()) {
        if (peek() == '\n') line_++;
        advance();
    }

    if (isAtEnd()) return errorToken("Unterminated string.");

    // The closing quote.
    advance();
    return makeToken(TokenType::STRING);
}

Token Lexer::number() {
    while (std::isdigit(peek())) advance();

    // Look for a fractional part.
    if (peek() == '.' && std::isdigit(peekNext())) {
        // Consume the ".".
        advance();

        while (std::isdigit(peek())) advance();
    }

    return makeToken(TokenType::NUMBER);
}

Token Lexer::identifier() {
    while (std::isalnum(peek()) || peek() == '_' || peek() == '-') advance();
    return makeToken(identifierType());
}

TokenType Lexer::identifierType() {
    static const std::unordered_map<std::string_view, TokenType> keywords = {
        {"text", TokenType::TEXT},
        {"style", TokenType::STYLE},
        {"script", TokenType::SCRIPT},
        {"if", TokenType::IF},
        {"else", TokenType::ELSE},
        {"inherit", TokenType::INHERIT},
        {"delete", TokenType::DELETE},
        {"insert", TokenType::INSERT},
        {"after", TokenType::AFTER},
        {"before", TokenType::BEFORE},
        {"replace", TokenType::REPLACE},
        {"at", TokenType::IDENTIFIER}, // "at" is a prefix for "at top" and "at bottom"
        {"from", TokenType::FROM},
        {"as", TokenType::AS},
        {"except", TokenType::EXCEPT},
        {"use", TokenType::USE},
        {"html5", TokenType::HTML5},
    };

    auto it = keywords.find(source_.substr(start_, current_ - start_));
    if (it != keywords.end()) {
        if (it->first == "at") {
            if (source_.substr(current_, 4) == " top") {
                current_ += 4;
                return TokenType::AT_TOP;
            } else if (source_.substr(current_, 7) == " bottom") {
                current_ += 7;
                return TokenType::AT_BOTTOM;
            }
        }
        return it->second;
    }

    return TokenType::IDENTIFIER;
}

Token Lexer::blockKeyword() {
    while (peek() != ']' && !isAtEnd()) advance();
    if (isAtEnd()) return errorToken("Unterminated block keyword.");
    advance(); // consume the ']'

    std::string_view block = source_.substr(start_, current_ - start_);
    if (block == "[Template]") return makeToken(TokenType::BLOCK_TEMPLATE);
    if (block == "[Custom]") return makeToken(TokenType::BLOCK_CUSTOM);
    if (block == "[Origin]") return makeToken(TokenType::BLOCK_ORIGIN);
    if (block == "[Import]") return makeToken(TokenType::BLOCK_IMPORT);
    if (block == "[Namespace]") return makeToken(TokenType::BLOCK_NAMESPACE);
    if (block == "[Configuration]") return makeToken(TokenType::BLOCK_CONFIGURATION);
    if (block == "[Info]") return makeToken(TokenType::BLOCK_INFO);
    if (block == "[Export]") return makeToken(TokenType::BLOCK_EXPORT);
    if (block == "[Name]") return makeToken(TokenType::BLOCK_NAME);

    return errorToken("Unknown block keyword.");
}

bool Lexer::isAtEnd() const {
    return current_ >= source_.length();
}

char Lexer::advance() {
    return source_[current_++];
}

char Lexer::peek() const {
    if (isAtEnd()) return '\0';
    return source_[current_];
}

char Lexer::peekNext() const {
    if (current_ + 1 >= source_.length()) return '\0';
    return source_[current_ + 1];
}

bool Lexer::match(char expected) {
    if (isAtEnd()) return false;
    if (source_[current_] != expected) return false;
    current_++;
    return true;
}

Token Lexer::makeToken(TokenType type) const {
    return Token{type, source_.substr(start_, current_ - start_), start_, line_};
}

Token Lexer::errorToken(const char* message) const {
    return Token{TokenType::ERROR, message, start_, line_};
}

void Lexer::skipWhitespace() {
    for (;;) {
        char c = peek();
        switch (c) {
            case ' ':
            case '\r':
            case '\t':
                advance();
                break;
            case '\n':
                line_++;
                advance();
                break;
            default:
                return;
        }
    }
}

} // namespace CHTL
