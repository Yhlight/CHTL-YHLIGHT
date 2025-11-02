#pragma once

#include "ASTNode.h"
#include <string>
#include <vector>
#include <memory>
#include <map>

namespace CHTL {

class ElementNode : public ASTNode {
public:
    ASTNodeType getType() const override { return ASTNodeType::Element; }

    std::string tagName;
    std::map<std::string, std::string> attributes;
    std::vector<std::unique_ptr<ASTNode>> children;
};

}
