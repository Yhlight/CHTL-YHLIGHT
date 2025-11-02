#pragma once

#include "CHTLNode/StyleContentNode.h"
#include "CHTLLexer/Token.h"
#include <vector>
#include <memory>

class StyleParser {
public:
    StyleParser(std::vector<Token> tokens);
    std::vector<std::unique_ptr<StyleContentNode>> parse();

private:
    std::vector<Token> tokens;
    size_t current = 0;

    Token advance();
    Token peek();
    bool isAtEnd();
};
