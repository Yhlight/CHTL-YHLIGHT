#include "ExpressionEvaluator.h"
#include "CHTL/Util/StringUtil.h"
#include <stdexcept>
#include <cmath>

namespace CHTL {

bool ExpressionEvaluator::isTruthy(const EvaluatedValue& value) {
    if (std::holds_alternative<bool>(value)) {
        return std::get<bool>(value);
    }
    if (std::holds_alternative<NumericValue>(value)) {
        return std::get<NumericValue>(value).value != 0;
    }
    if (std::holds_alternative<StringValue>(value)) {
        return !std::get<StringValue>(value).value.empty();
    }
    return false;
}

std::string ExpressionEvaluator::evaluate(const ASTNode* expression) {
    EvaluatedValue result = visit(expression);
    if (std::holds_alternative<NumericValue>(result)) {
        auto num = std::get<NumericValue>(result);
        return formatDouble(num.value) + num.unit;
    }
    if (std::holds_alternative<StringValue>(result)) {
        auto str = std::get<StringValue>(result);
        if (str.type == StringType::Literal) {
            return "\"" + str.value + "\"";
        }
        return str.value;
    }
    // A top-level expression should not evaluate to a boolean.
    // Booleans are intermediate values for conditions.
    throw std::runtime_error("Expression evaluated to an unexpected boolean value.");
}

EvaluatedValue ExpressionEvaluator::visit(const ASTNode* node) {
    if (!node) {
        throw std::runtime_error("Cannot visit a null node.");
    }
    switch (node->getType()) {
        case NodeType::NumberLiteral:
            return visit(static_cast<const NumberLiteralNode*>(node));
        case NodeType::StringLiteral:
             return visit(static_cast<const StringLiteralNode*>(node));
        case NodeType::Identifier:
            return visit(static_cast<const IdentifierNode*>(node));
        case NodeType::BinaryOp:
            return visit(static_cast<const BinaryOpNode*>(node));
        case NodeType::TernaryOp:
            return visit(static_cast<const TernaryOpNode*>(node));
        case NodeType::PropertyAccess:
            // This should have been resolved by the analyser. If we get here, it's a bug.
            throw std::runtime_error("PropertyAccessNode should have been resolved by the Analyser.");
        default:
            throw std::runtime_error("Unsupported node type in expression evaluator.");
    }
}

EvaluatedValue ExpressionEvaluator::visit(const NumberLiteralNode* node) {
    return NumericValue{node->value, node->unit};
}

EvaluatedValue ExpressionEvaluator::visit(const StringLiteralNode* node) {
    return StringValue{node->value, StringType::Literal};
}

EvaluatedValue ExpressionEvaluator::visit(const IdentifierNode* node) {
    return StringValue{node->name, StringType::Identifier};
}

EvaluatedValue ExpressionEvaluator::visit(const TernaryOpNode* node) {
    EvaluatedValue condition = visit(node->condition.get());
    if (isTruthy(condition)) {
        return visit(node->then_expr.get());
    } else {
        return visit(node->else_expr.get());
    }
}

EvaluatedValue ExpressionEvaluator::visit(const BinaryOpNode* node) {
    if (node->op.type == TokenType::PIPE_PIPE) {
        return isTruthy(visit(node->left.get())) || isTruthy(visit(node->right.get()));
    }
    if (node->op.type == TokenType::AMPERSAND_AMPERSAND) {
        return isTruthy(visit(node->left.get())) && isTruthy(visit(node->right.get()));
    }

    EvaluatedValue left = visit(node->left.get());
    EvaluatedValue right = visit(node->right.get());

    if (std::holds_alternative<NumericValue>(left) && std::holds_alternative<NumericValue>(right)) {
        auto left_num = std::get<NumericValue>(left);
        auto right_num = std::get<NumericValue>(right);

        switch (node->op.type) {
            case TokenType::GREATER:       return left_num.value > right_num.value;
            case TokenType::GREATER_EQUAL: return left_num.value >= right_num.value;
            case TokenType::LESS:          return left_num.value < right_num.value;
            case TokenType::LESS_EQUAL:    return left_num.value <= right_num.value;
            case TokenType::EQUAL_EQUAL:   return left_num.value == right_num.value;
            case TokenType::BANG_EQUAL:    return left_num.value != right_num.value;
            default: break;
        }

        if (!left_num.unit.empty() && !right_num.unit.empty() && left_num.unit != right_num.unit) {
            throw std::runtime_error("Mismatched units in arithmetic operation: " + left_num.unit + " and " + right_num.unit);
        }
        std::string result_unit = !left_num.unit.empty() ? left_num.unit : right_num.unit;

        switch (node->op.type) {
            case TokenType::PLUS: return NumericValue{left_num.value + right_num.value, result_unit};
            case TokenType::MINUS: return NumericValue{left_num.value - right_num.value, result_unit};
            case TokenType::STAR: return NumericValue{left_num.value * right_num.value, result_unit};
            case TokenType::SLASH:
                if (right_num.value == 0) throw std::runtime_error("Division by zero.");
                return NumericValue{left_num.value / right_num.value, result_unit};
            case TokenType::PERCENT: return NumericValue{fmod(left_num.value, right_num.value), result_unit};
            case TokenType::STAR_STAR: return NumericValue{pow(left_num.value, right_num.value), result_unit};
            default: break;
        }
    }

    if (std::holds_alternative<StringValue>(left) && std::holds_alternative<StringValue>(right)) {
        auto left_str = std::get<StringValue>(left).value;
        auto right_str = std::get<StringValue>(right).value;
        if (node->op.type == TokenType::EQUAL_EQUAL) return left_str == right_str;
        if (node->op.type == TokenType::BANG_EQUAL) return left_str != right_str;
        if (node->op.type == TokenType::PLUS) {
            return StringValue{left_str + right_str, StringType::Identifier};
        }
    }

    if (node->op.type == TokenType::PLUS) {
        if (std::holds_alternative<StringValue>(left) && std::holds_alternative<NumericValue>(right)) {
            return StringValue{std::get<StringValue>(left).value + formatDouble(std::get<NumericValue>(right).value) + std::get<NumericValue>(right).unit, StringType::Identifier};
        }
        if (std::holds_alternative<NumericValue>(left) && std::holds_alternative<StringValue>(right)) {
            return StringValue{formatDouble(std::get<NumericValue>(left).value) + std::get<NumericValue>(left).unit + std::get<StringValue>(right).value, StringType::Identifier};
        }
    }


    throw std::runtime_error("Unsupported operator for the given types in binary operation.");
}

} // namespace CHTL
