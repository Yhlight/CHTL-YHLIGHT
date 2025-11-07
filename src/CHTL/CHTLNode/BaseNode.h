#pragma once

#include <memory>
#include <vector>

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
    TemplateUsage
};

class BaseNode {
public:
    virtual ~BaseNode() = default;
    virtual NodeType getType() const = 0;
};

} // namespace CHTL
