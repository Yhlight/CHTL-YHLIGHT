#pragma once

#include "BaseNode.h"
#include <string>
#include <optional>

namespace CHTL {

class OriginNode : public BaseNode {
public:
    OriginNode(const std::string& type, const std::string& content)
        : m_type(type), m_content(content) {}

    OriginNode(const std::string& type, const std::string& name, const std::string& content)
        : m_type(type), m_name(name), m_content(content) {}

    NodeType getType() const override { return NodeType::Origin; }

    const std::string& getOriginType() const { return m_type; }
    const std::optional<std::string>& getName() const { return m_name; }
    const std::string& getContent() const { return m_content; }

private:
    std::string m_type;
    std::optional<std::string> m_name;
    std::string m_content;
};

} // namespace CHTL
