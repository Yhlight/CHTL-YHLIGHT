#pragma once

#include "CHTL/CHTLLexer/Token.h"
#include "CHTL/CHTLNode/ASTNode.h"
#include <vector>
#include <memory>

namespace CHTL {

class ElementNode;
class StyleNode;

class Parser {
public:
    Parser(const std::vector<Token>& tokens, std::string_view source);
    std::unique_ptr<ASTNode> parse();

private:
    const std::vector<Token>& m_tokens;
    std::string_view m_source;
    int m_current = 0;

    const Token& peek();
    const Token& previous();
    bool isAtEnd();
    const Token& advance();
    bool check(TokenType type);
    const Token& consume(TokenType type, const std::string& message);

    std::unique_ptr<ASTNode> parseStatement();
    std::unique_ptr<ASTNode> parseText();
    std::unique_ptr<ASTNode> parseElement();
    void parseAttributes(ElementNode& element);
    std::unique_ptr<ASTNode> parseStyle();
    void parseStyleProperties(StyleNode& styleNode);
    std::unique_ptr<ASTNode> parseTemplate();
    std::unique_ptr<ASTNode> parseCustomTemplate();
    std::unique_ptr<ASTNode> parseOrigin();
};

} // namespace CHTL
