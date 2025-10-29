#pragma once

#include "BaseNode.h"
#include <string>
#include <optional>

namespace CHTL {

enum class ImportType {
    Chtl,
    Html,
    Style,
    JavaScript
};

class ImportNode : public BaseNode {
public:
    ImportNode(ImportType type, const std::string& filePath, std::optional<std::string> asName = std::nullopt)
        : m_type(type), m_filePath(filePath), m_asName(asName) {}

    NodeType getType() const override { return NodeType::OriginUsage; }

    ImportType getImportType() const { return m_type; }
    const std::string& getFilePath() const { return m_filePath; }
    const std::optional<std::string>& getAsName() const { return m_asName; }

private:
    ImportType m_type;
    std::string m_filePath;
    std::optional<std::string> m_asName;
};

} // namespace CHTL
