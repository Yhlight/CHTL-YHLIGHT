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
};

class ASTNode {
public:
    virtual ~ASTNode() = default;
    virtual ASTNodeType getType() const = 0;

    std::vector<std::unique_ptr<ASTNode>> children;
};

} // namespace CHTL
