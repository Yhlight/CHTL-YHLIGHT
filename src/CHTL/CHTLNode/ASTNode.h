#pragma once

#include <memory>
#include <vector>

namespace CHTL {

enum class ASTNodeType {
    Program,
    Element,
    Text,
    Style,
    Script,
    Template,
    TemplateUsage,
    StyleProperty,
    Value,
    Import,
    Origin,
};

class ASTNode {
public:
    ASTNode(ASTNodeType type) : type_(type) {}
    virtual ~ASTNode() = default;
    ASTNodeType getType() const { return type_; }
    virtual std::unique_ptr<ASTNode> clone() const = 0;

    std::vector<std::unique_ptr<ASTNode>> children;
protected:
    ASTNodeType type_;
};

} // namespace CHTL
