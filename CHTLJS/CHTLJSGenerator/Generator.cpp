#include "Generator.h"
#include "../CHTLJSNode/ProgramNode.h"
#include "../CHTLJSNode/EnhancedSelectorNode.h"
#include "../CHTLJSNode/IdentifierNode.h"
#include "../CHTLJSNode/MemberExpressionNode.h"
#include "../CHTLJSNode/CallExpressionNode.h"
#include "../CHTLJSNode/StringLiteralNode.h"

CHTLJSGenerator::CHTLJSGenerator() {}

std::string CHTLJSGenerator::generate(CHTLJSBaseNode* node) {
    if (!node) return "";
    if (auto programNode = dynamic_cast<ProgramNode*>(node)) {
        std::string result;
        for (const auto& child : programNode->children) {
            if (child) {
                result += generate(child.get()) + ";\n";
            }
        }
        return result;
    }
    if (auto callNode = dynamic_cast<CallExpressionNode*>(node)) {
        std::string result = generate(callNode->callee.get()) + "(";
        for (size_t i = 0; i < callNode->arguments.size(); ++i) {
            result += generate(callNode->arguments[i].get());
            if (i < callNode->arguments.size() - 1) {
                result += ", ";
            }
        }
        result += ")";
        return result;
    }
    if (auto memberNode = dynamic_cast<MemberExpressionNode*>(node)) {
        return generate(memberNode->object.get()) + "." + generate(memberNode->property.get());
    }
    if (auto selectorNode = dynamic_cast<EnhancedSelectorNode*>(node)) {
        return "document.querySelector('" + selectorNode->selector + "')";
    }
    if (auto identifierNode = dynamic_cast<IdentifierNode*>(node)) {
        return identifierNode->name;
    }
    if (auto stringNode = dynamic_cast<StringLiteralNode*>(node)) {
        return "\"" + stringNode->value + "\"";
    }
    return "";
}
