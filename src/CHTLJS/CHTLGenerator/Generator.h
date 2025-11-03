#pragma once

#include "../CHTLNode/ASTNode.h"
#include "../CHTLNode/ExprNode.h"
#include "../CHTLNode/ListenNode.h"
#include "../CHTLNode/DelegateNode.h"
#include "../CHTLNode/AnimateNode.h"
#include "../CHTLNode/IdentifierNode.h"
#include "../CHTLNode/ProgramNode.h"
#include "../CHTLNode/VirUsageNode.h"
#include "../CHTLNode/RouterNode.h"
#include "../CHTLAnalyser/Analyser.h"
#include <string>

namespace CHTLJS {

class Generator {
public:
    Generator(ASTNode& root, Analyser& analyser);
    std::string generate();

private:
    std::string visit(ASTNode* node);
    std::string visitProgramNode(class ProgramNode* node);
    std::string visitBinaryExpr(class BinaryExprNode* node);
    std::string visitLiteral(class LiteralNode* node);
    std::string visitSelectorExpr(class SelectorExprNode* node);
    std::string visitListenNode(class ListenNode* node);
    std::string visitDelegateNode(class DelegateNode* node);
    std::string visitAnimateNode(class AnimateNode* node);
	std::string visitVirNode(class VirNode* node);
	std::string visitIdentifierNode(class IdentifierNode* node);
	std::string visitVirUsageNode(class VirUsageNode* node);
    std::string visitRouterNode(class RouterNode* node);

    std::string getSelectorString(class SelectorExprNode* node);

    ExprNode& m_root;
	Analyser& m_analyser;
};

} // namespace CHTLJS
