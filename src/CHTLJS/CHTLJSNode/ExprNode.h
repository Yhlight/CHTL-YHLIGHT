#pragma once

#include <memory>
#include <vector>

namespace CHTLJS {

enum class ExprNodeType {
    Program,
    Literal,
    Selector,
    Binary,
    Unary
};

class ExprNode {
public:
    explicit ExprNode(ExprNodeType type) : type_(type) {}
    virtual ~ExprNode() = default;

    ExprNodeType getType() const { return type_; }

protected:
    ExprNodeType type_;
};

}
