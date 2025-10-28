#pragma once

#include "CHTL/Lexer.h"
#include "CHTL/CHTLNode/BaseNode.h"
#include "CHTL/CHTLNode/AttributeNode.h"
#include "CHTL/CHTLNode/StyleBlockNode.h"
#include <memory>
#include <vector>

namespace CHTL {

class Parser {
public:
    Parser(Lexer& lexer);

    std::shared_ptr<BaseNode> parse();

private:
    Lexer& m_lexer;
    Token m_currentToken;

    void eat(TokenType type);
    std::shared_ptr<BaseNode> parseStatement();
    std::shared_ptr<BaseNode> parseElement();
    std::shared_ptr<BaseNode> parseText();
    std::shared_ptr<BaseNode> parseTemplateStyleDefinition();
    std::shared_ptr<StyleBlockNode> parseStyleBlock();
    void parseAttributesAndChildren(std::shared_ptr<class ElementNode> element);
};

} // namespace CHTL
