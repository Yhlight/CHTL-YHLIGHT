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
        if (current_token().type == TokenType::Identifier) {
            std::string key = current_token().value;
            advance(); // Consume the identifier

            if (current_token().type != TokenType::Colon && current_token().type != TokenType::Equal) {
                // Handle error: expected ':' or '='
                return nullptr;
            }
            advance(); // Consume the ':' or '='

            if (current_token().type != TokenType::StringLiteral) {
                // Handle error: expected a string literal
                return nullptr;
            }
            std::string value = current_token().value;
            advance(); // Consume the string literal

            if (current_token().type != TokenType::Semicolon) {
                // Handle error: expected ';'
                return nullptr;
            }
            advance(); // Consume ';'

            if (key == "text") {
                node->children.push_back(std::make_unique<TextNode>(value));
            } else {
                node->attributes[key] = value;
            }
        } else {
            // Handle other children
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
