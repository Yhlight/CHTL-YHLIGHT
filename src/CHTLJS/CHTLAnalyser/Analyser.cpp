#include "Analyser.h"
#include "../CHTLNode/BinaryExprNode.h"
#include "../CHTLNode/ListenNode.h"
#include "../CHTLNode/DelegateNode.h"
#include "../CHTLNode/AnimateNode.h"
#include "../CHTLNode/VirUsageNode.h"
#include <stdexcept>

namespace CHTLJS {

Analyser::Analyser(ASTNode& root) : m_root(root) {}

void Analyser::analyse() {
    visit(&m_root);
}

const VirNode* Analyser::getVir(const std::string& name) const {
    if (m_virs.count(name)) {
        return m_virs.at(name);
    }
    return nullptr;
}

void Analyser::visit(ASTNode* node) {
    if (!node) return;
    switch (node->getType()) {
        case ASTNodeType::Program: {
            auto* programNode = static_cast<ProgramNode*>(node);
            for (const auto& stmt : programNode->statements) {
                visit(stmt.get());
            }
            break;
        }
        case ASTNodeType::Vir:
            visitVirNode(static_cast<VirNode*>(node));
            break;
        case ASTNodeType::VirUsage: {
            auto* usageNode = static_cast<VirUsageNode*>(node);
            visit(usageNode->selector.get());
            visit(usageNode->vir.get());
            break;
        }
        case ASTNodeType::BinaryExpr: {
            auto* binaryNode = static_cast<BinaryExprNode*>(node);
            visit(binaryNode->left.get());
            visit(binaryNode->right.get());
            break;
        }
        case ASTNodeType::Listen: {
            auto* listenNode = static_cast<ListenNode*>(node);
            visit(listenNode->selector.get());
            break;
        }
        case ASTNodeType::Delegate: {
            auto* delegateNode = static_cast<DelegateNode*>(node);
            visit(delegateNode->parentSelector.get());
            visit(delegateNode->targetSelector.get());
            break;
        }
        case ASTNodeType::Animate: {
            auto* animateNode = static_cast<AnimateNode*>(node);
            visit(animateNode->target.get());
            break;
        }
        default:
            break;
    }
}

void Analyser::visitVirNode(VirNode* node) {
    if (m_virs.count(node->getName())) {
        throw std::runtime_error("Virtual object '" + node->getName() + "' already defined.");
    }
    m_virs[node->getName()] = node;
    visit(const_cast<ExprNode*>(node->getExpression()));
}

} // namespace CHTLJS
