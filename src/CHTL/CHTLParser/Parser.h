#pragma once

#include "CHTL/CHTLLexer/Token.h"
#include "CHTL/CHTLNode/ASTNode.h"
#include <vector>
#include <memory>

namespace CHTL {

class Parser {
public:
    explicit Parser(std::vector<Token> tokens);
    std::unique_ptr<ProgramNode> parse();

private:
    std::unique_ptr<ASTNode> parseStatement();
    std::unique_ptr<ElementNode> parseElementNode();
    std::unique_ptr<TextNode> parseTextNode();
    std::unique_ptr<StyleNode> parseStyleNode();
    void parseAttributes(ElementNode& node);


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

} // namespace CHTL
