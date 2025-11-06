#pragma once

#include "ExprNode.h"

namespace CHTLJS {

class ProgramNode : public ExprNode {
public:
    ProgramNode() : ExprNode(ExprNodeType::Program) {}

    void addExpression(std::unique_ptr<ExprNode> expr) {
        expressions_.push_back(std::move(expr));
    }

    const std::vector<std::unique_ptr<ExprNode>>& getExpressions() const {
        return expressions_;
    }

private:
    std::vector<std::unique_ptr<ExprNode>> expressions_;
};

}
