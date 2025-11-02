#include "Generator.h"
#include "../CHTLNode/BinaryExprNode.h"
#include "../CHTLNode/LiteralNode.h"
#include "../CHTLNode/SelectorExprNode.h"
#include <stdexcept>

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

} // namespace CHTLJS
