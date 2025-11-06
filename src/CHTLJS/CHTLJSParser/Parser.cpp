#include "Parser.h"
#include "CHTLJSNode/LiteralExprNode.h"
#include "CHTLJSNode/SelectorExprNode.h"
#include "CHTLJSNode/BinaryExprNode.h"
#include "CHTLJSNode/UnaryExprNode.h"
#include "CHTLJSNode/CallExprNode.h"
#include "CHTLJSNode/MemberAccessExprNode.h"
#include <iostream>

namespace CHTLJS {

Parser::Parser(Lexer& lexer) : lexer_(lexer) {
    rules_ = {
        {TokenType::LEFT_PAREN,     {&Parser::grouping, &Parser::call,   Precedence::CALL}},
        {TokenType::ARROW,          {nullptr,           &Parser::memberAccess,   Precedence::MEMBER}},
        {TokenType::MINUS,          {&Parser::unary,    &Parser::binary,   Precedence::TERM}},
        {TokenType::PLUS,           {nullptr,           &Parser::binary,   Precedence::TERM}},
        {TokenType::SLASH,          {nullptr,           &Parser::binary,   Precedence::FACTOR}},
        {TokenType::STAR,           {nullptr,           &Parser::binary,   Precedence::FACTOR}},
        {TokenType::NUMBER,         {&Parser::literal,  nullptr,   Precedence::NONE}},
        {TokenType::STRING,         {&Parser::literal,  nullptr,   Precedence::NONE}},
        {TokenType::LEFT_DOUBLE_BRACE, {&Parser::selector, nullptr,   Precedence::NONE}},
    };
}

void Parser::advance() {
    previous_ = current_;
    for (;;) {
        current_ = lexer_.scanToken();
        if (current_.type != TokenType::UNRECOGNIZED) break;
        std::cerr << "Error: Unexpected character on line " << current_.line << std::endl;
        hadError_ = true;
    }
}

void Parser::consume(TokenType type, const std::string& message) {
    if (check(type)) {
        advance();
        return;
    }
    std::cerr << "Error: " << message << " on line " << current_.line << std::endl;
    hadError_ = true;
}

bool Parser::match(TokenType type) {
    if (check(type)) {
        advance();
        return true;
    }
    return false;
}

bool Parser::check(TokenType type) const {
    return current_.type == type;
}

ParseRule& Parser::getRule(TokenType type) {
    return rules_[type];
}

std::unique_ptr<ExprNode> Parser::parsePrecedence(Precedence precedence) {
    advance();
    ParseFn prefixRule = getRule(previous_.type).prefix;
    if (prefixRule == nullptr) {
        std::cerr << "Error: Expect expression on line " << previous_.line << std::endl;
        return nullptr;
    }

    std::unique_ptr<ExprNode> expr = (this->*prefixRule)();

    while (precedence <= getRule(current_.type).precedence) {
        advance();
        InfixParseFn infixRule = getRule(previous_.type).infix;
        expr = (this->*infixRule)(std::move(expr));
    }

    return expr;
}

std::unique_ptr<ExprNode> Parser::literal() {
    return std::make_unique<LiteralExprNode>(std::string(previous_.lexeme));
}

std::unique_ptr<ExprNode> Parser::grouping() {
    auto expr = parsePrecedence(Precedence::ASSIGNMENT);
    consume(TokenType::RIGHT_PAREN, "Expect ')' after expression.");
    return expr;
}

std::unique_ptr<ExprNode> Parser::selector() {
    std::vector<SelectorComponent> components;
    bool expect_identifier = true;

    while (!check(TokenType::RIGHT_DOUBLE_BRACE) && !check(TokenType::END_OF_FILE)) {
        if (match(TokenType::DOT)) {
            consume(TokenType::IDENTIFIER, "Expect class name after '.'.");
            components.push_back({SelectorComponentType::CLASS, std::string(previous_.lexeme)});
            expect_identifier = false;
        } else if (match(TokenType::HASH)) {
            consume(TokenType::IDENTIFIER, "Expect id name after '#'.");
            components.push_back({SelectorComponentType::ID, std::string(previous_.lexeme)});
            expect_identifier = false;
        } else if (match(TokenType::IDENTIFIER)) {
            if (!expect_identifier) {
                components.push_back({SelectorComponentType::DESCENDANT, " "});
            }
            components.push_back({SelectorComponentType::TAG, std::string(previous_.lexeme)});
            expect_identifier = false;
        } else if (match(TokenType::LEFT_BRACKET)) {
            consume(TokenType::NUMBER, "Expect number in brackets.");
            components.push_back({SelectorComponentType::INDEX, std::string(previous_.lexeme)});
            consume(TokenType::RIGHT_BRACKET, "Expect ']' after index.");
            expect_identifier = false;
        } else if (match(TokenType::IDENTIFIER)) {
            if (components.empty() || components.back().type != SelectorComponentType::DESCENDANT) {
                 components.push_back({SelectorComponentType::DESCENDANT, " "});
            }
            components.push_back({SelectorComponentType::TAG, std::string(previous_.lexeme)});
        }
    }

    consume(TokenType::RIGHT_DOUBLE_BRACE, "Expect '}}' after selector.");
    return std::make_unique<SelectorExprNode>(std::move(components));
}

std::unique_ptr<ExprNode> Parser::binary(std::unique_ptr<ExprNode> left) {
    Token op = previous_;
    ParseRule& rule = getRule(op.type);
    auto right = parsePrecedence((Precedence)((int)rule.precedence + 1));
    return std::make_unique<BinaryExprNode>(std::move(left), op, std::move(right));
}

std::unique_ptr<ExprNode> Parser::unary() {
    Token op = previous_;
    auto right = parsePrecedence(Precedence::UNARY);
    return std::make_unique<UnaryExprNode>(op, std::move(right));
}

std::unique_ptr<ExprNode> Parser::memberAccess(std::unique_ptr<ExprNode> object) {
    consume(TokenType::IDENTIFIER, "Expect identifier after '->'.");
    return std::make_unique<MemberAccessExprNode>(std::move(object), previous_);
}

std::unique_ptr<ExprNode> Parser::call(std::unique_ptr<ExprNode> callee) {
    std::vector<std::unique_ptr<ExprNode>> arguments;
    if (!check(TokenType::RIGHT_PAREN)) {
        do {
            arguments.push_back(parsePrecedence(Precedence::ASSIGNMENT));
        } while (match(TokenType::COMMA));
    }
    consume(TokenType::RIGHT_PAREN, "Expect ')' after arguments.");
    return std::make_unique<CallExprNode>(std::move(callee), previous_, std::move(arguments));
}

std::unique_ptr<ProgramNode> Parser::parse() {
    auto program = std::make_unique<ProgramNode>();
    advance();

    while (!match(TokenType::END_OF_FILE)) {
        program->addExpression(parsePrecedence(Precedence::ASSIGNMENT));
        if (match(TokenType::SEMICOLON)) {
            // consume semicolon
        }
    }

    return hadError_ ? nullptr : std::move(program);
}

}
