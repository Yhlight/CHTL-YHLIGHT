#include "Parser.h"
#include <stdexcept>

Parser::Parser(const std::vector<Token>& tokens) : tokens(tokens) {}

std::unique_ptr<ASTNode> Parser::parse() {
    return parseStatement();
}

std::unique_ptr<ASTNode> Parser::parseStatement() {
    if (peek().type == TokenType::Text) {
        return parseTextNode();
    }
    if (peek().type == TokenType::Identifier) {
        return parseElementNode();
    }
    if (peek().type == TokenType::Style) {
        return parseStyleNode();
    }
    return nullptr;
}

std::unique_ptr<TextNode> Parser::parseTextNode() {
    advance(); // Consume 'text' token

    if (peek().type != TokenType::LBrace) {
        throw std::runtime_error("Expected '{' after 'text'");
    }
    advance(); // Consume '{'

    if (peek().type != TokenType::String) {
        throw std::runtime_error("Expected a string literal inside text block");
    }
    std::string text = advance().value;

    if (peek().type != TokenType::RBrace) {
        throw std::runtime_error("Expected '}' after text block");
    }
    advance(); // Consume '}'

    return std::make_unique<TextNode>(text);
}

std::unique_ptr<ElementNode> Parser::parseElementNode() {
    std::string tagName = advance().value; // Consume identifier
    auto node = std::make_unique<ElementNode>(tagName);

    if (peek().type != TokenType::LBrace) {
        throw std::runtime_error("Expected '{' after element tag");
    }
    advance(); // Consume '{'

    while (peek().type != TokenType::RBrace && !isAtEnd()) {
        if (peek().type == TokenType::Identifier && (current + 1 < tokens.size() && (tokens[current+1].type == TokenType::Colon || tokens[current+1].type == TokenType::Equal))) {
             std::string attrName = advance().value;

            if (peek().type != TokenType::Colon && peek().type != TokenType::Equal) {
                throw std::runtime_error("Expected ':' or '=' after attribute name");
            }
            advance(); // consume ':' or '='

            if (peek().type != TokenType::String) {
                throw std::runtime_error("Expected attribute value");
            }
            std::string attrValue = advance().value;

            node->attributes[attrName] = attrValue;

            if (peek().type != TokenType::Semicolon) {
                throw std::runtime_error("Expected ';' after attribute value");
            }
            advance(); // consume ';'
        } else {
            node->children.push_back(parseStatement());
        }
    }

    if (peek().type != TokenType::RBrace) {
        throw std::runtime_error("Expected '}' after element block");
    }
    advance(); // Consume '}'

    return node;
}

std::unique_ptr<StyleNode> Parser::parseStyleNode() {
    advance(); // Consume 'style' token
    auto node = std::make_unique<StyleNode>();

    if (peek().type != TokenType::LBrace) {
        throw std::runtime_error("Expected '{' after 'style'");
    }
    advance(); // Consume '{'

    while (peek().type != TokenType::RBrace && !isAtEnd()) {
        if (peek().type != TokenType::Identifier) {
            throw std::runtime_error("Expected style property name");
        }
        std::string propName = advance().value;

        if (peek().type != TokenType::Colon) {
            throw std::runtime_error("Expected ':' after style property name");
        }
        advance(); // consume ':'

        if (peek().type != TokenType::Value && peek().type != TokenType::String) {
            throw std::runtime_error("Expected style property value");
        }
        std::string propValue = advance().value;

        node->properties[propName] = {propValue};

        if (peek().type != TokenType::Semicolon) {
            throw std::runtime_error("Expected ';' after style property value");
        }
        advance(); // consume ';'
    }

    if (peek().type != TokenType::RBrace) {
        throw std::runtime_error("Expected '}' after style block");
    }
    advance(); // Consume '}'

    return node;
}


Token Parser::peek() {
    return tokens[current];
}

Token Parser::advance() {
    if (!isAtEnd()) current++;
    return tokens[current - 1];
}

bool Parser::isAtEnd() {
    return peek().type == TokenType::EndOfFile;
}
