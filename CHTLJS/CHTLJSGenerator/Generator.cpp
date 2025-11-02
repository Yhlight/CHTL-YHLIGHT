#include "Generator.h"
#include "../CHTLJSNode/EnhancedSelectorNode.h"
#include "../CHTLJSNode/ProgramNode.h"

CHTLJSGenerator::CHTLJSGenerator() {}

std::string CHTLJSGenerator::generate(const std::unique_ptr<CHTLJSBaseNode>& node) {
    if (auto programNode = dynamic_cast<ProgramNode*>(node.get())) {
        std::string result;
        for (const auto& child : programNode->children) {
            if (child) {
                result += generate(child) + ";\n";
            }
        }
        return result;
    }
    if (auto selectorNode = dynamic_cast<EnhancedSelectorNode*>(node.get())) {
        return "document.querySelector('" + selectorNode->selector + "')";
    }
    return "";
}
