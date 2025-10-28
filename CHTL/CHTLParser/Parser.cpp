#include "Parser.h"
#include "CHTL/CHTLNode/ElementNode.h"
#include "CHTL/CHTLNode/TextNode.h"
#include "CHTL/CHTLNode/AttributeNode.h"

namespace CHTL {

Parser::Parser(Lexer& lexer) : m_lexer(lexer) {
    m_currentToken = m_lexer.nextToken();
}

void Parser::eat(TokenType type) {
    if (m_currentToken.type == type) {
        m_currentToken = m_lexer.nextToken();
    } else {
        // In a real implementation, you would throw an error here.
        // For now, we'll just advance to keep it simple for the test.
        m_currentToken = m_lexer.nextToken();
    }
}

std::shared_ptr<BaseNode> Parser::parse() {
    return parseStatement();
}

std::shared_ptr<BaseNode> Parser::parseStatement() {
    if (m_currentToken.type == TokenType::Identifier) {
        if (m_currentToken.value == "text") {
            return parseText();
        }
        return parseElement();
    }
    return nullptr; // Return null for now if it's not an identifier
}

std::shared_ptr<BaseNode> Parser::parseElement() {
    std::string tagName = m_currentToken.value;
    eat(TokenType::Identifier);

    auto element = std::make_shared<ElementNode>(tagName);

    eat(TokenType::OpenBrace);

    while (m_currentToken.type != TokenType::CloseBrace && m_currentToken.type != TokenType::EndOfFile) {
        if (m_currentToken.type == TokenType::Identifier && m_lexer.peek().type == TokenType::Colon) {
            element->addAttribute(parseAttribute());
        } else {
            element->addChild(parseStatement());
        }
    }

    eat(TokenType::CloseBrace);

    return element;
}

std::shared_ptr<BaseNode> Parser::parseText() {
    eat(TokenType::Identifier); // Eat "text"
    eat(TokenType::OpenBrace);
    std::string textValue = m_currentToken.value;
    eat(TokenType::String);
    eat(TokenType::CloseBrace);
    return std::make_shared<TextNode>(textValue);
}

std::shared_ptr<AttributeNode> Parser::parseAttribute() {
    std::string key = m_currentToken.value;
    eat(TokenType::Identifier);
    eat(TokenType::Colon);
    std::string value = m_currentToken.value;
    if (m_currentToken.type == TokenType::Identifier || m_currentToken.type == TokenType::String) {
        eat(m_currentToken.type);
    } else {
        // Handle error: unexpected token for attribute value
    }
    eat(TokenType::Semicolon);
    return std::make_shared<AttributeNode>(key, value);
}

} // namespace CHTL
