#pragma once

#include "../CHTLJSLexer/Token.h"
#include "../CHTLJSLexer/Token.h"
#include "../CHTLJSNode/ProgramNode.h"
#include "../CHTLJSNode/ExpressionNode.h"
#include <vector>
#include <memory>

class CHTLJSParser {
public:
    CHTLJSParser(const std::vector<CHTLJSToken>& tokens);
    std::unique_ptr<ProgramNode> parse();

private:
    std::unique_ptr<ExpressionNode> parseExpression();
    std::unique_ptr<ExpressionNode> parsePrimaryExpression();
    std::unique_ptr<ExpressionNode> parseMemberExpression(std::unique_ptr<ExpressionNode> object);
    std::unique_ptr<ExpressionNode> parseCallExpression(std::unique_ptr<ExpressionNode> callee);

    std::vector<CHTLJSToken> tokens;
    size_t pos = 0;
};
