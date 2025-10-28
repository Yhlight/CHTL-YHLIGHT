#pragma once

#include <vector>
#include <memory>
#include "CHTL/CHTLLexer/Token.h"
#include "CHTL/CHTLNode/ASTNode.h"

namespace CHTL {

class Parser {
public:
    explicit Parser(std::vector<Token> tokens);
    std::unique_ptr<ProgramNode> parse();

private:
    std::unique_ptr<ASTNode> parseStatement();
    std::unique_ptr<ElementNode> parseElementNode();
    std::unique_ptr<TextNode> parseTextNode();
    std::unique_ptr<StyleNode> parseStyleNode(ElementNode* parent);
    std::unique_ptr<TemplateNode> parseTemplateNode();
    std::unique_ptr<TemplateNode> parseCustomNode();
    std::unique_ptr<TemplateUsageNode> parseTemplateUsage();
    void parseAttributes(ElementNode& node);

    std::unique_ptr<ASTNode> parseConditionalExpression();
    std::unique_ptr<ASTNode> parseExpression(int precedence = 0);
    std::unique_ptr<ASTNode> parsePrefix();
    std::unique_ptr<ASTNode> parseInfix(std::unique_ptr<ASTNode> left);
    int getPrecedence(TokenType type);

    const Token& advance();
    const Token& peek() const;
    const Token& previous() const;
    bool isAtEnd() const;
    bool check(TokenType type) const;
    bool match(const std::vector<TokenType>& types);
    const Token& consume(TokenType type, const std::string& message);

    std::vector<Token> m_tokens;
    size_t m_current = 0;
};

}
