#pragma once

#include "CHTL/CHTLNode/ASTNode.h"
#include "SharedCore/CHTLContext.h"
#include "CHTL/CHTLImporter/Importer.h"
#include <vector>
#include <memory>
#include <string>

namespace CHTL {

class ProgramNode;
class TemplateNode;
class StyleNode;
class ElementNode;
class NamespaceNode;
class ConfigurationNode;

class Analyser {
public:
    Analyser(ASTNode& root, const std::string& filePath, std::vector<std::string>& importStack, std::shared_ptr<CHTLContext> context);
    void analyse();

    std::vector<std::unique_ptr<ASTNode>>&& getOwnedTemplates() { return std::move(m_ownedTemplates); }

private:
    void collect(std::vector<std::unique_ptr<ASTNode>>& nodes);
    void resolve(ASTNode* node);
    void resolve(StyleNode* node);
    void resolve(ConfigurationNode* node);

    ASTNode& m_root;
    std::string m_filePath;
    std::shared_ptr<CHTLContext> m_context;
    std::vector<std::unique_ptr<ASTNode>> m_ownedTemplates;
    Importer m_importer;
    std::vector<std::string>& m_importStack;
};

} // namespace CHTL
