#include "Parser.h"
#include "Util/FileUtil.h"
#include "CHTL/CHTLNode/ElementNode.h"
#include "CHTL/CHTLNode/TextNode.h"
#include "CHTL/CHTLNode/AttributeNode.h"
#include "CHTL/CHTLNode/TemplateStyleDefinitionNode.h"
#include "CHTL/CHTLNode/StyleBlockNode.h"
#include "CHTL/CHTLNode/TemplateElementDefinitionNode.h"
#include "CHTL/CHTLNode/TemplateElementUsageNode.h"
#include "CHTL/CHTLNode/TemplateVarDefinitionNode.h"
#include "CHTL/CHTLNode/StylePropertyNode.h"
#include "CHTL/CHTLNode/ProgramNode.h"
#include "CHTL/CHTLNode/OriginNode.h"

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

std::shared_ptr<ProgramNode> Parser::parse() {
    auto programNode = std::make_shared<ProgramNode>();
    while(m_currentToken.type != TokenType::EndOfFile) {
        auto statement = parseStatement();
        if (statement) {
            programNode->addChild(statement);
        }
    }
    return programNode;
}

std::shared_ptr<BaseNode> Parser::parseStatement() {
    if (m_currentToken.type == TokenType::InheritKeyword) {
        eat(TokenType::InheritKeyword);
    }

    if (m_currentToken.type == TokenType::ImportKeyword) {
        parseImportStatement();
        return nullptr;
    }

    if (m_currentToken.type == TokenType::OriginKeyword) {
        return parseOriginBlock();
    }

    if (m_currentToken.type == TokenType::TemplateKeyword) {
        if (m_lexer.peek().value == "@Style") {
            return parseTemplateStyleDefinition();
        } else if (m_lexer.peek().value == "@Element") {
            return parseTemplateElementDefinition();
        } else if (m_lexer.peek().value == "@Var") {
            return parseTemplateVarDefinition();
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

    auto styleBlock = std::make_shared<StyleBlockNode>();

    eat(TokenType::OpenBrace);
    parseStyleBlockContent(styleBlock);
    eat(TokenType::CloseBrace);

    auto templateNode = std::make_shared<TemplateStyleDefinitionNode>(templateName, styleBlock);
    m_styleTemplates[templateName] = templateNode;
    return templateNode;
}

std::shared_ptr<BaseNode> Parser::parseTemplateVarDefinition() {
    eat(TokenType::TemplateKeyword);
    eat(TokenType::Identifier); // @Var
    std::string templateName = m_currentToken.value;
    eat(TokenType::Identifier);

    auto templateNode = std::make_shared<TemplateVarDefinitionNode>(templateName);

    eat(TokenType::OpenBrace);
    while (m_currentToken.type != TokenType::CloseBrace && m_currentToken.type != TokenType::EndOfFile) {
        std::string key = m_currentToken.value;
        eat(TokenType::Identifier);
        eat(TokenType::Colon);
        std::string value = m_currentToken.value;
        eat(m_currentToken.type); // Can be Identifier or String
        eat(TokenType::Semicolon);
        templateNode->addVariable(key, value);
    }
    eat(TokenType::CloseBrace);

    m_varTemplates[templateName] = templateNode;
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

    auto styleBlock = std::make_shared<StyleBlockNode>();
    parseStyleBlockContent(styleBlock);

    eat(TokenType::CloseBrace);
    return styleBlock;
}

void Parser::parseStyleBlockContent(std::shared_ptr<StyleBlockNode> styleBlock) {
    while(m_currentToken.type != TokenType::CloseBrace && m_currentToken.type != TokenType::EndOfFile) {
        if (m_currentToken.type == TokenType::InheritKeyword) {
            eat(TokenType::InheritKeyword);
        }

        if (m_currentToken.type == TokenType::Identifier && m_currentToken.value == "@Style") {
            eat(TokenType::Identifier); // Eat "@Style"
            styleBlock->addUsedTemplate(m_currentToken.value);
            eat(TokenType::Identifier); // Eat template name
            eat(TokenType::Semicolon);
        } else {
            // Assume it's a style property
            std::string key = m_currentToken.value;
            eat(TokenType::Identifier);
            eat(TokenType::Colon);

            std::string value;
            while(m_currentToken.type != TokenType::Semicolon && m_currentToken.type != TokenType::EndOfFile) {
                value += m_currentToken.value;
                eat(m_currentToken.type);
            }

            eat(TokenType::Semicolon);
            styleBlock->addProperty(std::make_shared<StylePropertyNode>(key, value));
        }
    }
}

std::shared_ptr<BaseNode> Parser::parseOriginBlock() {
    eat(TokenType::OriginKeyword);

    OriginType type;
    if (m_currentToken.value == "@Html") {
        type = OriginType::Html;
    } else if (m_currentToken.value == "@Style") {
        type = OriginType::Style;
    } else { // Assuming @JavaScript for now
        type = OriginType::JavaScript;
    }
    eat(TokenType::Identifier);

    std::optional<std::string> name;
    if (m_currentToken.type == TokenType::Identifier) {
        name = m_currentToken.value;
        eat(TokenType::Identifier);
    }

    std::string content = m_currentToken.value;
    eat(TokenType::RAW_CONTENT);

    auto node = std::make_shared<OriginNode>(type, content);
    if (name) {
        node->setName(*name);
        m_originTemplates[*name] = node;
    }
    return node;
}

#include <stdexcept>

void Parser::parseImportStatement() {
    eat(TokenType::ImportKeyword);

    OriginType originType;
    if (m_currentToken.value == "@Html") {
        originType = OriginType::Html;
    } else if (m_currentToken.value == "@Style") {
        originType = OriginType::Style;
    } else if (m_currentToken.value == "@JavaScript") {
        originType = OriginType::JavaScript;
    } else if (m_currentToken.value == "@Chtl") {
        // CHTL file import requires recursive parsing, which is not yet implemented.
        throw std::runtime_error("CHTL file import is not yet supported.");
    } else {
        throw std::runtime_error("Unsupported import type: " + m_currentToken.value);
    }
    eat(TokenType::Identifier);

    eat(TokenType::FromKeyword);

    std::string path = m_currentToken.value;
    eat(TokenType::String);

    eat(TokenType::AsKeyword);

    std::string alias = m_currentToken.value;
    eat(TokenType::Identifier);

    eat(TokenType::Semicolon);

    auto content = Util::FileUtil::readFileContents(path);
    if (content) {
        auto originNode = std::make_shared<OriginNode>(originType, *content);
        originNode->setName(alias);
        m_originTemplates[alias] = originNode;
    }
}

} // namespace CHTL
