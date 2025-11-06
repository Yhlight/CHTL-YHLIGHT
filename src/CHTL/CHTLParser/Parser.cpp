#include "Parser.h"
#include "CHTLNode/TextNode.h"
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

std::unique_ptr<ElementNode> Parser::parseElement() {
    std::string tagName = currentToken.value;
    consume(TokenType::Identifier);

    auto elementNode = std::make_unique<ElementNode>(tagName);

    consume(TokenType::OpenBrace);

    while(currentToken.type != TokenType::CloseBrace && currentToken.type != TokenType::Eof) {
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
                // Child Element or text node
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
