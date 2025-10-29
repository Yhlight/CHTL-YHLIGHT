#pragma once

#include "CHTL/CHTLNode/TemplateStyleDefinitionNode.h"
#include "CHTL/CHTLNode/TemplateElementDefinitionNode.h"
#include "CHTL/CHTLNode/TemplateVarDefinitionNode.h"
#include "CHTL/CHTLNode/OriginNode.h"
#include <map>
#include <string>
#include <memory>

namespace CHTL {

struct SymbolTable {
    std::map<std::string, std::shared_ptr<TemplateStyleDefinitionNode>> styleTemplates;
    std::map<std::string, std::shared_ptr<TemplateElementDefinitionNode>> elementTemplates;
    std::map<std::string, std::shared_ptr<TemplateVarDefinitionNode>> varTemplates;
    std::map<std::string, std::shared_ptr<OriginNode>> originTemplates;
};

} // namespace CHTL
