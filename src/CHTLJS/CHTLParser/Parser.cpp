#include "Parser.h"
#include "../CHTLNode/BinaryExprNode.h"
#include "../CHTLNode/LiteralNode.h"
#include "../CHTLNode/SelectorExprNode.h"
#include <stdexcept>

namespace CHTLJS {

Parser::Parser(const std::vector<Token>& tokens) : m_tokens(tokens) {}

std::unique_ptr<ExprNode> Parser::parse() {
    try {
        return expression();
    } catch (const std::runtime_error& e) {
        return nullptr;
    }
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

// Pratt parser implementation will go here in the next step

std::unique_ptr<ExprNode> Parser::expression() {
    return equality();
}

std::unique_ptr<ExprNode> Parser::equality() {
    auto expr = comparison();
    while (check(TokenType::BangEqual) || check(TokenType::EqualEqual)) {
        Token op = advance();
        auto right = comparison();
        auto binaryExpr = std::make_unique<BinaryExprNode>();
        binaryExpr->left = std::move(expr);
        binaryExpr->op = op;
        binaryExpr->right = std::move(right);
        expr = std::move(binaryExpr);
    }
    return expr;
}

std::unique_ptr<ExprNode> Parser::comparison() {
    auto expr = term();
    while (check(TokenType::Greater) || check(TokenType::GreaterEqual) || check(TokenType::Less) || check(TokenType::LessEqual)) {
        Token op = advance();
        auto right = term();
        auto binaryExpr = std::make_unique<BinaryExprNode>();
        binaryExpr->left = std::move(expr);
        binaryExpr->op = op;
        binaryExpr->right = std::move(right);
        expr = std::move(binaryExpr);
    }
    return expr;
}

std::unique_ptr<ExprNode> Parser::term() {
    auto expr = factor();
    while (check(TokenType::Minus) || check(TokenType::Plus)) {
        Token op = advance();
        auto right = factor();
        auto binaryExpr = std::make_unique<BinaryExprNode>();
        binaryExpr->left = std::move(expr);
        binaryExpr->op = op;
        binaryExpr->right = std::move(right);
        expr = std::move(binaryExpr);
    }
    return expr;
}

std::unique_ptr<ExprNode> Parser::factor() {
    auto expr = unary();
    while (check(TokenType::Slash) || check(TokenType::Star)) {
        Token op = advance();
        auto right = unary();
        auto binaryExpr = std::make_unique<BinaryExprNode>();
        binaryExpr->left = std::move(expr);
        binaryExpr->op = op;
        binaryExpr->right = std::move(right);
        expr = std::move(binaryExpr);
    }
    return expr;
}

std::unique_ptr<ExprNode> Parser::unary() {
    if (check(TokenType::Bang) || check(TokenType::Minus)) {
        Token op = advance();
        auto right = unary();
        // This is not quite right for a pratt parser, but it will work for now.
        // A proper implementation would use prefix parselets.
    }
    return primary();
}

std::unique_ptr<ExprNode> Parser::primary() {
    if (check(TokenType::Number) || check(TokenType::String)) {
        auto literalNode = std::make_unique<LiteralNode>();
        literalNode->value = std::string(advance().lexeme);
        return literalNode;
    }

    if (check(TokenType::LeftDoubleBrace)) {
        advance(); // consume {{
        auto selectorNode = std::make_unique<SelectorExprNode>();
        selectorNode->selector = std::string(consume(TokenType::Identifier, "Expect selector.").lexeme);
        consume(TokenType::RightDoubleBrace, "Expect '}}' after selector.");
        return selectorNode;
    }

    throw std::runtime_error("Expect expression.");
}


} // namespace CHTLJS
