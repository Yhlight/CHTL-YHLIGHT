#include "Lexer.h"
#include <map>

namespace CHTL {

static std::map<std::string, TokenType> keywords = {
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
    {"at", TokenType::AT},
    {"top", TokenType::TOP},
    {"bottom", TokenType::BOTTOM},
    {"from", TokenType::FROM},
    {"as", TokenType::AS},
    {"except", TokenType::EXCEPT},
    {"use", TokenType::USE},
    {"html5", TokenType::HTML5},
    {"condition", TokenType::CONDITION}
};

static std::map<std::string, TokenType> atKeywords = {
    {"@Style", TokenType::AT_STYLE},
    {"@Element", TokenType::AT_ELEMENT},
    {"@Var", TokenType::AT_VAR},
    {"@Html", TokenType::AT_HTML},
    {"@JavaScript", TokenType::AT_JAVASCRIPT},
    {"@Chtl", TokenType::AT_CHTL},
    {"@Config", TokenType::AT_CONFIG}
};


Lexer::Lexer(std::string_view source) : source_(source) {}

std::vector<Token> Lexer::scanTokens() {
    while (!isAtEnd()) {
        start_ = current_;
        scanToken();
    }

    tokens_.push_back({TokenType::END_OF_FILE, "", line_, source_.length()});
    return tokens_;
}

bool Lexer::isAtEnd() {
    return current_ >= source_.length();
}

void Lexer::scanToken() {
    char c = advance();
    switch (c) {
        case '{': addToken(TokenType::LEFT_BRACE); break;
        case '}': addToken(TokenType::RIGHT_BRACE); break;
        case '(': addToken(TokenType::LEFT_PAREN); break;
        case ')': addToken(TokenType::RIGHT_PAREN); break;
        case '[':
            if (isAlpha(peek())) {
                blockKeyword();
            } else {
                addToken(TokenType::LEFT_BRACKET);
            }
            break;
        case ']': addToken(TokenType::RIGHT_BRACKET); break;
        case ':': addToken(TokenType::COLON); break;
        case ';': addToken(TokenType::SEMICOLON); break;
        case ',': addToken(TokenType::COMMA); break;
        case '.': addToken(TokenType::DOT); break;
        case '?': addToken(TokenType::QUESTION); break;
        case '+': addToken(TokenType::PLUS); break;
        case '-': addToken(match('>') ? TokenType::ARROW : TokenType::MINUS); break;
        case '*': addToken(match('*') ? TokenType::STAR_STAR : TokenType::STAR); break;
        case '%': addToken(TokenType::PERCENT); break;
        case '>': addToken(TokenType::GREATER); break;
        case '<': addToken(TokenType::LESS); break;
        case '=': addToken(TokenType::EQUAL); break;
        case '&':
            if (match('&')) {
                addToken(TokenType::AMPERSAND_AMPERSAND);
            } else if (match('-')) {
                if (match('>')) {
                    addToken(TokenType::BIND_ARROW);
                }
            }
            else {
                addToken(TokenType::AMPERSAND);
            }
            break;
        case '|': if (match('|')) { addToken(TokenType::PIPE_PIPE); } break;
        case '#':
            while (peek() != '\n' && !isAtEnd()) advance();
            addToken(TokenType::GEN_COMMENT);
            break;
        case '/':
            if (match('/')) {
                while (peek() != '\n' && !isAtEnd()) advance();
                 addToken(TokenType::COMMENT);
            } else if (match('*')) {
                while (!(peek() == '*' && peekNext() == '/') && !isAtEnd()) {
                    if (peek() == '\n') line_++;
                    advance();
                }
                if (!isAtEnd()) {
                    advance(); // Consume '*'
                    advance(); // Consume '/'
                }
                 addToken(TokenType::COMMENT);
            } else {
                addToken(TokenType::SLASH);
            }
            break;

        case ' ':
        case '\r':
        case '\t':
            // Ignore whitespace
            break;
        case '\n':
            line_++;
            break;

        case '"':
        case '\'':
            string(c);
            break;

        default:
            if (isDigit(c)) {
                number();
            } else if (isAlpha(c) || c == '@') {
                identifier();
            } else {
                addToken(TokenType::UNKNOWN);
            }
            break;
    }
}

char Lexer::advance() {
    return source_[current_++];
}

bool Lexer::match(char expected) {
    if (isAtEnd()) return false;
    if (source_[current_] != expected) return false;
    current_++;
    return true;
}

char Lexer::peek() {
    if (isAtEnd()) return '\0';
    return source_[current_];
}

char Lexer::peekNext() {
    if (current_ + 1 >= source_.length()) return '\0';
    return source_[current_ + 1];
}

void Lexer::addToken(TokenType type) {
    addToken(type, source_.substr(start_, current_ - start_));
}

void Lexer::addToken(TokenType type, std::string_view lexeme) {
    tokens_.push_back({type, lexeme, line_, start_});
}

void Lexer::string(char quote) {
    while (peek() != quote && !isAtEnd()) {
        if (peek() == '\n') line_++;
        advance();
    }

    if (isAtEnd()) {
        // Unterminated string.
        addToken(TokenType::UNKNOWN, "Unterminated string.");
        return;
    }

    advance(); // The closing quote.

    // Trim the surrounding quotes.
    std::string_view value = source_.substr(start_ + 1, current_ - start_ - 2);
    addToken(TokenType::STRING, value);
}

void Lexer::number() {
    while (isDigit(peek())) advance();

    // Look for a fractional part.
    if (peek() == '.' && isDigit(peekNext())) {
        // Consume the "."
        advance();
        while (isDigit(peek())) advance();
    }

    // Also handle units like px, em, etc. which might follow a number
    while (isAlpha(peek())) advance();

    addToken(TokenType::NUMBER);
}

void Lexer::identifier() {
    bool isAtKeyword = source_[start_] == '@';
    while (isAlphaNumeric(peek()) || (peek() == '-' && isAlphaNumeric(peekNext()))) {
        advance();
    }

    std::string text = std::string(source_.substr(start_, current_ - start_));
    TokenType type;

    if (isAtKeyword) {
        auto it = atKeywords.find(text);
        type = (it == atKeywords.end()) ? TokenType::IDENTIFIER : it->second;
    } else {
        auto it = keywords.find(text);
        type = (it == keywords.end()) ? TokenType::IDENTIFIER : it->second;
    }

    addToken(type);
}

void Lexer::blockKeyword() {
    // We've already consumed the '['. Now, we scan until the matching ']'.
    while (peek() != ']' && !isAtEnd()) {
        advance();
    }

    if (isAtEnd()) {
        addToken(TokenType::UNKNOWN, "Unterminated block keyword.");
        return;
    }

    advance(); // The closing ']'.

    std::string_view keyword = source_.substr(start_, current_ - start_);
    if (keyword == "[Custom]") addToken(TokenType::KEYWORD_CUSTOM);
    else if (keyword == "[Template]") addToken(TokenType::KEYWORD_TEMPLATE);
    else if (keyword == "[Origin]") addToken(TokenType::KEYWORD_ORIGIN);
    else if (keyword == "[Import]") addToken(TokenType::KEYWORD_IMPORT);
    else if (keyword == "[Namespace]") addToken(TokenType::KEYWORD_NAMESPACE);
    else if (keyword == "[Configuration]") addToken(TokenType::KEYWORD_CONFIGURATION);
    else if (keyword == "[Info]") addToken(TokenType::KEYWORD_INFO);
    else if (keyword == "[Export]") addToken(TokenType::KEYWORD_EXPORT);
    else if (keyword == "[Name]") addToken(TokenType::KEYWORD_NAME);
    else if (keyword == "[OriginType]") addToken(TokenType::KEYWORD_ORIGIN_TYPE);

    else addToken(TokenType::UNKNOWN, "Unknown block keyword.");
}


// Helper implementations
bool Lexer::isDigit(char c) {
    return c >= '0' && c <= '9';
}

bool Lexer::isAlpha(char c) {
    return (c >= 'a' && c <= 'z') ||
           (c >= 'A' && c <= 'Z') ||
            c == '_';
}

bool Lexer::isAlphaNumeric(char c) {
    return isAlpha(c) || isDigit(c);
}

} // namespace CHTL
