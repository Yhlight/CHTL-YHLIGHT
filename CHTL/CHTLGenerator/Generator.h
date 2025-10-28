#pragma once

#include "CHTL/CHTLNode/BaseNode.h"
#include "CHTL/CHTLNode/TemplateStyleDefinitionNode.h"
#include <string>
#include <memory>
#include <map>

namespace CHTL {

class Generator {
public:
    Generator(std::shared_ptr<BaseNode> root, const std::map<std::string, std::shared_ptr<TemplateStyleDefinitionNode>>& styleTemplates);

    std::string generate();

private:
    std::shared_ptr<BaseNode> m_root;
    const std::map<std::string, std::shared_ptr<TemplateStyleDefinitionNode>>& m_styleTemplates;

    void visit(std::shared_ptr<BaseNode> node, std::string& output);
};

} // namespace CHTL
