#include "SelectorExprNode.h"

namespace CHTLJS {

SelectorExprNode::SelectorExprNode(std::vector<SelectorComponent> components)
    : ExprNode(ExprNodeType::Selector), components_(std::move(components)) {}

const std::vector<SelectorComponent>& SelectorExprNode::getComponents() const {
    return components_;
}

}
