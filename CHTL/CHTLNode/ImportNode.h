#pragma once

#include "BaseNode.h"
#include <string>
#include <optional>

namespace CHTL {

enum class ImportType {
    Html,
    Style,
    JavaScript,
    Chtl
};

class ImportNode : public BaseNode {
public:
    ImportNode(ImportType type, const std::string& path, std::optional<std::string> name)
        : m_type(type), m_path(path), m_name(name) {}

    NodeType getType() const override { return NodeType::Import; }
    ImportType getImportType() const { return m_type; }
    const std::string& getPath() const { return m_path; }
    const std::optional<std::string>& getName() const { return m_name; }

private:
    ImportType m_type;
    std::string m_path;
    std::optional<std::string> m_name;
};

} // namespace CHTL
