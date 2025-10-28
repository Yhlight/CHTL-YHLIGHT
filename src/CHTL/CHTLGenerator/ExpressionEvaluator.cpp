#include "ExpressionEvaluator.h"
#include "CHTL/Util/StringUtil.h"
#include <stdexcept>
#include <cmath>

namespace CHTL {

std::string ExpressionEvaluator::evaluate(const ASTNode* expression) {
    EvaluatedValue result = visit(expression);
    if (std::holds_alternative<NumericValue>(result)) {
        auto num = std::get<NumericValue>(result);
        return formatDouble(num.value) + num.unit;
    }

    auto str_val = std::get<std::string>(result);
    // A more robust way would be to add another type to the variant.
    if (expression->getType() == NodeType::StringLiteral) {
        return "\"" + str_val + "\"";
    }
    return str_val;
}

EvaluatedValue ExpressionEvaluator::visit(const ASTNode* node) {
    switch (node->getType()) {
        case NodeType::NumberLiteral:
            return visit(static_cast<const NumberLiteralNode*>(node));
        case NodeType::StringLiteral:
             return visit(static_cast<const StringLiteralNode*>(node));
        case NodeType::Identifier:
            return visit(static_cast<const IdentifierNode*>(node));
        case NodeType::BinaryOp:
            return visit(static_cast<const BinaryOpNode*>(node));
        default:
            throw std::runtime_error("Unsupported node type in expression evaluator.");
    }
}

EvaluatedValue ExpressionEvaluator::visit(const NumberLiteralNode* node) {
    return NumericValue{node->value, node->unit};
}

EvaluatedValue ExpressionEvaluator::visit(const StringLiteralNode* node) {
    return node->value;
}

EvaluatedValue ExpressionEvaluator::visit(const IdentifierNode* node) {
    return node->name;
}

EvaluatedValue ExpressionEvaluator::visit(const BinaryOpNode* node) {
    EvaluatedValue left = visit(node->left.get());
    EvaluatedValue right = visit(node->right.get());

    if (std::holds_alternative<NumericValue>(left) && std::holds_alternative<NumericValue>(right)) {
        auto left_num = std::get<NumericValue>(left);
        auto right_num = std::get<NumericValue>(right);

        // Unit compatibility check
        if (!left_num.unit.empty() && !right_num.unit.empty() && left_num.unit != right_num.unit) {
            throw std::runtime_error("Mismatched units in arithmetic operation: " + left_num.unit + " and " + right_num.unit);
        }

        std::string result_unit = !left_num.unit.empty() ? left_num.unit : right_num.unit;
        double result_val = 0;

        switch (node->op.type) {
            case TokenType::PLUS: result_val = left_num.value + right_num.value; break;
            case TokenType::MINUS: result_val = left_num.value - right_num.value; break;
            case TokenType::STAR: result_val = left_num.value * right_num.value; break;
            case TokenType::SLASH:
                if (right_num.value == 0) throw std::runtime_error("Division by zero.");
                result_val = left_num.value / right_num.value;
                break;
            case TokenType::PERCENT: result_val = fmod(left_num.value, right_num.value); break;
            case TokenType::STAR_STAR: result_val = pow(left_num.value, right_num.value); break;
            default: throw std::runtime_error("Unsupported operator for numeric types.");
        }
        return NumericValue{result_val, result_unit};
    }

    // Handle string concatenation for identifiers like `center`, `solid`, etc.
    if (node->op.type == TokenType::PLUS) {
         auto left_str = std::holds_alternative<NumericValue>(left) ? (formatDouble(std::get<NumericValue>(left).value) + std::get<NumericValue>(left).unit) : std::get<std::string>(left);
         auto right_str = std::holds_alternative<NumericValue>(right) ? (formatDouble(std::get<NumericValue>(right).value) + std::get<NumericValue>(right).unit) : std::get<std::string>(right);
         return left_str + " " + right_str; // Add space for css values like `1px solid red`
    }

    throw std::runtime_error("Unsupported operator for the given types in binary operation.");
}

} // namespace CHTL
