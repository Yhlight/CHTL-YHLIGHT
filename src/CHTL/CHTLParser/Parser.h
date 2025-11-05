#pragma once

#include "Token.h"
#include "BaseNode.h"
#include "ElementNode.h"
#include "TextNode.h"
#include "StyleNode.h"
#include "StylePropertyNode.h"
#include "StyleRuleNode.h"
#include "ScriptNode.h"
#include "TemplateNode.h"
#include "CustomNode.h"
#include "ElementDirectiveNode.h"
#include "StyleDirectiveNode.h"
#include "ProgramNode.h"
#include "OriginNode.h"
#include "OriginDirectiveNode.h"
#include "ImportNode.h"
#include <vector>
#include <memory>
#include <string>

class Parser {
public:
    Parser(const std::string& source, const std::vector<Token>& tokens);
    std::unique_ptr<ProgramNode> parse();

private:
    std::unique_ptr<BaseNode> parse_statement();
    std::unique_ptr<ElementNode> parse_element();
    std::unique_ptr<StyleNode> parse_style();
    std::unique_ptr<ScriptNode> parse_script();
    std::unique_ptr<TemplateNode> parse_template();
    std::unique_ptr<CustomNode> parse_custom();
    std::unique_ptr<BaseNode> parse_origin();
    std::unique_ptr<ImportNode> parse_import();
    std::unique_ptr<ElementDirectiveNode> parse_element_directive();
    std::unique_ptr<StyleDirectiveNode> parse_style_directive();
    Token current_token();
    void advance();

    const std::string& source;
    const std::vector<Token>& tokens;
    int current_token_index;
};
