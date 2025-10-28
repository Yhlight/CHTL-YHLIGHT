#include "Parser.h"
#include <stdexcept>

namespace CHTL {

Parser::Parser(std::vector<Token> tokens) : m_tokens(std::move(tokens)) {}

std::unique_ptr<ProgramNode> Parser::parse() {
    auto program = std::make_unique<ProgramNode>();
    while (!isAtEnd()) {
        program->children.push_back(parseStatement());
    }
    return program;
}

std::unique_ptr<ASTNode> Parser::parseStatement() {
    if (peek().type == TokenType::IDENTIFIER && m_tokens[m_current + 1].type == TokenType::LEFT_BRACE) {
        return parseElementNode();
    }
    // For now, we only support element nodes at the top level.
    // We can add text nodes, templates, etc. later.
    // A simple text node example: text { "content" }
    if (peek().type == TokenType::TEXT) {
         return parseTextNode();
    }

    // If we don't recognize the token, we have a syntax error.
    // A more robust parser would have error recovery.
    throw std::runtime_error("Unexpected token in parseStatement: " + peek().lexeme);
}

std::unique_ptr<ElementNode> Parser::parseElementNode() {
    auto node = std::make_unique<ElementNode>();
    node->tagName = consume(TokenType::IDENTIFIER, "Expect element tag name.").lexeme;
    consume(TokenType::LEFT_BRACE, "Expect '{' after element tag name.");

    while (!check(TokenType::RIGHT_BRACE) && !isAtEnd()) {
        // Inside an element, we can have attributes or child elements.
        if (peek().type == TokenType::IDENTIFIER && m_tokens[m_current + 1].type == TokenType::COLON) {
            parseAttributes(*node);
        } else {
            node->children.push_back(parseStatement());
        }
    }

    consume(TokenType::RIGHT_BRACE, "Expect '}' after element block.");
    return node;
}

std::unique_ptr<TextNode> Parser::parseTextNode() {
    consume(TokenType::TEXT, "Expect 'text' keyword.");
    consume(TokenType::LEFT_BRACE, "Expect '{' after 'text' keyword.");
    auto node = std::make_unique<TextNode>();
    if (peek().type == TokenType::STRING) {
        node->content = advance().lexeme;
    } else if (peek().type == TokenType::IDENTIFIER || peek().type == TokenType::UNQUOTED_LITERAL) {
        // Handle unquoted literals as text content
        node->content = advance().lexeme;
    } else {
        throw std::runtime_error("Expect string or unquoted literal inside text block.");
    }
    consume(TokenType::RIGHT_BRACE, "Expect '}' after text content.");
    return node;
}


void Parser::parseAttributes(ElementNode& node) {
    std::string key = consume(TokenType::IDENTIFIER, "Expect attribute key.").lexeme;
    consume(TokenType::COLON, "Expect ':' after attribute key.");

    // Attribute value can be a string, identifier (unquoted literal), or number.
    const Token& valueToken = advance();
    if (valueToken.type != TokenType::STRING && valueToken.type != TokenType::IDENTIFIER && valueToken.type != TokenType::NUMBER && valueToken.type != TokenType::UNQUOTED_LITERAL) {
        throw std::runtime_error("Expect attribute value (string, identifier, or number).");
    }

    node.attributes.push_back({key, valueToken.lexeme});
    consume(TokenType::SEMICOLON, "Expect ';' after attribute value.");
}


// --- Helper Methods ---

const Token& Parser::advance() {
    if (!isAtEnd()) m_current++;
    return previous();
}

const Token& Parser::peek() const {
    return m_tokens[m_current];
}

const Token& Parser::previous() const {
    return m_tokens[m_current - 1];
}

bool Parser::isAtEnd() const {
    return peek().type == TokenType::END_OF_FILE;
}

bool Parser::check(TokenType type) const {
    if (isAtEnd()) return false;
    return peek().type == type;
}

bool Parser::match(const std::vector<TokenType>& types) {
    for (TokenType type : types) {
        if (check(type)) {
            advance();
            return true;
        }
    }
    return false;
}

const Token& Parser::consume(TokenType type, const std::string& message) {
    if (check(type)) return advance();
    throw std::runtime_error(message);
}

} // namespace CHTL
