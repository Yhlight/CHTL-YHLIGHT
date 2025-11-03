#pragma once

#include "ExprNode.h"
#include "SelectorExprNode.h"
#include <string>
#include <vector>
#include <memory>
#include <optional>

namespace CHTLJS {

struct Route {
    std::string url;
    std::unique_ptr<SelectorExprNode> page;
};

class RouterNode : public ExprNode {
public:
    std::vector<Route> routes;
    std::optional<std::string> root_path;
    std::optional<std::unique_ptr<SelectorExprNode>> root_container;
    std::optional<std::string> mode;

    ASTNodeType getType() const override { return ASTNodeType::Router; }

    std::unique_ptr<ExprNode> clone_expr() const override {
        auto node = std::make_unique<RouterNode>();
        for (const auto& route : routes) {
            node->routes.push_back({route.url, std::unique_ptr<SelectorExprNode>(static_cast<SelectorExprNode*>(route.page->clone().release()))});
        }
        node->root_path = root_path;
        if (root_container.has_value()) {
            node->root_container = std::unique_ptr<SelectorExprNode>(static_cast<SelectorExprNode*>(root_container.value()->clone().release()));
        }
        node->mode = mode;
        return node;
    }
};

} // namespace CHTLJS
