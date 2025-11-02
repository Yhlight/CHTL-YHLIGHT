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
    return "document.querySelector(\"" + node->selector + "\")";
}

} // namespace CHTLJS
