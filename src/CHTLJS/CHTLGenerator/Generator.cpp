#include "Generator.h"
#include "../CHTLNode/BinaryExprNode.h"
#include "../CHTLNode/LiteralNode.h"
#include "../CHTLNode/SelectorExprNode.h"
#include "../CHTLNode/ListenNode.h"
#include "../CHTLNode/DelegateNode.h"
#include "../CHTLNode/AnimateNode.h"
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
        case ASTNodeType::Animate:
            return visitAnimateNode(static_cast<AnimateNode*>(node));
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
