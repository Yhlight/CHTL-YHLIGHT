#pragma once

#include "ASTNode.h"
#include "TemplateNode.h" // For TemplateType enum
#include <string>
#include <vector>
#include <memory>

namespace CHTL {

class CustomTemplateNode : public ASTNode {
public:
    ASTNodeType getType() const override { return ASTNodeType::CustomTemplate; }

    TemplateType templateType;
    std::string name;
    std::vector<std::unique_ptr<ASTNode>> children;
};

}
