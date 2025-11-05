#pragma once

#include "../CHTLNode/ASTNode.h"
#include <string>
#include <sstream>

namespace CHTL {

class Generator {
public:
    Generator();
    std::string generate(ASTNode* root);

private:
    void visit(ASTNode* node);
    void visitProgramNode(ASTNode* node);
    void visitElementNode(ASTNode* node);
    void visitTextNode(ASTNode* node);
    void visitStyleNode(ASTNode* node);
    void visitStylePropertyNode(ASTNode* node);
    void visitScriptNode(ASTNode* node);

    std::stringstream output_;
};

} // namespace CHTL
