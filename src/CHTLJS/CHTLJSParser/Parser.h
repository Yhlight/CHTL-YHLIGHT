#pragma once

#include "CHTLJSLexer/Lexer.h"
#include "CHTLJSNode/ExprNode.h"
#include "CHTLJSNode/ProgramNode.h"
#include <memory>
#include <map>

namespace CHTLJS {

enum class Precedence {
    NONE,
    ASSIGNMENT,  // =
    OR,          // or
    AND,         // and
    EQUALITY,    // == !=
    COMPARISON,  // < > <= >=
    TERM,        // + -
    FACTOR,      // * /
    UNARY,       // ! -
    CALL,        // ()
    MEMBER,      // . ->
    PRIMARY
};

class Parser; // Forward declaration

using ParseFn = std::unique_ptr<ExprNode> (Parser::*)();
using InfixParseFn = std::unique_ptr<ExprNode> (Parser::*)(std::unique_ptr<ExprNode>);


struct ParseRule {
    ParseFn prefix;
    InfixParseFn infix;
    Precedence precedence;
};

class Parser {
public:
    explicit Parser(Lexer& lexer);

    std::unique_ptr<ProgramNode> parse();

private:
    void advance();
    void consume(TokenType type, const std::string& message);
    bool match(TokenType type);
    bool check(TokenType type) const;

    std::unique_ptr<ExprNode> parsePrecedence(Precedence precedence);
    ParseRule& getRule(TokenType type);

    std::unique_ptr<ExprNode> literal();
    std::unique_ptr<ExprNode> grouping();
    std::unique_ptr<ExprNode> selector();
    std::unique_ptr<ExprNode> binary(std::unique_ptr<ExprNode> left);
    std::unique_ptr<ExprNode> unary();
    std::unique_ptr<ExprNode> memberAccess(std::unique_ptr<ExprNode> object);
    std::unique_ptr<ExprNode> call(std::unique_ptr<ExprNode> callee);

    Lexer& lexer_;
    Token current_;
    Token previous_;
    bool hadError_ = false;

    std::map<TokenType, ParseRule> rules_;
};

}
