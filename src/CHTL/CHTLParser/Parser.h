#pragma once

#include "../CHTLLexer/Lexer.h"
#include "../CHTLNode/ProgramNode.h"
#include "../CHTLNode/ElementNode.h"
#include "../CHTLNode/ValueNode.h"
#include "../CHTLNode/ComparisonNode.h"
#include "../CHTLNode/LogicalNode.h"
#include "../CHTLNode/ConditionalNode.h"
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
class ElementDeleteNode;

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
    std::unique_ptr<ElementDeleteNode> parseElementDeleteNode();
    std::unique_ptr<ElementNode> parseElementReference();
    std::unique_ptr<TemplateUsageNode> parseTemplateUsageReference();
    std::vector<std::unique_ptr<StylePropertyNode>> parseStyleProperties();
    std::unique_ptr<ValueNode> parseExpression();
    std::unique_ptr<ValueNode> parseConditionalExpression();
    std::unique_ptr<ValueNode> parseLogicalOrExpression();
    std::unique_ptr<ValueNode> parseLogicalAndExpression();
    std::unique_ptr<ValueNode> parseComparisonExpression();
    std::unique_ptr<ValueNode> parseAdditiveExpression();
    std::unique_ptr<ValueNode> parseTerm();
    std::unique_ptr<ValueNode> parseFactor();
    std::unique_ptr<ValueNode> parsePower();
    std::unique_ptr<ValueNode> parseAtom();


    Lexer* lexer;
    Token currentToken;

    void consume(TokenType expectedType);
};

} // namespace CHTL
