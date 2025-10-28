#pragma once

#include "CHTL/CHTLNode/ASTNode.h"
#include <string>
#include <variant>

namespace CHTL {

struct NumericValue {
    double value;
    std::string unit;
};

// A variant to hold the result of an expression evaluation.
using EvaluatedValue = std::variant<NumericValue, std::string>;

class ExpressionEvaluator {
public:
    ExpressionEvaluator() = default;

    // Evaluates an expression tree and returns the final value as a string.
    std::string evaluate(const ASTNode* expression);

private:
    // Visitor methods for each node type
    EvaluatedValue visit(const ASTNode* node);
    EvaluatedValue visit(const NumberLiteralNode* node);
    EvaluatedValue visit(const StringLiteralNode* node);
    EvaluatedValue visit(const IdentifierNode* node);
    EvaluatedValue visit(const BinaryOpNode* node);
};

} // namespace CHTL
