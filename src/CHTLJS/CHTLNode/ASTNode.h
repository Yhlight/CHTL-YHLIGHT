#pragma once

#include <memory>
#include <vector>

namespace CHTLJS {

enum class ASTNodeType {
    Program,
    BinaryExpr,
    Literal,
    SelectorExpr,
    Listen,
    EventHandler,
    Delegate,
    Animate,
    Keyframe,
    CssProperty,
    Vir,
    Identifier,
    VirUsage,
};

class ASTNode {
public:
    virtual ~ASTNode() = default;
    virtual ASTNodeType getType() const = 0;
    virtual std::unique_ptr<ASTNode> clone() const = 0;
};

} // namespace CHTLJS
