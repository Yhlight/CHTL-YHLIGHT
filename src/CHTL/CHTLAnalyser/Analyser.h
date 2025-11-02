#pragma once

#include "CHTL/CHTLNode/ASTNode.h"
#include "SymbolTable.h"
#include "CHTL/CHTLImporter/Importer.h"
#include <vector>
#include <memory>
#include <string>
#include <map>

namespace CHTL {

class ProgramNode;
class TemplateNode;
class StyleNode;
class ElementNode;
class NamespaceNode;
class ConfigurationNode;

class Analyser {
public:
    Analyser(ASTNode& root, const std::string& filePath, std::vector<std::string>& importStack);
    void analyse();

    std::vector<std::unique_ptr<ASTNode>>&& getOwnedTemplates() { return std::move(m_ownedTemplates); }
    const std::map<std::string, std::string>& getConfig() const { return m_config; }

private:
    void collect(std::vector<std::unique_ptr<ASTNode>>& nodes);
    void resolve(ASTNode* node);
    void resolve(StyleNode* node);
    void resolve(ConfigurationNode* node);

    ASTNode& m_root;
    std::string m_filePath;
    SymbolTable m_symbolTable;
    std::vector<std::unique_ptr<ASTNode>> m_ownedTemplates;
    Importer m_importer;
    std::vector<std::string>& m_importStack;
    std::map<std::string, std::string> m_config;
};

} // namespace CHTL
