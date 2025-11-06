#include "Generator.h"

namespace CHTLJS {

std::string Generator::generate(const ProgramNode& program) {
    return visit(program);
}

std::string Generator::visit(const ExprNode& node) {
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
    }
    return "";
}

std::string Generator::visitProgramNode(const ProgramNode& node) {
    std::string result;
    for (const auto& expr : node.getExpressions()) {
        result += visit(*expr);
    }
    return result;
}

std::string Generator::visitLiteralExprNode(const LiteralExprNode& node) {
    return node.getValue();
}

std::string Generator::visitSelectorExprNode(const SelectorExprNode& node) {
    std::string selector_string;
    bool has_index = false;
    std::string index;

    for (const auto& component : node.getComponents()) {
        switch (component.type) {
            case SelectorComponentType::TAG:
                selector_string += component.value;
                break;
            case SelectorComponentType::ID:
                selector_string += "#" + component.value;
                break;
            case SelectorComponentType::CLASS:
                selector_string += "." + component.value;
                break;
            case SelectorComponentType::DESCENDANT:
                selector_string += " ";
                break;
            case SelectorComponentType::INDEX:
                has_index = true;
                index = component.value;
                break;
        }
    }

    if (has_index) {
        return "document.querySelectorAll('" + selector_string + "')[" + index + "]";
    } else {
        return "document.querySelector('" + selector_string + "')";
    }
}

std::string Generator::visitBinaryExprNode(const BinaryExprNode& node) {
    return "";
}

std::string Generator::visitUnaryExprNode(const UnaryExprNode& node) {
    return "";
}

std::string Generator::visitCallExprNode(const CallExprNode& node) {
    std::string result = visit(node.getCallee());
    result += "(";
    for (size_t i = 0; i < node.getArguments().size(); ++i) {
        result += visit(*node.getArguments()[i]);
        if (i < node.getArguments().size() - 1) {
            result += ", ";
        }
    }
    result += ")";
    return result;
}

std::string Generator::visitMemberAccessExprNode(const MemberAccessExprNode& node) {
    std::string result = visit(node.getObject());
    result += ".";
    result += node.getMember().lexeme;
    return result;
}

}
