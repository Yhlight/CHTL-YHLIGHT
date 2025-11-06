#pragma once

#include "ExprNode.h"
#include <string>

namespace CHTLJS {

class LiteralExprNode : public ExprNode {
public:
    explicit LiteralExprNode(std::string value) : ExprNode(ExprNodeType::Literal), value_(std::move(value)) {}

    const std::string& getValue() const { return value_; }

private:
    std::string value_;
};

}
