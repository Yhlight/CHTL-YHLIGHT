#pragma once

#include "ASTNode.h"
#include <string>
#include <vector>
#include <optional>

namespace CHTL {

struct ImportTarget {
    bool isCustom = false;
    std::string type; // e.g., "@Element", "@Style"
    std::optional<std::string> name;
};

class ImportNode : public ASTNode {
public:
    ASTNodeType getType() const override { return ASTNodeType::Import; }

    std::vector<ImportTarget> targets;
    std::string path;
    std::optional<std::string> alias;
};

}
