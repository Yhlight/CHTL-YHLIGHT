#pragma once

#include "CHTL/Lexer.h"
#include "CHTL/CHTLNode/BaseNode.h"
#include "CHTL/CHTLNode/AttributeNode.h"
#include "CHTL/CHTLNode/StyleBlockNode.h"
#include "CHTL/CHTLNode/TemplateStyleDefinitionNode.h"
#include <memory>
#include <vector>
#include <map>

namespace CHTL {

class Parser {
public:
    Parser(Lexer& lexer);

    std::shared_ptr<BaseNode> parse();
    const std::map<std::string, std::shared_ptr<TemplateStyleDefinitionNode>>& getStyleTemplates() const { return m_styleTemplates; }

private:
    Lexer& m_lexer;
    Token m_currentToken;
    std::map<std::string, std::shared_ptr<class TemplateStyleDefinitionNode>> m_styleTemplates;

    void eat(TokenType type);
    std::shared_ptr<BaseNode> parseStatement();
    std::shared_ptr<BaseNode> parseElement();
    std::shared_ptr<BaseNode> parseText();
    std::shared_ptr<BaseNode> parseTemplateStyleDefinition();
    std::shared_ptr<StyleBlockNode> parseStyleBlock();
    void parseAttributesAndChildren(std::shared_ptr<class ElementNode> element);
};

} // namespace CHTL
