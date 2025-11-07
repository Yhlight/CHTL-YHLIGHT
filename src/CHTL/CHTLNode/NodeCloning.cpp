#include "CHTLNode/ElementNode.h"
#include "CHTLNode/TextNode.h"
#include "CHTLNode/StyleNode.h"
#include "CHTLNode/StylePropertyNode.h"
#include "CHTLNode/StyleRuleNode.h"
#include "CHTLNode/ScriptNode.h"
#include "CHTLNode/OriginNode.h"
#include "CHTLNode/TemplateNode.h"
#include "CHTLNode/TemplateUsageNode.h"
#include "CHTLNode/LiteralValueNode.h"
#include "CHTLNode/VariableUsageNode.h"
#include "CHTLNode/DeleteNode.h"
#include "CHTLNode/ProgramNode.h"

namespace CHTL {

std::unique_ptr<BaseNode> ProgramNode::clone() const {
    auto cloned = std::make_unique<ProgramNode>();
    for (const auto& stmt : statements) {
        cloned->statements.push_back(stmt->clone());
    }
    return cloned;
}

std::unique_ptr<BaseNode> ElementNode::clone() const {
    auto cloned = std::make_unique<ElementNode>(tagName);
    cloned->attributes = attributes;
    for (const auto& child : children) {
        cloned->children.push_back(child->clone());
    }
    return cloned;
}

std::unique_ptr<BaseNode> TextNode::clone() const {
    return std::make_unique<TextNode>(content);
}

std::unique_ptr<BaseNode> StyleNode::clone() const {
    auto cloned = std::make_unique<StyleNode>();
    for (const auto& child : children) {
        cloned->children.push_back(std::unique_ptr<StyleContentNode>(static_cast<StyleContentNode*>(child->clone().release())));
    }
    return cloned;
}

std::unique_ptr<BaseNode> StylePropertyNode::clone() const {
    auto cloned = std::make_unique<StylePropertyNode>(key);
    for (const auto& v : value) {
        cloned->value.push_back(std::unique_ptr<ValueNode>(static_cast<ValueNode*>(v->clone().release())));
    }
    return cloned;
}

std::unique_ptr<BaseNode> StyleRuleNode::clone() const {
    auto cloned = std::make_unique<StyleRuleNode>(selector);
    for (const auto& prop : properties) {
        cloned->properties.push_back(std::unique_ptr<StylePropertyNode>(static_cast<StylePropertyNode*>(prop->clone().release())));
    }
    return cloned;
}

std::unique_ptr<BaseNode> ScriptNode::clone() const {
    return std::make_unique<ScriptNode>(content);
}

std::unique_ptr<BaseNode> OriginNode::clone() const {
    return std::make_unique<OriginNode>(originType, content);
}

std::unique_ptr<BaseNode> TemplateNode::clone() const {
    auto cloned = std::make_unique<TemplateNode>();
    cloned->type = type;
    cloned->name = name;
    cloned->isCustom = isCustom;
    for (const auto& item : body) {
        cloned->body.push_back(item->clone());
    }
    for (const auto& inheritance : inheritances) {
        cloned->inheritances.push_back(std::unique_ptr<TemplateUsageNode>(static_cast<TemplateUsageNode*>(inheritance->clone().release())));
    }
    return cloned;
}

std::unique_ptr<BaseNode> TemplateUsageNode::clone() const {
    auto cloned = std::make_unique<TemplateUsageNode>(type, name);
    cloned->deleted = deleted;
    for (const auto& prop : provided_properties) {
        cloned->provided_properties.push_back(std::unique_ptr<StylePropertyNode>(static_cast<StylePropertyNode*>(prop->clone().release())));
    }
    for (const auto& item : body) {
        cloned->body.push_back(item->clone());
    }
    return cloned;
}

std::unique_ptr<BaseNode> LiteralValueNode::clone() const {
    return std::make_unique<LiteralValueNode>(value);
}

std::unique_ptr<BaseNode> VariableUsageNode::clone() const {
    return std::make_unique<VariableUsageNode>(groupName, variableName);
}

std::unique_ptr<BaseNode> DeleteNode::clone() const {
    auto cloned = std::make_unique<DeleteNode>();
    cloned->properties = properties;
    cloned->inheritances = inheritances;
    return cloned;
}

} // namespace CHTL
