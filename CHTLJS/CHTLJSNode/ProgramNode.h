#pragma once

#include "BaseNode.h"
#include <vector>
#include <memory>

class ProgramNode : public CHTLJSBaseNode {
public:
    ProgramNode() = default;
    std::vector<std::unique_ptr<CHTLJSBaseNode>> children;
};
