#include "Parser.h"
#include <stdexcept>

namespace CHTL {

Parser::Parser(Lexer& lexer) : lexer(lexer) {
    currentToken = lexer.getNextToken();
}

void Parser::consume(TokenType expectedType) {
    if (currentToken.type == expectedType) {
        currentToken = lexer.getNextToken();
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
        return parseElement();
    }
    // In the future, we will handle other statement types here.

    // If we don't recognize the token, consume it and move on
    // This part is problematic and will be fixed later. For now, it prevents infinite loops.
    if (currentToken.type != TokenType::Eof) {
        currentToken = lexer.getNextToken();
    }
    return nullptr;
}

std::unique_ptr<ElementNode> Parser::parseElement() {
    std::string tagName = currentToken.value;
    consume(TokenType::Identifier);

    auto elementNode = std::make_unique<ElementNode>(tagName);

    consume(TokenType::OpenBrace);

    while(currentToken.type != TokenType::CloseBrace && currentToken.type != TokenType::Eof) {
        auto child = parseStatement();
        if (child) {
            elementNode->children.push_back(std::move(child));
        }
    }

    consume(TokenType::CloseBrace);

    return elementNode;
}

} // namespace CHTL
