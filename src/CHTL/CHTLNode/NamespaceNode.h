#pragma once

#include "ASTNode.h"
#include <string>
#include <vector>
#include <memory>

namespace CHTL {

class NamespaceNode : public ASTNode {
public:
    std::string name;
    std::vector<std::unique_ptr<ASTNode>> children;

    ASTNodeType getType() const override { return ASTNodeType::Namespace; }
};

} // namespace CHTL
