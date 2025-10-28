#pragma once

#include "../CHTLLexer/Token.h"
#include "../CHTLNode/AST.h"
#include <vector>
#include <memory>

class Parser {
public:
    Parser(const std::vector<Token>& tokens);
    std::unique_ptr<ASTNode> parse();

private:
    std::vector<Token> tokens;
    size_t current = 0;

    Token peek();
    Token advance();
    bool isAtEnd();

    std::unique_ptr<TextNode> parseTextNode();
};
