#pragma once

#include "CHTLJSNode/ExprNode.h"
#include <string>
#include <memory>

namespace CHTLJS {

// Forward declarations for all node types to be visited
class ProgramNode;
class LiteralExprNode;
class SelectorExprNode;
class BinaryExprNode;
class UnaryExprNode;
class CallExprNode;
class MemberAccessExprNode;

class CHTLJSGenerator {
public:
    CHTLJSGenerator() = default;
    std::string generate(const ExprNode& node);

private:
    std::string visit(const ExprNode& node);
    std::string visitProgramNode(const ProgramNode& node);
    std::string visitLiteralExprNode(const LiteralExprNode& node);
    std::string visitSelectorExprNode(const SelectorExprNode& node);
    std::string visitBinaryExprNode(const BinaryExprNode& node);
    std::string visitUnaryExprNode(const UnaryExprNode& node);
    std::string visitCallExprNode(const CallExprNode& node);
    std::string visitMemberAccessExprNode(const MemberAccessExprNode& node);
};

}
