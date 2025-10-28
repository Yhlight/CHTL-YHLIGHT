#pragma once

#include "BaseNode.h"
#include <string>
#include <map>
#include <memory>

namespace CHTL {

class TemplateVarDefinitionNode : public BaseNode {
public:
    TemplateVarDefinitionNode(const std::string& name) : m_name(name) {}

    NodeType getType() const override { return NodeType::TemplateVarDefinition; }

    const std::string& getName() const { return m_name; }
    const std::map<std::string, std::string>& getVariables() const { return m_variables; }

    void addVariable(const std::string& key, const std::string& value) {
        m_variables[key] = value;
    }

private:
    std::string m_name;
    std::map<std::string, std::string> m_variables;
};

} // namespace CHTL
