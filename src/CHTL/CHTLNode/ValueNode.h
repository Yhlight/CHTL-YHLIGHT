#pragma once

#include "BaseNode.h"

namespace CHTL {

class ValueNode : public BaseNode {
public:
    virtual std::unique_ptr<ValueNode> clone() const = 0;
};

} // namespace CHTL
