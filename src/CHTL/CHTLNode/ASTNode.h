#pragma once

#include <memory>
#include <vector>

enum class ASTNodeType {
    Program,
    Text,
    Element
};

class ASTNode {
public:
    explicit ASTNode(ASTNodeType type) : type_(type) {}
    virtual ~ASTNode() = default;

    ASTNodeType getType() const { return type_; }

    void addChild(std::unique_ptr<ASTNode> child) {
        children_.push_back(std::move(child));
    }

    const std::vector<std::unique_ptr<ASTNode>>& getChildren() const {
        return children_;
    }

protected:
    ASTNodeType type_;
    std::vector<std::unique_ptr<ASTNode>> children_;
};
