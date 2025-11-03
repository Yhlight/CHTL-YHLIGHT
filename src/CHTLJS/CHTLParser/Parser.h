#pragma once

#include "../CHTLLexer/Token.h"
#include "../CHTLNode/ASTNode.h"
#include "../CHTLNode/ExprNode.h"
#include "../CHTLNode/ListenNode.h"
#include "../CHTLNode/DelegateNode.h"
#include "../CHTLNode/AnimateNode.h"
#include <vector>
#include <memory>

namespace CHTLJS {

class Parser {
public:
    Parser(const std::vector<Token>& tokens);
    std::unique_ptr<ExprNode> parse();

private:
    const std::vector<Token>& m_tokens;
    int m_current = 0;

    const Token& peek();
    const Token& previous();
    bool isAtEnd();
    const Token& advance();
    bool check(TokenType type);
    const Token& consume(TokenType type, const std::string& message);
    std::string consumeString(const std::string& message);

    std::unique_ptr<ExprNode> expression();
    std::unique_ptr<ExprNode> eventDispatch();
    std::unique_ptr<ExprNode> equality();
    std::unique_ptr<ExprNode> comparison();
    std::unique_ptr<ExprNode> term();
    std::unique_ptr<ExprNode> factor();
    std::unique_ptr<ExprNode> unary();
    std::unique_ptr<ExprNode> primary();

    std::unique_ptr<ListenNode> parseListenExpression(std::unique_ptr<ExprNode> selector);
    std::unique_ptr<DelegateNode> parseDelegateExpression(std::unique_ptr<ExprNode> selector);
    std::unique_ptr<AnimateNode> parseAnimateExpression();
    std::vector<std::unique_ptr<CssPropertyNode>> parseCssProperties();
};

} // namespace CHTLJS
