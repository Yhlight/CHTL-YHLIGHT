#pragma once

#include <memory>
#include <vector>

namespace CHTL {

enum class ASTNodeType {
    Program,
    Text,
    Element,
    StyleBlock,
    StyleProperty,
    Template,
    CustomTemplate,
    Origin,
    Import,
};

class ASTNode {
public:
    virtual ~ASTNode() = default;
    virtual ASTNodeType getType() const = 0;
};

}
