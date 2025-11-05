#include "Parser.h"

Parser::Parser(const std::vector<Token>& tokens) : tokens(tokens), current_token_index(0) {}

std::unique_ptr<BaseNode> Parser::parse() {
    if (current_token().type == TokenType::Identifier) {
        return parse_element();
    }
    return nullptr;
}

std::unique_ptr<ElementNode> Parser::parse_element() {
    auto node = std::make_unique<ElementNode>(current_token().value);
    advance(); // Consume the identifier

    if (current_token().type != TokenType::OpenBrace) {
        // Handle error: expected '{'
        return nullptr;
    }
    advance(); // Consume the '{'

    while (current_token().type != TokenType::CloseBrace && current_token().type != TokenType::EndOfFile) {
        if (current_token().type == TokenType::Identifier && current_token().value == "text") {
            advance(); // Consume "text"
            if (current_token().type != TokenType::Colon) {
                // Handle error: expected ':'
                return nullptr;
            }
            advance(); // Consume ':'

            if (current_token().type != TokenType::StringLiteral) {
                // Handle error: expected a string literal
                return nullptr;
            }
            node->children.push_back(std::make_unique<TextNode>(current_token().value));
            advance(); // Consume the string literal

            if (current_token().type != TokenType::Semicolon) {
                // Handle error: expected ';'
                return nullptr;
            }
            advance(); // Consume ';'
        } else {
            // Handle other attributes or children
            advance();
        }
    }

    if (current_token().type != TokenType::CloseBrace) {
        // Handle error: expected '}'
        return nullptr;
    }
    advance(); // Consume the '}'

    return node;
}

Token Parser::current_token() {
    if (current_token_index < tokens.size()) {
        return tokens[current_token_index];
    }
    return {TokenType::EndOfFile, "", 0, 0};
}

void Parser::advance() {
    if (current_token_index < tokens.size()) {
        current_token_index++;
    }
}
