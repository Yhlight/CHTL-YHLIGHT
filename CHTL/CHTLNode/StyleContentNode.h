#pragma once

#include "BaseNode.h"
#include <string>
#include <vector>
#include <memory>
#include <map>

enum class StyleContentType {
    Raw,
    Directive,
};

class StyleContentNode : public BaseNode {
public:
    StyleContentType type;
};

class RawStyleContentNode : public StyleContentNode {
public:
    RawStyleContentNode() { type = StyleContentType::Raw; }
    std::string raw_css;
};

class StyleDirectiveNode : public StyleContentNode {
public:
    StyleDirectiveNode() { type = StyleContentType::Directive; }
    std::string directive_name; // e.g. @Style
    std::string template_name;
    std::map<std::string, std::string> properties;
};
