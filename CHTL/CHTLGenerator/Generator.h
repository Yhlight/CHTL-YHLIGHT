#pragma once

#include "CHTL/CHTLNode/BaseNode.h"
#include "CHTL/CHTLNode/TemplateStyleDefinitionNode.h"
#include "CHTL/CHTLNode/TemplateElementDefinitionNode.h"
#include "CHTL/CHTLNode/TemplateVarDefinitionNode.h"
#include <string>
#include <memory>
#include <map>

namespace CHTL {

class Generator {
public:
    Generator(std::shared_ptr<BaseNode> root,
              const std::map<std::string, std::shared_ptr<TemplateStyleDefinitionNode>>& styleTemplates,
              const std::map<std::string, std::shared_ptr<TemplateElementDefinitionNode>>& elementTemplates,
              const std::map<std::string, std::shared_ptr<TemplateVarDefinitionNode>>& varTemplates);

    std::string generate();

private:
    std::shared_ptr<BaseNode> m_root;
    const std::map<std::string, std::shared_ptr<TemplateStyleDefinitionNode>>& m_styleTemplates;
    const std::map<std::string, std::shared_ptr<TemplateElementDefinitionNode>>& m_elementTemplates;
    const std::map<std::string, std::shared_ptr<TemplateVarDefinitionNode>>& m_varTemplates;

    void visit(std::shared_ptr<BaseNode> node, std::string& output);
    std::string generateStyleContent(std::shared_ptr<const StyleBlockNode> styleBlock);
    void collectStyleProperties(std::shared_ptr<const StyleBlockNode> styleBlock, std::map<std::string, std::string>& properties);
};

} // namespace CHTL
