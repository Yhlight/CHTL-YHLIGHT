#include "Parser.h"
#include "../CHTLNode/BinaryExprNode.h"
#include "../CHTLNode/LiteralNode.h"
#include "../CHTLNode/SelectorExprNode.h"
#include "../CHTLNode/ListenNode.h"
#include "../CHTLNode/DelegateNode.h"
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

std::unique_ptr<ExprNode> Parser::expression() {
    return eventDispatch();
}

std::unique_ptr<ExprNode> Parser::eventDispatch() {
    auto expr = equality();

    if (check(TokenType::Listen)) {
        advance();
        return parseListenExpression(std::move(expr));
    }

    if (check(TokenType::Delegate)) {
        advance();
        return parseDelegateExpression(std::move(expr));
    }

    return expr;
}

std::unique_ptr<ListenNode> Parser::parseListenExpression(std::unique_ptr<ExprNode> selector) {
    consume(TokenType::LeftBrace, "Expect '{' after Listen.");
    std::vector<std::unique_ptr<EventHandlerNode>> handlers;

    while (!check(TokenType::RightBrace)) {
        std::string eventName = std::string(consume(TokenType::Identifier, "Expect event name.").lexeme);
        consume(TokenType::Colon, "Expect ':' after event name.");
        std::string handlerBody = std::string(consume(TokenType::String, "Expect event handler body.").lexeme);
        handlers.emplace_back(std::make_unique<EventHandlerNode>(eventName, handlerBody));

        if (check(TokenType::Comma)) {
            advance();
        } else {
            break;
        }
    }

    consume(TokenType::RightBrace, "Expect '}' after Listen block.");
    auto selectorNode = dynamic_cast<SelectorExprNode*>(selector.release());
    if (!selectorNode) {
        throw std::runtime_error("Expected a selector expression for Listen.");
    }
    return std::make_unique<ListenNode>(
        std::unique_ptr<SelectorExprNode>(selectorNode),
        std::move(handlers)
    );
}

std::unique_ptr<DelegateNode> Parser::parseDelegateExpression(std::unique_ptr<ExprNode> selector) {
    consume(TokenType::LeftBrace, "Expect '{' after Delegate.");

    if (consume(TokenType::Identifier, "Expect 'target' keyword.").lexeme != "target") {
        throw std::runtime_error("Expect 'target' keyword.");
    }
    consume(TokenType::Colon, "Expect ':' after 'target'.");
    auto targetSelector = primary();

    consume(TokenType::Comma, "Expect ',' after target selector.");

    std::vector<std::unique_ptr<EventHandlerNode>> handlers;
    while (!check(TokenType::RightBrace)) {
        std::string eventName = std::string(consume(TokenType::Identifier, "Expect event name.").lexeme);
        consume(TokenType::Colon, "Expect ':' after event name.");
        std::string handlerBody = std::string(consume(TokenType::String, "Expect event handler body.").lexeme);
        handlers.emplace_back(std::make_unique<EventHandlerNode>(eventName, handlerBody));

        if (check(TokenType::Comma)) {
            advance();
        } else {
            break;
        }
    }

    consume(TokenType::RightBrace, "Expect '}' after Delegate block.");
    auto parentSelectorNode = dynamic_cast<SelectorExprNode*>(selector.release());
    if (!parentSelectorNode) {
        throw std::runtime_error("Expected a selector expression for Delegate parent.");
    }
    auto targetSelectorNode = dynamic_cast<SelectorExprNode*>(targetSelector.release());
    if (!targetSelectorNode) {
        throw std::runtime_error("Expected a selector expression for Delegate target.");
    }
    return std::make_unique<DelegateNode>(
        std::unique_ptr<SelectorExprNode>(parentSelectorNode),
        std::unique_ptr<SelectorExprNode>(targetSelectorNode),
        std::move(handlers)
    );
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
        advance();
        auto selectorNode = std::make_unique<SelectorExprNode>();
        if (check(TokenType::Dot)) {
            advance();
            selectorNode->type = SelectorType::Class;
            selectorNode->baseName = std::string(consume(TokenType::Identifier, "Expect class name.").lexeme);
        } else if (check(TokenType::Identifier) && peek().lexeme[0] == '#') {
            selectorNode->type = SelectorType::Id;
            selectorNode->baseName = std::string(advance().lexeme).substr(1);
        } else {
            selectorNode->type = SelectorType::Tag;
            selectorNode->baseName = std::string(consume(TokenType::Identifier, "Expect tag name.").lexeme);
        }

        if (check(TokenType::Identifier)) {
            selectorNode->descendant = std::string(advance().lexeme);
            selectorNode->type = SelectorType::Compound;
        }

        if (check(TokenType::LeftBracket)) {
            advance();
            selectorNode->index = std::stoi(std::string(consume(TokenType::Number, "Expect index.").lexeme));
            consume(TokenType::RightBracket, "Expect ']' after index.");
        }

        consume(TokenType::RightDoubleBrace, "Expect '}}' after selector.");
        return selectorNode;
    }

    throw std::runtime_error("Expect expression.");
}


}
