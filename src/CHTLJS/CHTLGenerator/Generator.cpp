#include "Generator.h"
#include "../CHTLNode/BinaryExprNode.h"
#include "../CHTLNode/LiteralNode.h"
#include "../CHTLNode/SelectorExprNode.h"
#include "../CHTLNode/ListenNode.h"
#include "../CHTLNode/DelegateNode.h"
#include "../CHTLNode/AnimateNode.h"
#include "../CHTLNode/VirNode.h"
#include "../CHTLNode/IdentifierNode.h"
#include "../CHTLNode/ProgramNode.h"
#include "../CHTLNode/VirUsageNode.h"
#include "../CHTLNode/RouterNode.h"
#include "../CHTLNode/MemberExprNode.h"
#include <stdexcept>
#include <sstream>

namespace CHTLJS {

Generator::Generator(ASTNode& root, Analyser& analyser) : m_root(root), m_analyser(analyser) {}

std::string Generator::generate() {
    return visit(&m_root);
}

std::string Generator::visit(ASTNode* node) {
    switch (node->getType()) {
        case ASTNodeType::Program:
            return visitProgramNode(static_cast<ProgramNode*>(node));
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
        case ASTNodeType::Animate:
            return visitAnimateNode(static_cast<AnimateNode*>(node));
        case ASTNodeType::Vir:
            return visitVirNode(static_cast<VirNode*>(node));
        case ASTNodeType::Identifier:
            return visitIdentifierNode(static_cast<IdentifierNode*>(node));
        case ASTNodeType::VirUsage:
            return visitVirUsageNode(static_cast<VirUsageNode*>(node));
        case ASTNodeType::Router:
            return visitRouterNode(static_cast<RouterNode*>(node));
        case ASTNodeType::MemberExpr:
            return visitMemberExprNode(static_cast<MemberExprNode*>(node));
        default:
            throw std::runtime_error("Unknown node type");
    }
}

std::string Generator::visitMemberExprNode(MemberExprNode* node) {
    return visit(node->object.get()) + "." + visit(node->property.get());
}

std::string Generator::visitVirUsageNode(VirUsageNode* node) {
    const VirNode* vir = m_analyser.getVir(node->vir->getName());
    if (vir) {
        auto cloned_expr = vir->getExpression()->clone_expr();
        switch (cloned_expr->getType()) {
            case ASTNodeType::Listen: {
                auto* listenNode = static_cast<ListenNode*>(cloned_expr.get());
                listenNode->selector = std::unique_ptr<SelectorExprNode>(static_cast<SelectorExprNode*>(node->selector->clone_expr().release()));
                break;
            }
            case ASTNodeType::Delegate: {
                auto* delegateNode = static_cast<DelegateNode*>(cloned_expr.get());
                delegateNode->parentSelector = std::unique_ptr<SelectorExprNode>(static_cast<SelectorExprNode*>(node->selector->clone_expr().release()));
                break;
            }
            case ASTNodeType::Animate: {
                auto* animateNode = static_cast<AnimateNode*>(cloned_expr.get());
                animateNode->target = std::unique_ptr<SelectorExprNode>(static_cast<SelectorExprNode*>(node->selector->clone_expr().release()));
                break;
            }
            default:
                break;
        }
        return visit(cloned_expr.get());
    }
    throw std::runtime_error("Undeclared virtual object: " + node->vir->getName());
}

std::string Generator::visitIdentifierNode(IdentifierNode* node) {
    const VirNode* vir = m_analyser.getVir(node->getName());
    if (vir) {
        return visit(const_cast<ExprNode*>(vir->getExpression()));
    }
    return node->getName();
}

std::string Generator::visitVirNode(VirNode* node) {
    return "";
}

std::string Generator::visitProgramNode(ProgramNode* node) {
    std::stringstream ss;
    for (const auto& stmt : node->statements) {
        std::string generated = visit(stmt.get());
        if (!generated.empty()) {
            ss << generated << ";\n";
        }
    }
    return ss.str();
}

std::string Generator::visitRouterNode(RouterNode* node) {
    std::stringstream ss;
    ss << "const routes = {\n";
    for (const auto& route : node->routes) {
        ss << "  '" << route.url << "': " << visit(route.page.get()) << ",\n";
    }
    ss << "};\n";

    if (node->root_container.has_value()) {
        ss << "const rootContainer = " << visit(node->root_container.value().get()) << ";\n";
    }

    ss << "const router = () => {\n";
    ss << "  const path = window.location.hash.slice(1) || '/';\n";
    ss << "  const page = routes[path];\n";
    ss << "  if (page) {\n";
    ss << "    Object.values(routes).forEach(p => p.style.display = 'none');\n";
    ss << "    page.style.display = 'block';\n";
    ss << "  }\n";
    ss << "};\n";
    ss << "window.addEventListener('hashchange', router);\n";
    ss << "window.addEventListener('load', router);\n";

    return ss.str();
}

std::string Generator::visitBinaryExpr(BinaryExprNode* node) {
    return visit(node->left.get()) + " " + std::string(node->op.lexeme) + " " + visit(node->right.get());
}

std::string Generator::visitLiteral(LiteralNode* node) {
    return node->value;
}

std::string Generator::getSelectorString(SelectorExprNode* node) {
    std::string query;
    switch (node->type) {
        case SelectorType::Id:
            query = "#" + node->baseName;
            break;
        case SelectorType::Class:
            query = "." + node->baseName;
            break;
        case SelectorType::Tag:
            query = node->baseName;
            break;
        case SelectorType::Compound:
            query = "." + node->baseName + " " + node->descendant;
            break;
    }
    return query;
}

std::string Generator::visitSelectorExpr(SelectorExprNode* node) {
    std::string query;
    switch (node->type) {
        case SelectorType::Id:
            query = "document.getElementById(\"" + node->baseName + "\")";
            break;
        default:
            query = "document.querySelectorAll(\"" + getSelectorString(node) + "\")";
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
           << node->handlers[i]->handlerBody << ");";
        if (i < node->handlers.size() - 1) {
            ss << "\n";
        }
    }
    return ss.str();
}

std::string Generator::visitDelegateNode(DelegateNode* node) {
    std::stringstream ss;
    std::string parentSelector = visit(node->parentSelector.get());
    std::string targetSelectorString = getSelectorString(node->targetSelector.get());

    for (size_t i = 0; i < node->handlers.size(); ++i) {
        ss << parentSelector << ".addEventListener('" << node->handlers[i]->eventName << "', event => {\n"
           << "  if (event.target.matches('" << targetSelectorString << "')) {\n"
           << "    (" << node->handlers[i]->handlerBody << ")(event);\n"
           << "  }\n"
           << "});";
        if (i < node->handlers.size() - 1) {
           ss << "\n";
        }
    }
    return ss.str();
}

std::string Generator::visitAnimateNode(AnimateNode* node) {
    std::stringstream ss;
    std::string target = visit(node->target.get());

    ss << "const anim = " << target << ".animate([\n";

    auto write_css_properties = [&](const std::vector<std::unique_ptr<CssPropertyNode>>& props) {
        for (size_t i = 0; i < props.size(); ++i) {
            ss << "      " << props[i]->name << ": \"" << props[i]->value << "\"";
            if (i < props.size() - 1) {
                ss << ",\n";
            }
        }
    };

    if (node->begin.has_value()) {
        ss << "  {\n";
        write_css_properties(node->begin.value());
        ss << "\n  },\n";
    }

    for (const auto& keyframe : node->when) {
        ss << "  {\n";
        write_css_properties(keyframe->properties);
        ss << ",\n      offset: " << keyframe->at;
        ss << "\n  },\n";
    }

    if (node->end.has_value()) {
        ss << "  {\n";
        write_css_properties(node->end.value());
        ss << "\n  }\n";
    }

    ss << "], {\n";
    ss << "  duration: " << node->duration << ",\n";
    ss << "  easing: \"" << node->easing << "\"";
    if (node->loop.has_value()) ss << ",\n  iterations: " << node->loop.value();
    if (node->direction.has_value()) ss << ",\n  direction: \"" << node->direction.value() << "\"";
    if (node->delay.has_value()) ss << ",\n  delay: " << node->delay.value();
    ss << "\n});";

    if (node->callback.has_value()) {
        ss << "\nanim.onfinish = " << node->callback.value() << ";";
    }

    return ss.str();
}

} // namespace CHTLJS
