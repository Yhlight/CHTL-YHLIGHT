#include "Parser.h"
#include "CHTLNode/ElementNode.h"
#include "CHTLNode/TextNode.h"

Parser::Parser(std::vector<Token> tokens) : tokens(std::move(tokens)) {}

AstNode Parser::parse() {
    return parseElement();
}

AstNode Parser::parseElement() {
    auto element = std::make_unique<ElementNode>();

    if (peek().type == TokenType::Identifier) {
        element->tag_name = advance().value;
    }

    if (peek().type == TokenType::OpenBrace) {
        advance(); // consume '{'
        while (peek().type != TokenType::CloseBrace && !isAtEnd()) {
            if (peek().type == TokenType::Identifier) {
                std::string key = advance().value;
                if (peek().type == TokenType::Colon) {
                    advance(); // consume ':'
                    if (peek().type == TokenType::StringLiteral) {
                        element->attributes[key] = advance().value;
                    }
                    if (peek().type == TokenType::Semicolon) {
                        advance(); // consume ';'
                    }
                } else {
                    // It's a nested element
                    current--; // backtrack
                    element->children.push_back(parseElement());
                }
            } else if (peek().type == TokenType::TextKeyword) {
                 advance(); // consume 'text'
                 if(peek().type == TokenType::OpenBrace){
                    advance(); // consume '{'
                    auto textNode = std::make_unique<TextNode>();
                    if (peek().type == TokenType::StringLiteral) {
                        textNode->text = advance().value;
                    }
                    element->children.push_back(std::move(textNode));
                    if (peek().type == TokenType::CloseBrace) {
                        advance(); // consume '}'
                    }
                 }
            }else {
                // To avoid infinite loops on unexpected tokens.
                advance();
            }
        }
        advance(); // consume '}'
    }

    return element;
}

Token Parser::advance() {
    return tokens[current++];
}

Token Parser::peek() {
    return tokens[current];
}

bool Parser::isAtEnd() {
    return peek().type == TokenType::EndOfFile;
}
