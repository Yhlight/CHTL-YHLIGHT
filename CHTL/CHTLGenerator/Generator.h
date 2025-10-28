#pragma once

#include "../CHTLNode/AST.h"
#include <string>

class Generator {
public:
    std::string generate(const ASTNode& node);
};
