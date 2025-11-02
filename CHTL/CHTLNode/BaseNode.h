#pragma once

#include <memory>
#include <vector>

class BaseNode {
public:
    virtual ~BaseNode() = default;
};

using AstNode = std::unique_ptr<BaseNode>;
using AstNodeList = std::vector<AstNode>;
