#pragma once

#include "Token.h"
#include <string>

namespace CHTL {

class Lexer {
public:
    Lexer(const std::string& source);

    Token getNextToken();

private:
    std::string source;
    size_t position;
};

} // namespace CHTL
