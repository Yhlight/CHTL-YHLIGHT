#pragma once

#include "../CHTLJSLexer/Token.h"
#include "../CHTLJSNode/BaseNode.h"
#include "../CHTLJSNode/ProgramNode.h"
#include <vector>
#include <memory>

class CHTLJSParser {
public:
    CHTLJSParser(const std::vector<CHTLJSToken>& tokens);
    std::unique_ptr<ProgramNode> parse();

private:
    std::unique_ptr<CHTLJSBaseNode> parseStatement();
    std::vector<CHTLJSToken> tokens;
    size_t pos = 0;
};
