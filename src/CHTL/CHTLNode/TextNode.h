#pragma once

#include "ASTNode.h"
#include <string>

namespace CHTL {

class TextNode : public ASTNode {
public:
    ASTNodeType getType() const override { return ASTNodeType::Text; }

    std::string content;
};

}
