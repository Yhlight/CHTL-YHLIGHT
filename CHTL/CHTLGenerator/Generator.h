#pragma once

#include "CHTL/CHTLNode/BaseNode.h"
#include "CHTL/CHTLNode/TemplateStyleDefinitionNode.h"
#include "CHTL/CHTLNode/TemplateElementDefinitionNode.h"
#include "CHTL/CHTLNode/TemplateVarDefinitionNode.h"
#include "CHTL/CHTLParser/SymbolTable.h"
#include <string>
#include <memory>
#include <map>

namespace CHTL {

class Generator {
public:
    Generator(std::shared_ptr<BaseNode> root,
              std::shared_ptr<SymbolTable> symbolTable);

    std::string generate();

private:
    std::shared_ptr<BaseNode> m_root;
    std::shared_ptr<SymbolTable> m_symbolTable;
    std::string m_globalCss;

    void visit(std::shared_ptr<BaseNode> node, std::string& output);
    std::string generateStyleContent(std::shared_ptr<const StyleBlockNode> styleBlock);
    void collectStyleProperties(std::shared_ptr<const StyleBlockNode> styleBlock, std::map<std::string, std::string>& properties);
};

} // namespace CHTL
