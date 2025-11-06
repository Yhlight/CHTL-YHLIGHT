#include "CHTLJSGenerator.h"
#include "CHTLJSNode/ProgramNode.h"
#include "CHTLJSNode/LiteralExprNode.h"
#include "CHTLJSNode/SelectorExprNode.h"
#include "CHTLJSNode/BinaryExprNode.h"
#include "CHTLJSNode/UnaryExprNode.h"
#include "CHTLJSNode/CallExprNode.h"
#include "CHTLJSNode/MemberAccessExprNode.h"
#include <stdexcept>
#include <sstream>

namespace CHTLJS {

std::string CHTLJSGenerator::generate(const ExprNode& node) {
    return visit(node);
}

std::string CHTLJSGenerator::visit(const ExprNode& node) {
    switch (node.getType()) {
        case ExprNodeType::Program:
            return visitProgramNode(static_cast<const ProgramNode&>(node));
        case ExprNodeType::Literal:
            return visitLiteralExprNode(static_cast<const LiteralExprNode&>(node));
        case ExprNodeType::Selector:
            return visitSelectorExprNode(static_cast<const SelectorExprNode&>(node));
        case ExprNodeType::Binary:
            return visitBinaryExprNode(static_cast<const BinaryExprNode&>(node));
        case ExprNodeType::Unary:
            return visitUnaryExprNode(static_cast<const UnaryExprNode&>(node));
        case ExprNodeType::Call:
            return visitCallExprNode(static_cast<const CallExprNode&>(node));
        case ExprNodeType::MemberAccess:
            return visitMemberAccessExprNode(static_cast<const MemberAccessExprNode&>(node));
        default:
            throw std::runtime_error("Unknown expression node type.");
    }
}

std::string CHTLJSGenerator::visitProgramNode(const ProgramNode& node) {
    std::stringstream ss;
    for (const auto& expr : node.getExpressions()) {
        ss << visit(*expr) << ";\n";
    }
    return ss.str();
}

std::string CHTLJSGenerator::visitLiteralExprNode(const LiteralExprNode& node) {
    return node.getValue();
}

std::string CHTLJSGenerator::visitSelectorExprNode(const SelectorExprNode& node) {
    std::stringstream ss;
    for (const auto& component : node.getComponents()) {
        switch (component.type) {
            case SelectorComponentType::TAG:
                ss << component.value;
                break;
            case SelectorComponentType::ID:
                ss << "#" << component.value;
                break;
            case SelectorComponentType::CLASS:
                ss << "." << component.value;
                break;
            case SelectorComponentType::DESCENDANT:
                ss << " ";
                break;
            case SelectorComponentType::INDEX:
                // This will be handled by querySelectorAll()[index]
                break;
        }
    }

    std::string selector_string = ss.str();
    bool has_index = false;
    std::string index_value;
    for (const auto& component : node.getComponents()) {
        if (component.type == SelectorComponentType::INDEX) {
            has_index = true;
            index_value = component.value;
            break;
        }
    }

    if (has_index) {
        return "document.querySelectorAll('" + selector_string + "')[" + index_value + "]";
    } else {
        return "document.querySelector('" + selector_string + "')";
    }
}

std::string CHTLJSGenerator::visitBinaryExprNode(const BinaryExprNode& node) {
    std::string left = visit(*node.getLeft());
    std::string op(node.getOperator().lexeme);
    std::string right = visit(*node.getRight());
    return left + " " + op + " " + right;
}

std::string CHTLJSGenerator::visitUnaryExprNode(const UnaryExprNode& node) {
    std::string op(node.getOperator().lexeme);
    std::string right = visit(*node.getRight());
    return op + right;
}

std::string CHTLJSGenerator::visitCallExprNode(const CallExprNode& node) {
    std::string callee = visit(node.getCallee());
    std::stringstream ss;
    ss << callee << "(";
    const auto& args = node.getArguments();
    for (size_t i = 0; i < args.size(); ++i) {
        ss << visit(*args[i]);
        if (i < args.size() - 1) {
            ss << ", ";
        }
    }
    ss << ")";
    return ss.str();
}

std::string CHTLJSGenerator::visitMemberAccessExprNode(const MemberAccessExprNode& node) {
    std::string object = visit(node.getObject());
    std::string member(node.getMember().lexeme);
    return object + "." + member;
}

}
