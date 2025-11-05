#pragma once

#include "ASTNode.h"
#include <string>
#include <vector>

namespace CHTL {

enum class ImportType {
    Html,
    Style,
    JavaScript,
    Chtl,
    Config,
};

enum class ChtlImportType {
    CustomElement,
    CustomStyle,
    CustomVar,
    TemplateElement,
    TemplateStyle,
    TemplateVar,
    OriginHtml,
    OriginStyle,
    OriginJavaScript,
    AllCustom,
    AllTemplate,
    AllOrigin,
    File,
};

struct ImportTarget {
    ChtlImportType type;
    std::string name;
    std::string alias;
};

class ImportNode : public ASTNode {
public:
    ImportNode(ImportType type, const std::string& path)
        : ASTNode(ASTNodeType::Import), importType(type), path(path) {}

    std::unique_ptr<ASTNode> clone() const override {
        auto node = std::make_unique<ImportNode>(importType, path);
        node->targets = targets;
        return node;
    }

    ImportType importType;
    std::string path;
    std::vector<ImportTarget> targets;
};

} // namespace CHTL
