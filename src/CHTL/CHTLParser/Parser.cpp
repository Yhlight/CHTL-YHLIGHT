#include "Parser.h"
#include "CHTL/CHTLNode/ProgramNode.h"
#include "CHTL/CHTLNode/TextNode.h"
#include "CHTL/CHTLNode/ElementNode.h"
#include "CHTL/CHTLNode/StyleNode.h"
#include "CHTL/CHTLNode/StylePropertyNode.h"
#include <stdexcept>

namespace CHTL {

Parser::Parser(const std::vector<Token>& tokens, std::string_view source) : m_tokens(tokens), m_source(source) {}

std::unique_ptr<ASTNode> Parser::parse() {
    auto programNode = std::make_unique<ProgramNode>();
    while (!isAtEnd()) {
        programNode->children.push_back(parseStatement());
    }
    return programNode;
}

const Token& Parser::peek() { return m_tokens[m_current]; }

const Token& Parser::previous() { return m_tokens[m_current - 1]; }

bool Parser::isAtEnd() { return peek().type == TokenType::Eof; }

const Token& Parser::advance() {
    if (!isAtEnd()) m_current++;
    return previous();
}

bool Parser::check(TokenType type) {
    if (isAtEnd()) return false;
    return peek().type == type;
}

const Token& Parser::consume(TokenType type, const std::string& message) {
    if (check(type)) return advance();
    throw std::runtime_error(message);
}

std::unique_ptr<ASTNode> Parser::parseText() {
    consume(TokenType::Text, "Expect 'text' keyword.");
    const Token& leftBrace = consume(TokenType::LeftBrace, "Expect '{' after 'text' keyword.");

    int startPos = leftBrace.start_pos + leftBrace.lexeme.length();

    int lookahead = m_current;
    while (m_tokens[lookahead].type != TokenType::RightBrace && m_tokens[lookahead].type != TokenType::Eof) {
        lookahead++;
    }

    if (m_tokens[lookahead].type == TokenType::Eof) {
        throw std::runtime_error("Unterminated text block.");
    }

    const Token& rightBrace = m_tokens[lookahead];
    int endPos = rightBrace.start_pos;

    auto textNode = std::make_unique<TextNode>();
    textNode->content = m_source.substr(startPos, endPos - startPos);

    m_current = lookahead;
    consume(TokenType::RightBrace, "Expect '}' after text block.");

    return textNode;
}

std::unique_ptr<ASTNode> Parser::parseStatement() {
    if (check(TokenType::Text)) {
        return parseText();
    }
    if (check(TokenType::Identifier)) {
        return parseElement();
    }
    if (check(TokenType::Style)) {
        return parseStyle();
    }
    // For now, just advance to avoid infinite loops on unknown tokens
    advance();
    return nullptr;
}

std::unique_ptr<ASTNode> Parser::parseStyle() {
    consume(TokenType::Style, "Expect 'style' keyword.");
    consume(TokenType::LeftBrace, "Expect '{' after 'style' keyword.");

    auto styleNode = std::make_unique<StyleNode>();

    while (!check(TokenType::RightBrace) && !isAtEnd()) {
        auto propNode = std::make_unique<StylePropertyNode>();
        propNode->name = std::string(consume(TokenType::Identifier, "Expect property name.").lexeme);
        consume(TokenType::Colon, "Expect ':' after property name.");

        std::string value = "";
        while (!check(TokenType::Semicolon) && !isAtEnd()) {
            value += advance().lexeme;
        }
        propNode->value = value;

        consume(TokenType::Semicolon, "Expect ';' after property value.");
        styleNode->properties.push_back(std::move(propNode));
    }

    consume(TokenType::RightBrace, "Expect '}' after style block.");

    return styleNode;
}

std::unique_ptr<ASTNode> Parser::parseElement() {
    auto elementNode = std::make_unique<ElementNode>();
    elementNode->tagName = advance().lexeme;

    consume(TokenType::LeftBrace, "Expect '{' after element tag.");

    parseAttributes(*elementNode);

    while (!check(TokenType::RightBrace) && !isAtEnd()) {
        elementNode->children.push_back(parseStatement());
    }

    consume(TokenType::RightBrace, "Expect '}' after element block.");

    return elementNode;
}

void Parser::parseAttributes(ElementNode& element) {
    while (check(TokenType::Identifier) && (m_tokens[m_current + 1].type == TokenType::Colon || m_tokens[m_current + 1].type == TokenType::Equals)) {
        std::string name(advance().lexeme);
        advance(); // consume ':' or '='
        std::string value(advance().lexeme);
        consume(TokenType::Semicolon, "Expect ';' after attribute value.");
        element.attributes[name] = value;
    }
}

} // namespace CHTL
