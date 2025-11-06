#pragma once

#include "CHTLJSNode/ExprNode.h"
#include "CHTLJSNode/ProgramNode.h"
#include "CHTLJSNode/LiteralExprNode.h"
#include "CHTLJSNode/SelectorExprNode.h"
#include "CHTLJSNode/BinaryExprNode.h"
#include "CHTLJSNode/UnaryExprNode.h"
#include "CHTLJSNode/CallExprNode.h"
#include "CHTLJSNode/MemberAccessExprNode.h"
#include <string>
#include <vector>
#include <memory>

namespace CHTLJS {

class Generator {
public:
    std::string generate(const ProgramNode& program);

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
