#pragma once

#include "../CHTLNode/AST.h"
#include <string>
#include <sstream>

class Generator {
public:
    std::string generate(const ASTNode& node);
private:
    void generateStyle(std::stringstream& ss, const ElementNode& node);
};
