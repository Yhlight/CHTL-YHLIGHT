#pragma once

#include "CHTLLexer/Lexer.h"
#include "CHTLNode/ASTNode.h"
#include "CHTLNode/ProgramNode.h"
#include "CHTLNode/TemplateNode.h"
#include <memory>

class Parser {
public:
    explicit Parser(Lexer& lexer);

    std::unique_ptr<ProgramNode> parse();

private:
    void advance();
    bool match(TokenType type);
    void consume(TokenType type, const std::string& message);
    bool check(TokenType type) const;
    const Token& peek() const;

    std::unique_ptr<ASTNode> declaration();
    std::unique_ptr<ASTNode> statement();
    std::unique_ptr<ASTNode> parseText();
    std::unique_ptr<ASTNode> parseElement();
    std::unique_ptr<ASTNode> parseStyle();
    std::unique_ptr<ASTNode> parseStyleProperty();
    std::unique_ptr<ASTNode> parseTemplate();
    std::unique_ptr<ASTNode> parseTemplateUsage();
    std::unique_ptr<ASTNode> parseTemplateBody(TemplateType type);
    std::unique_ptr<ASTNode> parseImport();


    Lexer& lexer_;
    Token current_;
    Token previous_;
    bool hadError_ = false;
};
