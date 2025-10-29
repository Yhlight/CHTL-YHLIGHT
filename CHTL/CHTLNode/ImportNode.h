
#pragma once

#include "BaseNode.h"
#include "OriginNode.h"
#include <string>
#include <optional>

namespace CHTL {

class ImportNode : public BaseNode {
public:
    ImportNode(OriginType type, const std::string& filePath, const std::string& name)
        : m_type(type), m_filePath(filePath), m_name(name) {}

    NodeType getType() const override { return NodeType::Import; }

    OriginType getImportType() const { return m_type; }
    const std::string& getFilePath() const { return m_filePath; }
    const std::string& getName() const { return m_name; }

private:
    OriginType m_type;
    std::string m_filePath;
    std::string m_name;
};

}
