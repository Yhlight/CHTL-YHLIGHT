#include "Parser.h"
#include "CHTLNode/ProgramNode.h"
#include "CHTLNode/TextNode.h"
#include "CHTLNode/ElementNode.h"
#include "CHTLNode/StyleNode.h"
#include "CHTLNode/StylePropertyNode.h"
#include "CHTLNode/LiteralValueNode.h"
#include <iostream>
#include <string>
#include <vector>

Parser::Parser(Lexer& lexer) : lexer_(lexer) {}

void Parser::advance() {
    previous_ = current_;
    for (;;) {
        current_ = lexer_.scanToken();
        if (current_.type != TokenType::UNRECOGNIZED) break;
        std::cerr << "Error: Unexpected character on line " << current_.line << std::endl;
        hadError_ = true;
    }
}

bool Parser::match(TokenType type) {
    if (check(type)) {
        advance();
        return true;
    }
    return false;
}

void Parser::consume(TokenType type, const std::string& message) {
    if (check(type)) {
        advance();
        return;
    }
    std::cerr << "Error: " << message << " on line " << current_.line << std::endl;
    hadError_ = true;
}

bool Parser::check(TokenType type) const {
    return current_.type == type;
}

const Token& Parser::peek() const {
    return current_;
}

std::unique_ptr<ASTNode> Parser::declaration() {
    return statement();
}

std::unique_ptr<ASTNode> Parser::parseText() {
    consume(TokenType::LEFT_BRACE, "Expect '{' after 'text'.");
    std::string text_content;
    if (check(TokenType::STRING)) {
        text_content = std::string(current_.lexeme);
        if (text_content.length() >= 2) {
            text_content = text_content.substr(1, text_content.length() - 2);
        }
        advance();
    }
    consume(TokenType::RIGHT_BRACE, "Expect '}' after text content.");
    return std::make_unique<TextNode>(std::move(text_content));
}

std::unique_ptr<ASTNode> Parser::parseStyleProperty() {
    if (!check(TokenType::IDENTIFIER)) {
        hadError_ = true;
        std::cerr << "Error: Expect property name on line " << current_.line << std::endl;
        return nullptr;
    }
    std::string name = std::string(current_.lexeme);
    advance();

    consume(TokenType::COLON, "Expect ':' after property name.");

    std::vector<Token> value_tokens;
    while (!check(TokenType::SEMICOLON) && !check(TokenType::END_OF_FILE)) {
        value_tokens.push_back(current_);
        advance();
    }

    std::string valueStr;
    for (size_t i = 0; i < value_tokens.size(); ++i) {
        std::string lexeme_str;
        if (value_tokens[i].type == TokenType::STRING) {
            lexeme_str = std::string(value_tokens[i].lexeme);
            if (lexeme_str.length() >= 2) {
                lexeme_str = lexeme_str.substr(1, lexeme_str.length() - 2);
            }
        } else {
            lexeme_str = std::string(value_tokens[i].lexeme);
        }
        valueStr += lexeme_str;

        if (i + 1 < value_tokens.size()) {
            const auto& current_tok = value_tokens[i];
            const auto& next_tok = value_tokens[i + 1];
            if (current_tok.start_pos + current_tok.lexeme.length() < next_tok.start_pos) {
                valueStr += " ";
            }
        }
    }

    auto value = std::make_unique<LiteralValueNode>(std::move(valueStr));
    consume(TokenType::SEMICOLON, "Expect ';' after property value.");
    return std::make_unique<StylePropertyNode>(std::move(name), std::move(value));
}

std::unique_ptr<ASTNode> Parser::parseStyle() {
    consume(TokenType::LEFT_BRACE, "Expect '{' after 'style' keyword.");
    auto styleNode = std::make_unique<StyleNode>();
    while (!check(TokenType::RIGHT_BRACE) && !check(TokenType::END_OF_FILE)) {
        styleNode->addChild(parseStyleProperty());
    }
    consume(TokenType::RIGHT_BRACE, "Expect '}' after style block.");
    return styleNode;
}

std::unique_ptr<ASTNode> Parser::parseElement() {
    std::string tagName = std::string(previous_.lexeme);
    auto element = std::make_unique<ElementNode>(tagName);

    consume(TokenType::LEFT_BRACE, "Expect '{' after element name.");

    while (!check(TokenType::RIGHT_BRACE) && !check(TokenType::END_OF_FILE)) {
        if (check(TokenType::IDENTIFIER) && lexer_.peekToken().type == TokenType::COLON) {
            std::string attrName = std::string(current_.lexeme);
            advance();
            advance();
            if (check(TokenType::STRING)) {
                std::string attrValue = std::string(current_.lexeme);
                if (attrValue.length() >= 2) {
                    attrValue = attrValue.substr(1, attrValue.length() - 2);
                }
                element->addAttribute(attrName, attrValue);
                advance();
            } else if (check(TokenType::IDENTIFIER)) {
                 element->addAttribute(attrName, std::string(current_.lexeme));
                 advance();
            } else {
                 hadError_ = true;
                 std::cerr << "Error: Expected attribute value on line " << current_.line << std::endl;
            }
            consume(TokenType::SEMICOLON, "Expect ';' after attribute value.");
        } else {
            auto child = declaration();
            if(child) {
              element->addChild(std::move(child));
            }
        }
    }

    consume(TokenType::RIGHT_BRACE, "Expect '}' after element content.");
    return element;
}

std::unique_ptr<ASTNode> Parser::statement() {
    if (match(TokenType::KEYWORD_STYLE)) {
        return parseStyle();
    }
    if (match(TokenType::KEYWORD_TEXT)) {
        return parseText();
    }
    if (match(TokenType::IDENTIFIER)) {
        return parseElement();
    }

    if(current_.type != TokenType::END_OF_FILE && current_.type != TokenType::RIGHT_BRACE) {
       std::cerr << "Error: Unexpected token " << std::string(current_.lexeme) << " on line " << current_.line << std::endl;
       hadError_ = true;
       advance();
    }
    return nullptr;
}

std::unique_ptr<ProgramNode> Parser::parse() {
    auto program = std::make_unique<ProgramNode>();
    advance();

    while (!check(TokenType::END_OF_FILE)) {
        auto decl = declaration();
        if(decl) {
            program->addChild(std::move(decl));
        }
    }

    return hadError_ ? nullptr : std::move(program);
}
