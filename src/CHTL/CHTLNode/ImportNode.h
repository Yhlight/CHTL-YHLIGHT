#pragma once

#include "BaseNode.h"
#include <string>

class ImportNode : public BaseNode {
public:
    ImportNode(const std::string& type, const std::string& path, const std::string& alias)
        : type(type), path(path), alias(alias) {}

    NodeType getType() const override { return NodeType::Import; }

    std::unique_ptr<BaseNode> clone() const override {
        return std::make_unique<ImportNode>(type, path, alias);
    }

    std::string type;
    std::string path;
    std::string alias;
};
