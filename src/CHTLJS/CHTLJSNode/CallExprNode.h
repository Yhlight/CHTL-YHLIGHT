#pragma once

#include "ExprNode.h"
#include "CHTLJSLexer/Token.h"
#include <vector>

namespace CHTLJS {

class CallExprNode : public ExprNode {
public:
    CallExprNode(std::unique_ptr<ExprNode> callee, Token paren, std::vector<std::unique_ptr<ExprNode>> arguments);

    const ExprNode& getCallee() const;
    const std::vector<std::unique_ptr<ExprNode>>& getArguments() const;

private:
    std::unique_ptr<ExprNode> callee_;
    Token paren_; // For error reporting
    std::vector<std::unique_ptr<ExprNode>> arguments_;
};

}
