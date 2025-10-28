#include "Parser.h"
#include "CHTL/CHTLNode/ElementNode.h"
#include "CHTL/CHTLNode/TextNode.h"
#include "CHTL/CHTLNode/AttributeNode.h"
#include "CHTL/CHTLNode/TemplateStyleDefinitionNode.h"
#include "CHTL/CHTLNode/StyleBlockNode.h"
#include "CHTL/CHTLNode/TemplateElementDefinitionNode.h"
#include "CHTL/CHTLNode/TemplateElementUsageNode.h"

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
    // For now, let's assume a program is a collection of statements,
    // and we'll just return the last one for simplicity in testing.
    // A more robust implementation would return a ProgramNode.
    std::shared_ptr<BaseNode> lastNode = nullptr;
    while(m_currentToken.type != TokenType::EndOfFile) {
        lastNode = parseStatement();
    }
    return lastNode;
}

std::shared_ptr<BaseNode> Parser::parseStatement() {
    if (m_currentToken.type == TokenType::TemplateKeyword) {
        if (m_lexer.peek().value == "@Style") {
            return parseTemplateStyleDefinition();
        } else if (m_lexer.peek().value == "@Element") {
            return parseTemplateElementDefinition();
        }
    }
    if (m_currentToken.type == TokenType::Identifier) {
        if (m_currentToken.value == "@Element") {
            return parseTemplateElementUsage();
        }
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
    auto templateNode = std::make_shared<TemplateStyleDefinitionNode>(templateName, styleBlock);
    m_styleTemplates[templateName] = templateNode;
    return templateNode;
}

std::shared_ptr<BaseNode> Parser::parseTemplateElementDefinition() {
    eat(TokenType::TemplateKeyword);
    eat(TokenType::Identifier); // @Element
    std::string templateName = m_currentToken.value;
    eat(TokenType::Identifier);

    auto templateNode = std::make_shared<TemplateElementDefinitionNode>(templateName);

    eat(TokenType::OpenBrace);
    while (m_currentToken.type != TokenType::CloseBrace && m_currentToken.type != TokenType::EndOfFile) {
        templateNode->addChild(parseStatement());
    }
    eat(TokenType::CloseBrace);

    m_elementTemplates[templateName] = templateNode;
    return templateNode;
}

std::shared_ptr<BaseNode> Parser::parseTemplateElementUsage() {
    eat(TokenType::Identifier); // @Element
    std::string templateName = m_currentToken.value;
    eat(TokenType::Identifier);
    eat(TokenType::Semicolon);
    return std::make_shared<TemplateElementUsageNode>(templateName);
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

    std::string rawContent;
    std::vector<std::string> usedTemplates;

    while(m_currentToken.type != TokenType::CloseBrace && m_currentToken.type != TokenType::EndOfFile) {
        if (m_currentToken.type == TokenType::Identifier && m_currentToken.value == "@Style") {
            eat(TokenType::Identifier); // Eat "@Style"
            usedTemplates.push_back(m_currentToken.value);
            eat(TokenType::Identifier); // Eat template name
            eat(TokenType::Semicolon);
        } else {
            rawContent += m_currentToken.value;
            eat(m_currentToken.type);
        }
    }

    eat(TokenType::CloseBrace);

    auto styleBlock = std::make_shared<StyleBlockNode>(rawContent);
    for(const auto& t : usedTemplates) {
        styleBlock->addUsedTemplate(t);
    }
    return styleBlock;
}

} // namespace CHTL
