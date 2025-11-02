#pragma once

#include "ASTNode.h"
#include <string>
#include <vector>
#include <memory>
#include <optional>
#include <map>

namespace CHTL {

class ConfigurationNode : public ASTNode {
public:
    ASTNodeType getType() const override { return ASTNodeType::Configuration; }

    std::optional<std::string> name;
    std::map<std::string, std::string> settings;
    std::map<std::string, std::string> nameGroup;
};

}
