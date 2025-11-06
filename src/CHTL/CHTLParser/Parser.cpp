#include "Parser.h"
#include "CHTLNode/TextNode.h"
#include "CHTLNode/StyleNode.h"
#include "CHTLNode/StylePropertyNode.h"
#include "CHTLNode/StyleRuleNode.h"
#include "CHTLNode/ScriptNode.h"
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
        std::string value = currentToken.value;
        consume(currentToken.type); // String or Identifier
        consume(TokenType::Semicolon);
        properties.push_back(std::make_unique<StylePropertyNode>(key, value));
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
        Token peekToken = lexer->peek();

        if (peekToken.type == TokenType::OpenBrace || currentToken.type == TokenType::Ampersand) {
            styleNode->children.push_back(parseStyleRule());
        } else if (peekToken.type == TokenType::Colon) {
            // It's a style property
            std::string key = currentToken.value;
            consume(TokenType::Identifier);
            consume(TokenType::Colon);
            std::string value = currentToken.value;
            consume(currentToken.type);
            consume(TokenType::Semicolon);
            styleNode->children.push_back(std::make_unique<StylePropertyNode>(key, value));
        } else {
            // Error or unexpected token
            consume(currentToken.type);
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


std::unique_ptr<ElementNode> Parser::parseElement() {
    std::string tagName = currentToken.value;
    consume(TokenType::Identifier);

    auto elementNode = std::make_unique<ElementNode>(tagName);

    consume(TokenType::OpenBrace);

    while(currentToken.type != TokenType::CloseBrace && currentToken.type != TokenType::Eof) {
        // Here, we can have attributes or child statements (elements, text, style)
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
            } else {
                // Child Element, text node, or style node
                elementNode->children.push_back(parseStatement());
            }
        } else {
             // If we don't recognize the token, consume it and move on
            if (currentToken.type != TokenType::Eof) {
                currentToken = lexer->getNextToken();
            }
        }
    }

    consume(TokenType::CloseBrace);

    return elementNode;
}

} // namespace CHTL
