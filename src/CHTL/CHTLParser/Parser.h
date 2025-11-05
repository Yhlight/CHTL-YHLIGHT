#pragma once

#include "Token.h"
#include "BaseNode.h"
#include "ElementNode.h"
#include "TextNode.h"
#include <vector>
#include <memory>

class Parser {
public:
    Parser(const std::vector<Token>& tokens);
    std::unique_ptr<BaseNode> parse();

private:
    std::unique_ptr<ElementNode> parse_element();
    std::unique_ptr<TextNode> parse_text();
    Token current_token();
    void advance();

    const std::vector<Token>& tokens;
    int current_token_index;
};
