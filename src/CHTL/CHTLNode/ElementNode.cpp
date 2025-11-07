#include "CHTLNode/ElementNode.h"
#include "CHTLNode/TemplateUsageNode.h"
#include "CHTLNode/StyleNode.h"
#include "CHTLNode/TextNode.h"
#include "CHTLNode/BaseNode.h"

namespace CHTL {

std::unique_ptr<ElementNode> ElementNode::cloneWithSpecializations(const TemplateUsageNode* usageNode) const {
    auto clonedNode = std::make_unique<ElementNode>(this->tagName);
    clonedNode->attributes = this->attributes;

    for (const auto& special : usageNode->body) {
        if (special->getType() == NodeType::Element) {
            auto specializedElement = static_cast<const ElementNode*>(special.get());
            if (specializedElement->tagName == this->tagName) {
                for (const auto& child : specializedElement->children) {
                    if (child->getType() == NodeType::Style) {
                        clonedNode->children.push_back(child->clone());
                    }
                }
            }
        }
    }

    for (const auto& child : this->children) {
        if (child->getType() == NodeType::Element) {
            auto elementChild = static_cast<const ElementNode*>(child.get());
            clonedNode->children.push_back(elementChild->cloneWithSpecializations(usageNode));
        } else {
            clonedNode->children.push_back(child->clone());
        }
    }

    return clonedNode;
}

} // namespace CHTL
