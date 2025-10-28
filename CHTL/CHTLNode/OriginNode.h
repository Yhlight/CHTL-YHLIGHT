
#pragma once

#include "BaseNode.h"
#include <string>
#include <optional>

namespace CHTL {

enum class OriginType {
    Html,
    Style,
    JavaScript
};

class OriginNode : public BaseNode {
public:
    OriginNode(OriginType type, const std::string& content, std::optional<std::string> name = std::nullopt)
        : m_type(type), m_content(content), m_name(name) {}

    NodeType getType() const override { return NodeType::Origin; }

    OriginType getOriginType() const { return m_type; }
    const std::string& getContent() const { return m_content; }
    const std::optional<std::string>& getName() const { return m_name; }
    void setName(const std::string& name) { m_name = name; }

private:
    OriginType m_type;
    std::string m_content;
    std::optional<std::string> m_name;
};

} // namespace CHTL
