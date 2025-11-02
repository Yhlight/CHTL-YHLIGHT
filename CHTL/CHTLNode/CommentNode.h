#pragma once

#include "BaseNode.h"
#include "CHTLLexer/Token.h"
#include <string>

class CommentNode : public BaseNode {
public:
    std::string comment;
    TokenType type;
};
