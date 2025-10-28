#pragma once

#include "CHTL/Lexer.h"
#include "CHTL/CHTLNode/BaseNode.h"
#include "CHTL/CHTLNode/AttributeNode.h"
#include "CHTL/CHTLNode/StyleBlockNode.h"
#include "CHTL/CHTLNode/TemplateStyleDefinitionNode.h"
#include "CHTL/CHTLNode/TemplateElementDefinitionNode.h"
#include "CHTL/CHTLNode/TemplateVarDefinitionNode.h"
#include "CHTL/CHTLNode/ProgramNode.h"
#include <memory>
#include <vector>
#include <map>

namespace CHTL {

class Parser {
public:
    Parser(Lexer& lexer);

    std::shared_ptr<ProgramNode> parse();
    const std::map<std::string, std::shared_ptr<TemplateStyleDefinitionNode>>& getStyleTemplates() const { return m_styleTemplates; }
    const std::map<std::string, std::shared_ptr<TemplateElementDefinitionNode>>& getElementTemplates() const { return m_elementTemplates; }
    const std::map<std::string, std::shared_ptr<TemplateVarDefinitionNode>>& getVarTemplates() const { return m_varTemplates; }

private:
    Lexer& m_lexer;
    Token m_currentToken;
    std::map<std::string, std::shared_ptr<TemplateStyleDefinitionNode>> m_styleTemplates;
    std::map<std::string, std::shared_ptr<TemplateElementDefinitionNode>> m_elementTemplates;
    std::map<std::string, std::shared_ptr<TemplateVarDefinitionNode>> m_varTemplates;

    void eat(TokenType type);
    std::shared_ptr<BaseNode> parseStatement();
    std::shared_ptr<BaseNode> parseElement();
    std::shared_ptr<BaseNode> parseText();
    std::shared_ptr<BaseNode> parseTemplateStyleDefinition();
    std::shared_ptr<BaseNode> parseTemplateElementDefinition();
    std::shared_ptr<BaseNode> parseTemplateVarDefinition();
    std::shared_ptr<BaseNode> parseTemplateElementUsage();
    std::shared_ptr<StyleBlockNode> parseStyleBlock();
    void parseAttributesAndChildren(std::shared_ptr<class ElementNode> element);
    void parseStyleBlockContent(std::shared_ptr<StyleBlockNode> styleBlock);
};

} // namespace CHTL
