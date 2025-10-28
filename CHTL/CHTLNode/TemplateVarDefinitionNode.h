#pragma once

#include "BaseNode.h"
#include <string>
#include <map>
#include <memory>
#include <optional>

namespace CHTL {

class TemplateVarDefinitionNode : public BaseNode {
public:
    TemplateVarDefinitionNode(const std::string& name) : m_name(name) {}

    NodeType getType() const override { return NodeType::TemplateVarDefinition; }

    const std::string& getName() const { return m_name; }
    const std::map<std::string, std::string>& getVariables() const { return m_variables; }
    const std::optional<std::string>& getParentName() const { return m_parentName; }

    void addVariable(const std::string& key, const std::string& value) {
        m_variables[key] = value;
    }
    void setParentName(const std::string& parentName) { m_parentName = parentName; }

private:
    std::string m_name;
    std::map<std::string, std::string> m_variables;
    std::optional<std::string> m_parentName;
};

} // namespace CHTL
