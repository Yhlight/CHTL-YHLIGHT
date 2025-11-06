#pragma once

#include "ExprNode.h"
#include "CHTLJSLexer/Token.h"

namespace CHTLJS {

class MemberAccessExprNode : public ExprNode {
public:
    MemberAccessExprNode(std::unique_ptr<ExprNode> object, Token member);

    const ExprNode& getObject() const;
    const Token& getMember() const;

private:
    std::unique_ptr<ExprNode> object_;
    Token member_;
};

}
