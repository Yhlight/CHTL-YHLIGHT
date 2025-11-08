#pragma once

#include "BaseNode.h"
#include <string>
#include <vector>

namespace CHTL {

class ImportNode : public BaseNode {
public:
    // e.g., "Template", "Custom", "Origin"
    std::string qualifier;
    // e.g., "Element", "Style", "Html", "Chtl"
    std::string type;
    // For precise imports, e.g., "Box"
    std::string itemName;
    // The file path
    std::string path;
    // The alias name
    std::string alias;

    ImportNode(const std::string& qualifier, const std::string& type, const std::string& itemName, const std::string& path, const std::string& alias)
        : qualifier(qualifier), type(type), itemName(itemName), path(path), alias(alias) {}

    NodeType getType() const override { return NodeType::Import; }
};

} // namespace CHTL
