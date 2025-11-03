#include "Generator.h"
#include "../CHTLNode/BinaryExprNode.h"
#include "../CHTLNode/LiteralNode.h"
#include "../CHTLNode/SelectorExprNode.h"
#include "../CHTLNode/ListenNode.h"
#include "../CHTLNode/DelegateNode.h"
#include <stdexcept>
#include <sstream>

namespace CHTLJS {

Generator::Generator(ExprNode& root) : m_root(root) {}

std::string Generator::generate() {
    return visit(&m_root);
}
std::string Generator::visit(ExprNode* node) {
    switch (node->getType()) {
        case ASTNodeType::BinaryExpr:
            return visitBinaryExpr(static_cast<BinaryExprNode*>(node));
        case ASTNodeType::Literal:
            return visitLiteral(static_cast<LiteralNode*>(node));
        case ASTNodeType::SelectorExpr:
            return visitSelectorExpr(static_cast<SelectorExprNode*>(node));
        case ASTNodeType::Listen:
            return visitListenNode(static_cast<ListenNode*>(node));
        case ASTNodeType::Delegate:
            return visitDelegateNode(static_cast<DelegateNode*>(node));
        default:
            throw std::runtime_error("Unknown node type");
    }
}

std::string Generator::visitBinaryExpr(BinaryExprNode* node) {
    return visit(node->left.get()) + " " + std::string(node->op.lexeme) + " " + visit(node->right.get());
}

std::string Generator::visitLiteral(LiteralNode* node) {
    return node->value;
}

std::string Generator::visitSelectorExpr(SelectorExprNode* node) {
    std::string query;
    switch (node->type) {
        case SelectorType::Id:
            query = "document.getElementById(\"" + node->baseName + "\")";
            break;
        case SelectorType::Class:
            query = "document.querySelectorAll(\"." + node->baseName + "\")";
            break;
        case SelectorType::Tag:
            query = "document.querySelectorAll(\"" + node->baseName + "\")";
            break;
        case SelectorType::Compound:
            query = "document.querySelectorAll(\"." + node->baseName + " " + node->descendant + "\")";
            break;
    }

    if (node->index.has_value()) {
        query += "[" + std::to_string(node->index.value()) + "]";
    }

    return query;
}
std::string Generator::visitListenNode(ListenNode* node) {
    std::stringstream ss;
    std::string selector = visit(node->selector.get());
    for (size_t i = 0; i < node->handlers.size(); ++i) {
        ss << selector << ".addEventListener('" << node->handlers[i]->eventName << "', "
           << node->handlers[i]->handlerBody.substr(1, node->handlers[i]->handlerBody.length() - 2) << ");";
        if (i < node->handlers.size() - 1) {
            ss << "\n";
        }
    }
    return ss.str();
}

std::string Generator::visitDelegateNode(DelegateNode* node) {
    std::stringstream ss;
    std::string parentSelector = visit(node->parentSelector.get());
    std::string targetSelectorString;

    switch (node->targetSelector->type) {
        case SelectorType::Id:
            targetSelectorString = "#" + node->targetSelector->baseName;
            break;
        case SelectorType::Class:
            targetSelectorString = "." + node->targetSelector->baseName;
            break;
        case SelectorType::Tag:
            targetSelectorString = node->targetSelector->baseName;
            break;
        case SelectorType::Compound:
            targetSelectorString = "." + node->targetSelector->baseName + " " + node->targetSelector->descendant;
            break;
    }

    for (size_t i = 0; i < node->handlers.size(); ++i) {
        ss << parentSelector << ".addEventListener('" << node->handlers[i]->eventName << "', event => {\n"
           << "  if (event.target.matches('" << targetSelectorString << "')) {\n"
           << "    (" << node->handlers[i]->handlerBody.substr(1, node->handlers[i]->handlerBody.length() - 2) << ")(event);\n"
           << "  }\n"
           << "});";
        if (i < node->handlers.size() - 1) {
           ss << "\n";
        }
    }
    return ss.str();
}

} // namespace CHTLJS
