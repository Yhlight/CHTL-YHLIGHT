#pragma once

#include "BaseNode.h"
#include <string>
#include <optional>

namespace CHTL {

enum class ImportType {
    Chtl,
    Html,
    Style,
    JavaScript,
    ChtlElement,
};

enum class ImportCategory {
    Template,
    Custom,
    Origin,
};

enum class ImportedSymbolType {
    Element,
    Style,
    Var,
    Any,
};

class ImportNode : public BaseNode {
public:
    ImportNode(ImportType type, const std::string& filePath, std::optional<std::string> asName = std::nullopt,
        std::optional<ImportCategory> category = std::nullopt,
        std::optional<ImportedSymbolType> symbolType = std::nullopt,
        std::optional<std::string> symbolName = std::nullopt)
        : m_type(type), m_filePath(filePath), m_asName(asName), m_category(category), m_symbolType(symbolType), m_symbolName(symbolName) {}


    NodeType getType() const override { return NodeType::OriginUsage; }

    ImportType getImportType() const { return m_type; }
    const std::string& getFilePath() const { return m_filePath; }
    const std::optional<std::string>& getAsName() const { return m_asName; }
    const std::optional<ImportCategory>& getCategory() const { return m_category; }
    const std::optional<ImportedSymbolType>& getSymbolType() const { return m_symbolType; }
    const std::optional<std::string>& getSymbolName() const { return m_symbolName; }

private:
    ImportType m_type;
    std::string m_filePath;
    std::optional<std::string> m_asName;
    std::optional<ImportCategory> m_category;
    std::optional<ImportedSymbolType> m_symbolType;
    std::optional<std::string> m_symbolName;
};

} // namespace CHTL
