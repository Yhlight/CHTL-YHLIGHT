#pragma once

#include "ASTNode.h"
#include <string>

class ImportNode : public ASTNode {
public:
    explicit ImportNode(std::string path)
        : ASTNode(ASTNodeType::Import), path_(std::move(path)) {}

    const std::string& getPath() const { return path_; }

    std::unique_ptr<ASTNode> clone() const override {
        return std::make_unique<ImportNode>(path_);
    }

private:
    std::string path_;
};
