#pragma once

#include "CHTLLexer/Lexer.h"
#include "CHTLNode/ASTNode.h"
#include "CHTLNode/ProgramNode.h"
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

    Lexer& lexer_;
    Token current_;
    Token previous_;
    bool hadError_ = false;
};
