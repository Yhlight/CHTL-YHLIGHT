#include "Parser.h"
#include <stdexcept>

Parser::Parser(const std::vector<Token>& tokens) : tokens(tokens) {}

std::unique_ptr<ASTNode> Parser::parse() {
    if (peek().type == TokenType::Text) {
        return parseTextNode();
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
