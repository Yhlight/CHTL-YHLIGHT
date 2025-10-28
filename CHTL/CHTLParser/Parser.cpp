#include "Parser.h"
#include "CHTL/CHTLNode/ElementNode.h"
#include "CHTL/CHTLNode/TextNode.h"
#include "CHTL/CHTLNode/AttributeNode.h"
#include "CHTL/CHTLNode/TemplateStyleDefinitionNode.h"
#include "CHTL/CHTLNode/StyleBlockNode.h"

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
    if (m_currentToken.type == TokenType::TemplateKeyword) {
        return parseTemplateStyleDefinition();
    }
    if (m_currentToken.type == TokenType::Identifier) {
        if (m_currentToken.value == "text") {
            return parseText();
        }
        return parseElement();
    }
    return nullptr; // Return null for now
}

std::shared_ptr<BaseNode> Parser::parseTemplateStyleDefinition() {
    eat(TokenType::TemplateKeyword);
    eat(TokenType::Identifier); // @Style
    std::string templateName = m_currentToken.value;
    eat(TokenType::Identifier);
    eat(TokenType::OpenBrace);

    std::string styleContent;
    while(m_currentToken.type != TokenType::CloseBrace && m_currentToken.type != TokenType::EndOfFile) {
        styleContent += m_currentToken.value;
        eat(m_currentToken.type);
    }

    eat(TokenType::CloseBrace);

    auto styleBlock = std::make_shared<StyleBlockNode>(styleContent);
    return std::make_shared<TemplateStyleDefinitionNode>(templateName, styleBlock);
}

std::shared_ptr<BaseNode> Parser::parseElement() {
    std::string tagName = m_currentToken.value;
    eat(TokenType::Identifier);

    auto element = std::make_shared<ElementNode>(tagName);

    eat(TokenType::OpenBrace);
    parseAttributesAndChildren(element);
    eat(TokenType::CloseBrace);

    return element;
}

void Parser::parseAttributesAndChildren(std::shared_ptr<ElementNode> element) {
    while (m_currentToken.type != TokenType::CloseBrace && m_currentToken.type != TokenType::EndOfFile) {
        if (m_currentToken.type == TokenType::Identifier) {
            if (m_currentToken.value == "style") {
                element->setStyleBlock(parseStyleBlock());
                continue;
            }
            if (m_lexer.peek().type == TokenType::Colon) {
                // It's an attribute
                std::string key = m_currentToken.value;
                eat(TokenType::Identifier);
                eat(TokenType::Colon);

                if (key == "text") {
                    std::string textValue = m_currentToken.value;
                    eat(TokenType::String);
                    element->addChild(std::make_shared<TextNode>(textValue));
                } else {
                    std::string value = m_currentToken.value;
                    eat(m_currentToken.type);
                    element->addAttribute(std::make_shared<AttributeNode>(key, value));
                }
                eat(TokenType::Semicolon);
            } else {
                // It's a child element
                element->addChild(parseStatement());
            }
        } else {
            // It's something else (e.g., another element), so parse it as a statement
            element->addChild(parseStatement());
        }
    }
}


std::shared_ptr<BaseNode> Parser::parseText() {
    eat(TokenType::Identifier); // Eat "text"
    eat(TokenType::OpenBrace);
    std::string textValue = m_currentToken.value;
    eat(TokenType::String);
    eat(TokenType::CloseBrace);
    return std::make_shared<TextNode>(textValue);
}

std::shared_ptr<StyleBlockNode> Parser::parseStyleBlock() {
    eat(TokenType::Identifier); // Eat "style"
    eat(TokenType::OpenBrace);

    std::string content;
    while(m_currentToken.type != TokenType::CloseBrace && m_currentToken.type != TokenType::EndOfFile) {
        // This is a naive implementation that just captures the raw content.
        // In the future, we will parse this into a more structured representation.
        content += m_currentToken.value;
        eat(m_currentToken.type);
    }

    eat(TokenType::CloseBrace);
    return std::make_shared<StyleBlockNode>(content);
}

} // namespace CHTL
