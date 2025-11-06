#pragma once

#include "../CHTLLexer/Lexer.h"
#include "../CHTLNode/ProgramNode.h"
#include "../CHTLNode/ElementNode.h"
#include <memory>

namespace CHTL {

class Parser {
public:
    Parser(Lexer& lexer);

    std::unique_ptr<ProgramNode> parse();

private:
    std::unique_ptr<BaseNode> parseStatement();
    std::unique_ptr<ElementNode> parseElement();

    Lexer& lexer;
    Token currentToken;

    void consume(TokenType expectedType);
};

} // namespace CHTL
