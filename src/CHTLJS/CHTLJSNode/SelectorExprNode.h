#pragma once

#include "ExprNode.h"
#include <string>

namespace CHTLJS {

class SelectorExprNode : public ExprNode {
public:
    explicit SelectorExprNode(std::string selector) : ExprNode(ExprNodeType::Selector), selector_(std::move(selector)) {}

    const std::string& getSelector() const { return selector_; }

private:
    std::string selector_;
};

}
