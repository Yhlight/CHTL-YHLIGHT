#pragma once

#include "CHTLLexer/Token.h"
#include "CHTLNode/BaseNode.h"
#include <vector>

class Parser {
public:
    explicit Parser(std::vector<Token> tokens);
    AstNodeList parse();

private:
    std::vector<Token> tokens;
    size_t current = 0;

    AstNode parseStatement();
    AstNode parseElement();
    AstNode parseStyleBlock();
    AstNode parseScriptBlock();
    AstNode parseComment();
    AstNode parseTemplate();
    AstNode parseCustom();
    AstNode parseOrigin();
    AstNode parseImport();
    AstNode parseNamespace();
    Token advance();
    Token peek();
    Token peekNext();
    bool isAtEnd();
};
