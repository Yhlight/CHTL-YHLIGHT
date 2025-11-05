#pragma once

#include "Token.h"
#include "BaseNode.h"
#include "ElementNode.h"
#include "TextNode.h"
#include "StyleNode.h"
#include "StylePropertyNode.h"
#include "StyleRuleNode.h"
#include "ScriptNode.h"
#include <vector>
#include <memory>
#include <string>

class Parser {
public:
    Parser(const std::string& source, const std::vector<Token>& tokens);
    std::unique_ptr<BaseNode> parse();

private:
    std::unique_ptr<ElementNode> parse_element();
    std::unique_ptr<StyleNode> parse_style();
    std::unique_ptr<ScriptNode> parse_script();
    Token current_token();
    void advance();

    const std::string& source;
    const std::vector<Token>& tokens;
    int current_token_index;
};
