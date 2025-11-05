#pragma once

#include "StyleContentNode.h"
#include <string>
#include <map>

class StyleDirectiveNode : public StyleContentNode {
public:
    StyleDirectiveNode(const std::string& name) : name(name) {}

    NodeType getType() const override { return NodeType::StyleDirective; }
    StyleContentType getStyleContentType() const override { return StyleContentType::Directive; }

    std::string name;
    std::map<std::string, std::string> properties;
};
