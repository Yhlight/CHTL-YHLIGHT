#pragma once

#include "../CHTLLexer/Lexer.h"
#include "../CHTLNode/ProgramNode.h"
#include "../CHTLNode/ElementNode.h"
#include <memory>

namespace CHTL {

class TextNode; // Forward declaration
class StyleNode;
class StylePropertyNode;

class Parser {
public:
    Parser(Lexer& lexer);

    std::unique_ptr<ProgramNode> parse();

private:
    std::unique_ptr<BaseNode> parseStatement();
    std::unique_ptr<ElementNode> parseElement();
    std::unique_ptr<TextNode> parseTextNode();
    std::unique_ptr<StyleNode> parseStyle();
    std::vector<std::unique_ptr<StylePropertyNode>> parseStyleProperties();


    Lexer* lexer;
    Token currentToken;

    void consume(TokenType expectedType);
};

} // namespace CHTL
