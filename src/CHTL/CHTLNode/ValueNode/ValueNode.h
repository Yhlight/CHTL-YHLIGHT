#pragma once

#include "../ASTNode.h"

namespace CHTL {

    enum class ValueNodeType {
        Literal,
        VariableUsage,
    };

    class ValueNode : public ASTNode {
    public:
        ValueNodeType valueType;

        ValueNode(ValueNodeType type, ASTNodeType nodeType = ASTNodeType::Value)
            : ASTNode(nodeType), valueType(type) {}
    };

}
