#pragma once

#include "BaseNode.h"
#include <string>

namespace CHTL {

class ImportNode : public BaseNode {
public:
    std::string type;
    std::string path;
    std::string name;

    ImportNode(const std::string& type, const std::string& path, const std::string& name)
        : type(type), path(path), name(name) {}

    NodeType getType() const override { return NodeType::Import; }
};

} // namespace CHTL
