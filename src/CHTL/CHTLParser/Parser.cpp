#include "Parser.h"
#include "CHTLNode/TextNode.h"
#include "CHTLNode/StyleNode.h"
#include "CHTLNode/StylePropertyNode.h"
#include "CHTLNode/StyleRuleNode.h"
#include "CHTLNode/ScriptNode.h"
#include "CHTLNode/OriginNode.h"
#include "CHTLNode/TemplateNode.h"
#include "CHTLNode/TemplateUsageNode.h"
#include "CHTLNode/ValueNode.h"
#include "CHTLNode/LiteralValueNode.h"
#include "CHTLNode/VariableUsageNode.h"
#include "CHTLNode/DeleteNode.h"
#include <stdexcept>

namespace CHTL {

Parser::Parser(Lexer& lexer) : lexer(&lexer) {
    currentToken = this->lexer->getNextToken();
}

void Parser::consume(TokenType expectedType) {
    if (currentToken.type == expectedType) {
        currentToken = lexer->getNextToken();
    } else {
        // Simple error handling for now
        throw std::runtime_error("Unexpected token: expected " + std::to_string((int)expectedType) +
                                 " but got " + std::to_string((int)currentToken.type));
    }
}

std::unique_ptr<ProgramNode> Parser::parse() {
    auto programNode = std::make_unique<ProgramNode>();

    while (currentToken.type != TokenType::Eof) {
        auto statement = parseStatement();
        if(statement) {
            programNode->statements.push_back(std::move(statement));
        }
    }

    return programNode;
}

std::unique_ptr<BaseNode> Parser::parseStatement() {
    if (currentToken.type == TokenType::Identifier) {
        if (currentToken.value == "text") {
            return parseTextNode();
        } else if (currentToken.value == "style") {
            return parseStyle();
        } else if (currentToken.value == "script") {
            return parseScriptNode();
        }
        return parseElement();
    }

    if (currentToken.type == TokenType::OpenBracket) {
        Token peekToken = lexer->peek();
        if (peekToken.value == "Template" || peekToken.value == "Custom") {
            return parseTemplateNode();
        }
        return parseOriginNode();
    }

    if (currentToken.type == TokenType::At) {
        return parseTemplateUsageNode();
    }

    // If we don't recognize the token, consume it and move on
    if (currentToken.type != TokenType::Eof) {
        currentToken = lexer->getNextToken();
    }
    return nullptr;
}

std::unique_ptr<TextNode> Parser::parseTextNode() {
    consume(TokenType::Identifier); // Consume "text"
    consume(TokenType::OpenBrace);

    std::string content = currentToken.value;
    consume(TokenType::String);

    consume(TokenType::CloseBrace);

    return std::make_unique<TextNode>(content);
}

std::vector<std::unique_ptr<StylePropertyNode>> Parser::parseStyleProperties() {
    auto properties = std::vector<std::unique_ptr<StylePropertyNode>>();

    while (currentToken.type == TokenType::Identifier) {
        std::string key = currentToken.value;
        consume(TokenType::Identifier);
        consume(TokenType::Colon);

        auto values = std::vector<std::unique_ptr<ValueNode>>();
        while (currentToken.type != TokenType::Semicolon && currentToken.type != TokenType::Eof) {
            if (currentToken.type == TokenType::Identifier) {
                Token peekToken = lexer->peek();
                if (peekToken.type == TokenType::OpenParen) {
                    // Variable usage
                    std::string groupName = currentToken.value;
                    consume(TokenType::Identifier);
                    consume(TokenType::OpenParen);
                    std::string variableName = currentToken.value;
                    consume(TokenType::Identifier);
                    consume(TokenType::CloseParen);
                    values.push_back(std::make_unique<VariableUsageNode>(groupName, variableName));
                } else {
                    // Literal value
                    values.push_back(std::make_unique<LiteralValueNode>(currentToken.value));
                    consume(TokenType::Identifier);
                }
            } else if (currentToken.type == TokenType::String) {
                values.push_back(std::make_unique<LiteralValueNode>(currentToken.value));
                consume(TokenType::String);
            } else {
                // Error or unexpected token
                consume(currentToken.type);
            }
        }

        consume(TokenType::Semicolon);
        properties.push_back(std::make_unique<StylePropertyNode>(key, std::move(values)));
    }

    return properties;
}

std::unique_ptr<StyleRuleNode> Parser::parseStyleRule() {
    std::string selector;
    if (currentToken.type == TokenType::Ampersand) {
        selector += currentToken.value;
        consume(TokenType::Ampersand);
        // Handle pseudo-classes like :hover
        while (currentToken.type == TokenType::Colon) {
            selector += currentToken.value;
            consume(TokenType::Colon);
            selector += currentToken.value;
            consume(TokenType::Identifier);
        }
    } else {
        selector = currentToken.value;
        consume(TokenType::Identifier);
    }

    consume(TokenType::OpenBrace);

    auto ruleNode = std::make_unique<StyleRuleNode>(selector);
    ruleNode->properties = parseStyleProperties();

    consume(TokenType::CloseBrace);
    return ruleNode;
}

std::unique_ptr<StyleNode> Parser::parseStyle() {
    consume(TokenType::Identifier); // Consume "style"
    consume(TokenType::OpenBrace);

    auto styleNode = std::make_unique<StyleNode>();

    while (currentToken.type != TokenType::CloseBrace && currentToken.type != TokenType::Eof) {
        if (currentToken.type == TokenType::At) {
            styleNode->children.push_back(parseTemplateUsageNode());
        } else {
            Token peekToken = lexer->peek();

            if (peekToken.type == TokenType::OpenBrace || currentToken.type == TokenType::Ampersand) {
                styleNode->children.push_back(parseStyleRule());
            } else if (peekToken.type == TokenType::Colon) {
                // It's a style property
                auto properties = parseStyleProperties();
                for (auto& prop : properties) {
                    styleNode->children.push_back(std::move(prop));
                }
            } else {
                // Error or unexpected token
                consume(currentToken.type);
            }
        }
    }

    consume(TokenType::CloseBrace);
    return styleNode;
}

std::unique_ptr<ScriptNode> Parser::parseScriptNode() {
    consume(TokenType::Identifier); // Consume "script"
    consume(TokenType::OpenBrace);

    std::string content = currentToken.value;
    consume(TokenType::String);

    consume(TokenType::CloseBrace);

    return std::make_unique<ScriptNode>(content);
}

std::unique_ptr<OriginNode> Parser::parseOriginNode() {
    consume(TokenType::OpenBracket);
    consume(TokenType::Identifier); // Consume "Origin"
    consume(TokenType::CloseBracket);

    consume(TokenType::At);
    std::string type = currentToken.value;
    consume(TokenType::Identifier);

    consume(TokenType::OpenBrace);

    std::string content = currentToken.value;
    consume(TokenType::String);

    consume(TokenType::CloseBrace);

    return std::make_unique<OriginNode>(type, content);
}


std::unique_ptr<ElementNode> Parser::parseElement() {
    std::string tagName = currentToken.value;
    consume(TokenType::Identifier);

    auto elementNode = std::make_unique<ElementNode>(tagName);

    consume(TokenType::OpenBrace);

    while(currentToken.type != TokenType::CloseBrace && currentToken.type != TokenType::Eof) {
        // Check for attribute first
        if (currentToken.type == TokenType::Identifier) {
            Token peekToken = lexer->peek();
            if (peekToken.type == TokenType::Colon || peekToken.type == TokenType::Equal) {
                // Attribute
                std::string key = currentToken.value;
                consume(TokenType::Identifier);
                consume(peekToken.type); // consume ':' or '='

                std::string value;
                if (currentToken.type == TokenType::String || currentToken.type == TokenType::Identifier) {
                    value = currentToken.value;
                    consume(currentToken.type);
                } else {
                    throw std::runtime_error("Expected string or identifier for attribute value");
                }
                elementNode->attributes[key] = value;

                if (currentToken.type == TokenType::Semicolon) {
                    consume(TokenType::Semicolon);
                }
                continue;
            }
        }

        // If not an attribute, parse as a statement
        auto statement = parseStatement();
        if (statement) {
            elementNode->children.push_back(std::move(statement));
        }
    }

    consume(TokenType::CloseBrace);

    return elementNode;
}

std::unique_ptr<TemplateNode> Parser::parseTemplateNode() {
    consume(TokenType::OpenBracket);
    bool isCustom = currentToken.value == "Custom";
    consume(TokenType::Identifier); // Consume "Template" or "Custom"
    consume(TokenType::CloseBracket);

    consume(TokenType::At);
    std::string type = currentToken.value;
    consume(TokenType::Identifier);

    std::string name = currentToken.value;
    consume(TokenType::Identifier);

    consume(TokenType::OpenBrace);

    auto templateNode = std::make_unique<TemplateNode>();
    templateNode->type = type;
    templateNode->name = name;
    templateNode->isCustom = isCustom;

    if (type == "Style") {
        while (currentToken.type != TokenType::CloseBrace && currentToken.type != TokenType::Eof) {
            if (currentToken.type == TokenType::At) {
                templateNode->inheritances.push_back(parseTemplateUsageNode());
            } else if (currentToken.type == TokenType::Delete) {
                templateNode->body.push_back(parseDeleteNode());
            } else {
                // Check for valueless properties
                Token peekToken = lexer->peek();
                if (isCustom && (peekToken.type == TokenType::Comma || peekToken.type == TokenType::Semicolon)) {
                    // Valueless properties
                    while (currentToken.type == TokenType::Identifier) {
                        std::string key = currentToken.value;
                        consume(TokenType::Identifier);
                        auto values = std::vector<std::unique_ptr<ValueNode>>();
                        templateNode->body.push_back(std::make_unique<StylePropertyNode>(key, std::move(values)));

                        if (currentToken.type == TokenType::Comma) {
                            consume(TokenType::Comma);
                        } else {
                            break;
                        }
                    }
                    if (currentToken.type == TokenType::Semicolon) {
                        consume(TokenType::Semicolon);
                    }
                } else {
                    auto properties = parseStyleProperties();
                    for (auto& prop : properties) {
                        templateNode->body.push_back(std::move(prop));
                    }
                }
            }
        }
    } else if (type == "Element") {
        while (currentToken.type != TokenType::CloseBrace && currentToken.type != TokenType::Eof) {
            templateNode->body.push_back(parseStatement());
        }
    } else if (type == "Var") {
        auto properties = parseStyleProperties();
        for (auto& prop : properties) {
            templateNode->body.push_back(std::move(prop));
        }
    }

    consume(TokenType::CloseBrace);

    return templateNode;
}

std::unique_ptr<TemplateUsageNode> Parser::parseTemplateUsageNode() {
    consume(TokenType::At);
    std::string type = currentToken.value;
    consume(TokenType::Identifier);

    std::string name = currentToken.value;
    consume(TokenType::Identifier);

    auto node = std::make_unique<TemplateUsageNode>(type, name);

    if (currentToken.type == TokenType::OpenBrace) {
        consume(TokenType::OpenBrace);
        if (type == "Style") {
            node->provided_properties = parseStyleProperties();
        } else if (type == "Element") {
            while (currentToken.type != TokenType::CloseBrace && currentToken.type != TokenType::Eof) {
                node->body.push_back(parseStatement());
            }
        }
        consume(TokenType::CloseBrace);
    } else {
        consume(TokenType::Semicolon);
    }

    return node;
}

std::unique_ptr<DeleteNode> Parser::parseDeleteNode() {
    consume(TokenType::Delete);

    auto deleteNode = std::make_unique<DeleteNode>();

    while (currentToken.type != TokenType::Semicolon && currentToken.type != TokenType::Eof) {
        if (currentToken.type == TokenType::At) {
            consume(TokenType::At);
            consume(TokenType::Identifier); // Consume "Style"
            deleteNode->inheritances.push_back(currentToken.value);
            consume(TokenType::Identifier);
        } else {
            deleteNode->properties.push_back(currentToken.value);
            consume(TokenType::Identifier);
        }
    }

    consume(TokenType::Semicolon);

    return deleteNode;
}

} // namespace CHTL
