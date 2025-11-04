#include "Parser.h"
#include "../CHTLNode/ElementNode.h"
#include "../CHTLNode/TextNode.h"
#include "../CHTLNode/StyleNode.h"
#include "../CHTLNode/ScriptNode.h"
#include <stdexcept>

namespace CHTL {

Parser::Parser(std::string_view source)
    : source_(source), lexer_(source) {}

std::unique_ptr<ProgramNode> Parser::parse() {
    auto programNode = std::make_unique<ProgramNode>();
    advance();
    while (!match(TokenType::END_OF_FILE)) {
        programNode->children.push_back(parseStatement());
    }
    return programNode;
}

void Parser::advance() {
    previousToken_ = currentToken_;
    for (;;) {
        currentToken_ = lexer_.scanToken();
        if (currentToken_.type != TokenType::ERROR) break;

        throw std::runtime_error(std::string(currentToken_.lexeme));
    }
}

void Parser::consume(TokenType type, const char* message) {
    if (check(type)) {
        advance();
        return;
    }
    throw std::runtime_error(message);
}

bool Parser::match(TokenType type) {
    if (!check(type)) return false;
    advance();
    return true;
}

bool Parser::check(TokenType type) const {
    return currentToken_.type == type;
}

const Token& Parser::previous() const {
    return previousToken_;
}

const Token& Parser::peek() const {
    return currentToken_;
}
std::unique_ptr<ASTNode> Parser::parseStatement() {
    if (check(TokenType::TEXT)) {
        return parseText();
    } else if (check(TokenType::STYLE)) {
        return parseStyle();
    } else if (check(TokenType::SCRIPT)) {
        return parseScript();
    } else if (check(TokenType::IDENTIFIER)) {
        return parseElement();
    }
    throw std::runtime_error("Expected a statement.");
}

std::unique_ptr<StyleNode> Parser::parseStyle() {
    consume(TokenType::STYLE, "Expect 'style' keyword.");
    consume(TokenType::LEFT_BRACE, "Expect '{' after 'style' keyword.");

    size_t start = previousToken_.start_pos + previousToken_.lexeme.length();
    while (!check(TokenType::RIGHT_BRACE) && !check(TokenType::END_OF_FILE)) {
        advance();
    }
    size_t end = currentToken_.start_pos;

    consume(TokenType::RIGHT_BRACE, "Expect '}' after style block.");

    return std::make_unique<StyleNode>(std::string(source_.substr(start, end - start)));
}

std::unique_ptr<ScriptNode> Parser::parseScript() {
    consume(TokenType::SCRIPT, "Expect 'script' keyword.");
    consume(TokenType::LEFT_BRACE, "Expect '{' after 'script' keyword.");

    size_t start = previousToken_.start_pos + previousToken_.lexeme.length();
    while (!check(TokenType::RIGHT_BRACE) && !check(TokenType::END_OF_FILE)) {
        advance();
    }
    size_t end = currentToken_.start_pos;

    consume(TokenType::RIGHT_BRACE, "Expect '}' after script block.");

    return std::make_unique<ScriptNode>(std::string(source_.substr(start, end - start)));
}

std::unique_ptr<ElementNode> Parser::parseElement() {
    consume(TokenType::IDENTIFIER, "Expect element name.");
    auto element = std::make_unique<ElementNode>(std::string(previous().lexeme));

    consume(TokenType::LEFT_BRACE, "Expect '{' after element name.");

    while (!check(TokenType::RIGHT_BRACE) && !check(TokenType::END_OF_FILE)) {
        if (check(TokenType::IDENTIFIER)) {
            Token nextToken = lexer_.peekToken();
            if (nextToken.type == TokenType::COLON || nextToken.type == TokenType::EQUAL) {
                parseAttributes(*element);
            } else {
                element->children.push_back(parseStatement());
            }
        } else {
            element->children.push_back(parseStatement());
        }
    }

    consume(TokenType::RIGHT_BRACE, "Expect '}' after element block.");

    return element;
}

void Parser::parseAttributes(ElementNode& element) {
    consume(TokenType::IDENTIFIER, "Expect attribute name.");
    std::string name = std::string(previous().lexeme);

    if (!match(TokenType::COLON) && !match(TokenType::EQUAL)) {
        throw std::runtime_error("Expect ':' or '=' after attribute name.");
    }

    if (match(TokenType::STRING)) {
        std::string_view lexeme = previous().lexeme;
        auto value = lexeme.substr(1, lexeme.length() - 2); // Remove quotes
        element.attributes[name] = std::string(value);
    } else if (match(TokenType::IDENTIFIER) || match(TokenType::NUMBER)) {
        element.attributes[name] = std::string(previous().lexeme);
    } else {
        throw std::runtime_error("Expect attribute value.");
    }

    consume(TokenType::SEMICOLON, "Expect ';' after attribute value.");
}

std::unique_ptr<TextNode> Parser::parseText() {
    consume(TokenType::TEXT, "Expect 'text' keyword.");
    consume(TokenType::LEFT_BRACE, "Expect '{' after 'text' keyword.");
    consume(TokenType::STRING, "Expect string literal in text block.");
    std::string_view lexeme = previous().lexeme;
    auto content = lexeme.substr(1, lexeme.length() - 2); // Remove quotes
    auto text = std::make_unique<TextNode>(std::string(content));
    consume(TokenType::RIGHT_BRACE, "Expect '}' after text block.");
    return text;
}

} // namespace CHTL
