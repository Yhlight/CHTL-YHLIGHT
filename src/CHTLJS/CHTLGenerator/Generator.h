#pragma once

#include "../CHTLNode/ASTNode.h"
#include "../CHTLNode/ExprNode.h"
#include <string>

namespace CHTLJS {

class Generator {
public:
    Generator(ExprNode& root);
    std::string generate();

private:
    std::string visit(ExprNode* node);
    std::string visitBinaryExpr(class BinaryExprNode* node);
    std::string visitLiteral(class LiteralNode* node);
    std::string visitSelectorExpr(class SelectorExprNode* node);

    ExprNode& m_root;
};

} // namespace CHTLJS
