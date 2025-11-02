#pragma once

#include "ASTNode.h"
#include <string>
#include <optional>

namespace CHTL {

class OriginNode : public ASTNode {
public:
    ASTNodeType getType() const override { return ASTNodeType::Origin; }

    std::string originType;
    std::optional<std::string> name;
    std::string content;
};

}
