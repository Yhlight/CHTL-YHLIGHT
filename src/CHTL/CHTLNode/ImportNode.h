#pragma once

#include "BaseNode.h"
#include <string>
#include <vector>

namespace CHTL {

class ImportNode : public BaseNode {
public:
    std::string qualifier; // e.g., "Template", "Custom", "Origin"
    std::string type;      // e.g., "Style", "Element", "Var", "Html", "Chtl"
    std::string itemName;  // For selective imports, e.g., "MyTemplate"
    std::string path;
    std::string alias;

    ImportNode(const std::string& qualifier, const std::string& type, const std::string& itemName, const std::string& path, const std::string& alias)
        : qualifier(qualifier), type(type), itemName(itemName), path(path), alias(alias) {}

    NodeType getType() const override { return NodeType::Import; }
    std::string ToString() const override {
        return "ImportNode: [qualifier=" + qualifier + ", type=" + type + ", itemName=" + itemName + ", path=" + path + ", alias=" + alias + "]";
    }
};

} // namespace CHTL
