#pragma once

#include "Token.h"
#include <string>
#include <vector>

class CHTLJSLexer {
public:
    CHTLJSLexer(const std::string& source);
    std::vector<CHTLJSToken> tokenize();

private:
    std::string source;
    size_t pos = 0;
};
