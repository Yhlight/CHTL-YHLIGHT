#include "StyleParser.h"
#include <map>

StyleParser::StyleParser(std::vector<Token> tokens) : tokens(std::move(tokens)) {}

std::vector<std::unique_ptr<StyleContentNode>> StyleParser::parse() {
    std::vector<std::unique_ptr<StyleContentNode>> nodes;
    while (!isAtEnd()) {
        if (peek().type == TokenType::StyleSpecifier) {
            advance(); // consume '@Style'
            auto directiveNode = std::make_unique<StyleDirectiveNode>();
            directiveNode->directive_name = "@Style";
            if (peek().type == TokenType::Identifier) {
                directiveNode->template_name = advance().value;
            }
            if (peek().type == TokenType::OpenBrace) {
                advance(); // consume '{'
                while (peek().type != TokenType::CloseBrace && !isAtEnd()) {
                    if (peek().type == TokenType::Identifier && tokens[current + 1].type == TokenType::Colon) {
                        std::string key = advance().value;
                        advance(); // consume ':'
                        if (peek().type == TokenType::StringLiteral || peek().type == TokenType::Identifier || peek().type == TokenType::NumericLiteral) {
                            directiveNode->properties[key] = advance().value;
                        }
                        if (peek().type == TokenType::Semicolon) {
                            advance(); // consume ';'
                        }
                    } else {
                        advance();
                    }
                }
                advance(); // consume '}'
            } else if (peek().type == TokenType::Semicolon) {
                advance(); // consume ';'
            }
            nodes.push_back(std::move(directiveNode));
        } else {
            auto rawNode = std::make_unique<RawStyleContentNode>();
            std::string raw_css;
            while (!isAtEnd() && peek().type != TokenType::StyleSpecifier) {
                Token token = advance();
                raw_css += token.reconstruct();
                if (token.type == TokenType::Colon) {
                    raw_css += " ";
                }
            }
            rawNode->raw_css = raw_css;
            nodes.push_back(std::move(rawNode));
        }
    }
    return nodes;
}

Token StyleParser::advance() {
    if (!isAtEnd()) current++;
    return tokens[current - 1];
}

Token StyleParser::peek() {
    return tokens[current];
}

bool StyleParser::isAtEnd() {
    return peek().type == TokenType::EndOfFile;
}
