#include "Parser.h"
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
#include <fstream>
#include <stdexcept>

namespace CHTL {

Parser::Parser(Lexer& lexer) : m_lexer(lexer), m_symbolTable(std::make_shared<SymbolTable>()) {
    m_currentToken = m_lexer.nextToken();
}

Parser::Parser(Lexer& lexer, std::shared_ptr<SymbolTable> symbolTable) : m_lexer(lexer), m_symbolTable(symbolTable) {
    m_currentToken = m_lexer.nextToken();
}

void Parser::eat(TokenType type) {
    if (m_currentToken.type == type) {
        m_currentToken = m_lexer.nextToken();
    } else {
        throw std::runtime_error("Unexpected token type. Expected " + std::to_string((int)type) + " but got " + std::to_string((int)m_currentToken.type) + " with value " + m_currentToken.value);
    }
}

std::shared_ptr<ProgramNode> Parser::parse() {
    auto programNode = std::make_shared<ProgramNode>();
    while(m_currentToken.type != TokenType::EndOfFile) {
        programNode->addChild(parseStatement());
    }
    return programNode;
}

std::shared_ptr<BaseNode> Parser::parseStatement() {
    if (m_currentToken.type == TokenType::InheritKeyword) {
        eat(TokenType::InheritKeyword);
    }

    if (m_currentToken.type == TokenType::OriginKeyword) {
        return parseOriginBlock();
    }

    if (m_currentToken.type == TokenType::ImportKeyword) {
        return parseImportStatement();
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

    // Fallback for unrecognized tokens to prevent infinite loops
    if (m_currentToken.type != TokenType::EndOfFile) {
        eat(m_currentToken.type);
    }
    return nullptr;
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
    m_symbolTable->styleTemplates[templateName] = templateNode;
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

    m_symbolTable->varTemplates[templateName] = templateNode;
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

    m_symbolTable->elementTemplates[templateName] = templateNode;
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
        m_symbolTable->originTemplates[*name] = node;
    }
    return node;
}

std::shared_ptr<BaseNode> Parser::parseImportStatement() {
    eat(TokenType::ImportKeyword);

    ImportType importType = ImportType::Chtl;
    OriginType originType;
    bool isChtlImport = true;

    std::optional<ImportCategory> category;
    std::optional<ImportedSymbolType> symbolType;
    std::optional<std::string> symbolName;

    // Parse category: [Custom], [Template], [Origin]
    if (m_currentToken.type == TokenType::TemplateKeyword) {
        category = ImportCategory::Template;
        eat(TokenType::TemplateKeyword);
    } else if (m_currentToken.type == TokenType::Identifier && (m_currentToken.value == "[Custom]" || m_currentToken.value == "[Origin]")) {
        if (m_currentToken.value == "[Custom]") {
            category = ImportCategory::Custom;
        } else {
            category = ImportCategory::Origin;
        }
        eat(TokenType::Identifier);
    }

    bool hasCategory = category.has_value();

    // Parse symbol type: @Element, @Style, @Var, etc.
    if (m_currentToken.type == TokenType::Identifier) {
        const auto& value = m_currentToken.value;
        if (value == "@Element") {
            symbolType = ImportedSymbolType::Element;
            eat(TokenType::Identifier);
        } else if (value == "@Style") {
            if (hasCategory) {
                symbolType = ImportedSymbolType::Style;
            } else {
                isChtlImport = false;
                importType = ImportType::Style;
                originType = OriginType::Style;
            }
            eat(TokenType::Identifier);
        } else if (value == "@Var") {
            symbolType = ImportedSymbolType::Var;
            eat(TokenType::Identifier);
        } else if (value == "@Html") {
            isChtlImport = false;
            importType = ImportType::Html;
            originType = OriginType::Html;
            eat(TokenType::Identifier);
        } else if (value == "@JavaScript") {
            isChtlImport = false;
            importType = ImportType::JavaScript;
            originType = OriginType::JavaScript;
            eat(TokenType::Identifier);
        } else if (value == "@Chtl") {
            symbolType = ImportedSymbolType::Any;
            eat(TokenType::Identifier);
        }
    }

    // Parse symbol name if it exists
    if (m_currentToken.type == TokenType::Identifier && m_lexer.peek().value == "from") {
        symbolName = m_currentToken.value;
        eat(TokenType::Identifier);
    }

    eat(TokenType::Identifier); // "from"

    std::string filePath = m_currentToken.value;
    if (m_currentToken.type == TokenType::String) {
        eat(TokenType::String);
    } else {
        eat(TokenType::Identifier);
    }

    std::optional<std::string> asName;
    if (m_currentToken.type == TokenType::Identifier && m_currentToken.value == "as") {
        eat(TokenType::Identifier);
        asName = m_currentToken.value;
        eat(TokenType::Identifier);
    }

    if (!isChtlImport) {
        std::ifstream file(filePath);
        if (!file.is_open()) {
            throw std::runtime_error("Could not open file: " + filePath);
        }
        std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());

        if (asName) {
            auto originNode = std::make_shared<OriginNode>(originType, content);
            originNode->setName(*asName);
            m_symbolTable->originTemplates[*asName] = originNode;
        }
    } else {
        std::ifstream file(filePath);
        if (!file.is_open()) {
            throw std::runtime_error("Could not open file: " + filePath);
        }
        std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());

        Lexer importedLexer(content);
        if (symbolName || (category && !symbolName)) {
            auto tempSymbolTable = std::make_shared<SymbolTable>();
            Parser importedParser(importedLexer, tempSymbolTable);
            importedParser.parse();

            if (symbolName) {
                // Specific symbol import
                if (symbolType == ImportedSymbolType::Element) {
                    auto it = tempSymbolTable->elementTemplates.find(*symbolName);
                    if (it != tempSymbolTable->elementTemplates.end()) {
                        m_symbolTable->elementTemplates[*symbolName] = it->second;
                    }
                } else if (symbolType == ImportedSymbolType::Style) {
                    auto it = tempSymbolTable->styleTemplates.find(*symbolName);
                    if (it != tempSymbolTable->styleTemplates.end()) {
                        m_symbolTable->styleTemplates[*symbolName] = it->second;
                    }
                } else if (symbolType == ImportedSymbolType::Var) {
                    auto it = tempSymbolTable->varTemplates.find(*symbolName);
                    if (it != tempSymbolTable->varTemplates.end()) {
                        m_symbolTable->varTemplates[*symbolName] = it->second;
                    }
                }
            } else if (category) {
                // Category-based import
                if (*category == ImportCategory::Template) {
                    m_symbolTable->elementTemplates.insert(tempSymbolTable->elementTemplates.begin(), tempSymbolTable->elementTemplates.end());
                    m_symbolTable->styleTemplates.insert(tempSymbolTable->styleTemplates.begin(), tempSymbolTable->styleTemplates.end());
                    m_symbolTable->varTemplates.insert(tempSymbolTable->varTemplates.begin(), tempSymbolTable->varTemplates.end());
                }
            }
        } else {
            Parser importedParser(importedLexer, m_symbolTable);
            importedParser.parse();
        }
    }

    return std::make_shared<ImportNode>(importType, filePath, asName, category, symbolType, symbolName);
}

} // namespace CHTL
