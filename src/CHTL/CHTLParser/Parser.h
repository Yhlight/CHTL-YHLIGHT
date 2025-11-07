#pragma once

#include "../CHTLLexer/Lexer.h"
#include "../CHTLNode/ProgramNode.h"
#include "../CHTLNode/ElementNode.h"
#include <memory>

namespace CHTL {

class TextNode; // Forward declaration
class StyleNode;
class StylePropertyNode;
class StyleRuleNode;
class ScriptNode;
class OriginNode;
class TemplateNode;
class TemplateUsageNode;
class DeleteNode;
class InsertNode;

class Parser {
public:
    Parser(Lexer& lexer);

    std::unique_ptr<ProgramNode> parse();

private:
    std::unique_ptr<BaseNode> parseStatement();
    std::unique_ptr<ElementNode> parseElement();
    std::unique_ptr<TextNode> parseTextNode();
    std::unique_ptr<StyleNode> parseStyle();
    std::unique_ptr<StyleRuleNode> parseStyleRule();
    std::unique_ptr<ScriptNode> parseScriptNode();
    std::unique_ptr<OriginNode> parseOriginNode();
    std::unique_ptr<TemplateNode> parseTemplateNode();
    std::unique_ptr<TemplateUsageNode> parseTemplateUsageNode();
    std::unique_ptr<DeleteNode> parseDeleteNode();
    std::unique_ptr<InsertNode> parseInsertNode();
    std::vector<std::unique_ptr<StylePropertyNode>> parseStyleProperties();


    Lexer* lexer;
    Token currentToken;

    void consume(TokenType expectedType);
};

} // namespace CHTL
