#pragma once

#include <memory>
#include <vector>
#include <string>

namespace CHTL {

enum class NodeType {
    Program,
    Element,
    Text,
    Style,
    StyleProperty,
    StyleRule,
    Script,
    Origin,
    Template,
    TemplateUsage,
    LiteralValue,
    VariableUsage,
    Delete,
    Insert,
    ElementDelete,
    BinaryOperation,
    Comparison,
    Logical,
    Conditional,
    PropertyReference,
    If,
    Else
};

class BaseNode {
public:
    virtual ~BaseNode() = default;
    virtual NodeType getType() const = 0;
    virtual std::string ToString() const { return "BaseNode"; }
};

} // namespace CHTL
