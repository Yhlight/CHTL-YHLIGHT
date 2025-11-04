#include "Parser.h"
#include "../CHTLNode/BinaryExprNode.h"
#include "../CHTLNode/LiteralNode.h"
#include "../CHTLNode/SelectorExprNode.h"
#include "../CHTLNode/ListenNode.h"
#include "../CHTLNode/DelegateNode.h"
#include "../CHTLNode/AnimateNode.h"
#include "../CHTLNode/IdentifierNode.h"
#include "../CHTLNode/ProgramNode.h"
#include "../CHTLNode/VirUsageNode.h"
#include "../CHTLNode/RouterNode.h"
#include "../CHTLNode/MemberExprNode.h"
#include <stdexcept>
#include <vector>
#include <string>

namespace CHTLJS {

Parser::Parser(const std::vector<Token>& tokens) : m_tokens(tokens) {}

std::unique_ptr<ProgramNode> Parser::parse() {
    std::vector<std::unique_ptr<ExprNode>> statements;
    while (!isAtEnd()) {
        statements.push_back(expression());
        if (check(TokenType::Semicolon)) {
            advance();
        }
    }
    return std::make_unique<ProgramNode>(std::move(statements));
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
std::string Parser::consumeString(const std::string& message) {
    const Token& token = consume(TokenType::String, message);
    return std::string(token.lexeme.substr(1, token.lexeme.length() - 2));
}

std::unique_ptr<ExprNode> Parser::expression() {
    if (check(TokenType::Animate)) {
        advance();
        return parseAnimateExpression();
    }
    if (check(TokenType::Vir)) {
        advance();
        return parseVirExpression();
    }
    if (check(TokenType::Router)) {
        advance();
        return parseRouterExpression();
    }
    return eventDispatch();
}

std::unique_ptr<VirNode> Parser::parseVirExpression() {
    std::string name = std::string(consume(TokenType::Identifier, "Expect virtual object name.").lexeme);
    consume(TokenType::Equal, "Expect '=' after virtual object name.");
    auto expression = this->expression();
    return std::make_unique<VirNode>(name, std::move(expression));
}

std::unique_ptr<RouterNode> Parser::parseRouterExpression() {
    consume(TokenType::LeftBrace, "Expect '{' after Router.");
    auto routerNode = std::make_unique<RouterNode>();

    while (peek().type != TokenType::RightBrace && !isAtEnd()) {
        std::string key = std::string(consume(TokenType::Identifier, "Expect property name.").lexeme);
        consume(TokenType::Colon, "Expect ':' after property name.");

        if (key == "url") {
            routerNode->routes.push_back({consumeString("Expect URL."), nullptr});
        } else if (key == "page") {
            if (routerNode->routes.empty()) {
                throw std::runtime_error("Cannot assign page before URL.");
            }
            routerNode->routes.back().page = std::unique_ptr<SelectorExprNode>(static_cast<SelectorExprNode*>(primary().release()));
        } else if (key == "root") {
            if (check(TokenType::String)) {
                routerNode->root_path = consumeString("Expect root path.");
            } else {
                routerNode->root_container = std::unique_ptr<SelectorExprNode>(static_cast<SelectorExprNode*>(primary().release()));
            }
        } else if (key == "mode") {
            routerNode->mode = consumeString("Expect mode.");
        }

        if (check(TokenType::Comma)) advance();
    }

    consume(TokenType::RightBrace, "Expect '}' after Router block.");
    return routerNode;
}

std::unique_ptr<ExprNode> Parser::eventDispatch() {
    auto expr = equality();

    if (check(TokenType::Identifier)) {
        auto identifier = std::make_unique<IdentifierNode>(std::string(advance().lexeme));
        return std::make_unique<VirUsageNode>(std::move(expr), std::move(identifier));
    }

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
        std::string handlerBody = consumeString("Expect event handler body.");
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
        std::string handlerBody = consumeString("Expect event handler body.");
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

std::vector<std::unique_ptr<CssPropertyNode>> Parser::parseCssProperties() {
    consume(TokenType::LeftBrace, "Expect '{' after property block.");
    std::vector<std::unique_ptr<CssPropertyNode>> properties;
    while (!check(TokenType::RightBrace)) {
        std::string name = std::string(consume(TokenType::Identifier, "Expect property name.").lexeme);
        consume(TokenType::Colon, "Expect ':' after property name.");
        std::string value = consumeString("Expect property value.");
        properties.emplace_back(std::make_unique<CssPropertyNode>(name, value));
        if (check(TokenType::Comma)) {
            advance();
        }
    }
    consume(TokenType::RightBrace, "Expect '}' after property block.");
    return properties;
}


std::unique_ptr<AnimateNode> Parser::parseAnimateExpression() {
    consume(TokenType::LeftBrace, "Expect '{' after Animate.");
    std::unique_ptr<SelectorExprNode> target;
    int duration = 0;
    std::string easing;
     auto animateNode = std::make_unique<AnimateNode>(nullptr, 0, "");

    while(peek().type != TokenType::RightBrace && !isAtEnd()) {
        std::string key = std::string(consume(TokenType::Identifier, "Expect property name.").lexeme);
        consume(TokenType::Colon, "Expect ':' after property name.");

        if (key == "target") {
            animateNode->target = std::unique_ptr<SelectorExprNode>(dynamic_cast<SelectorExprNode*>(primary().release()));
        } else if (key == "duration") {
            animateNode->duration = std::stoi(std::string(consume(TokenType::Number, "Expect duration.").lexeme));
        } else if (key == "easing") {
            animateNode->easing = consumeString("Expect easing value.");
        } else if (key == "begin") {
            animateNode->begin = parseCssProperties();
        } else if (key == "end") {
            animateNode->end = parseCssProperties();
        } else if (key == "loop") {
            animateNode->loop = std::stoi(std::string(consume(TokenType::Number, "Expect loop count.").lexeme));
        } else if (key == "direction") {
            animateNode->direction = consumeString("Expect direction.");
        } else if (key == "delay") {
            animateNode->delay = std::stoi(std::string(consume(TokenType::Number, "Expect delay.").lexeme));
        } else if (key == "callback") {
            animateNode->callback = std::string(consume(TokenType::Identifier, "Expect callback name.").lexeme);
        }
        else if (key == "when") {
            consume(TokenType::LeftBracket, "Expect '[' after when.");
            while(!check(TokenType::RightBracket)) {
                consume(TokenType::LeftBrace, "Expect '{' after at.");
                consume(TokenType::Identifier, "Expect at.");
                 consume(TokenType::Colon, "Expect ':' after at.");
                double at = std::stod(std::string(consume(TokenType::Number, "Expect at value.").lexeme));
                consume(TokenType::Comma, "Expect ',' after at.");
                std::vector<std::unique_ptr<CssPropertyNode>> properties;
                while(!check(TokenType::RightBrace)) {
                    std::string name = std::string(consume(TokenType::Identifier, "Expect property name.").lexeme);
                    consume(TokenType::Colon, "Expect ':' after property name.");
                    std::string value = consumeString("Expect property value.");
                    properties.emplace_back(std::make_unique<CssPropertyNode>(name, value));
                    if(check(TokenType::Comma)) advance();
                }
                consume(TokenType::RightBrace, "Expect '}' after keyframe properties.");
                animateNode->when.emplace_back(std::make_unique<KeyframeNode>(at, std::move(properties)));
                 if(check(TokenType::Comma)) advance();
            }
            consume(TokenType::RightBracket, "Expect ']' after when.");
        }
        if (check(TokenType::Comma)) advance();
    }

    consume(TokenType::RightBrace, "Expect '}' after Animate block.");
    return animateNode;
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
    return call();
}

std::unique_ptr<ExprNode> Parser::call() {
    auto expr = primary();

    while (true) {
        if (check(TokenType::ArrowDash)) {
            advance();

            if (expr->getType() == ASTNodeType::SelectorExpr) {
                auto* selector = static_cast<SelectorExprNode*>(expr.get());
                if (selector->type != SelectorType::Id && !selector->index.has_value()) {
                    selector->index = 0;
                }
            }

            auto property = primary();
            auto memberExpr = std::make_unique<MemberExprNode>();
            memberExpr->object = std::move(expr);
            memberExpr->property = std::move(property);
            expr = std::move(memberExpr);
        } else {
            break;
        }
    }

    return expr;
}

std::unique_ptr<ExprNode> Parser::primary() {
    if (check(TokenType::Number) || check(TokenType::String)) {
        auto literalNode = std::make_unique<LiteralNode>();
        literalNode->value = std::string(advance().lexeme);
        return literalNode;
    }

    if (check(TokenType::Identifier)) {
        return std::make_unique<IdentifierNode>(std::string(advance().lexeme));
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
