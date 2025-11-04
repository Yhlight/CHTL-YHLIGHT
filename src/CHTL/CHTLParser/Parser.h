#pragma once

#include "../CHTLLexer/Lexer.h"
#include "../CHTLNode/ASTNode.h"
#include "../CHTLNode/ProgramNode.h"
#include <memory>

namespace CHTL {

// Forward declarations to break circular dependency
class ElementNode;
class TextNode;
class StyleNode;
class ScriptNode;

class Parser {
public:
    Parser(std::string_view source);
    std::unique_ptr<ProgramNode> parse();

private:
    void advance();
    void consume(TokenType type, const char* message);
    bool match(TokenType type);
    bool check(TokenType type) const;
    const Token& previous() const;
    const Token& peek() const;

    std::unique_ptr<ASTNode> parseStatement();
    std::unique_ptr<ElementNode> parseElement();
    void parseAttributes(ElementNode& element);
    std::unique_ptr<TextNode> parseText();
    std::unique_ptr<StyleNode> parseStyle();
    std::unique_ptr<ScriptNode> parseScript();

    std::string_view source_;
    Lexer lexer_;
    Token currentToken_;
    Token previousToken_;
};

} // namespace CHTL
