#include "Parser.h"
#include "CHTL/CHTLNode/ProgramNode.h"
#include "CHTL/CHTLNode/TextNode.h"
#include <stdexcept>

namespace CHTL {

Parser::Parser(const std::vector<Token>& tokens, std::string_view source) : m_tokens(tokens), m_source(source) {}

std::unique_ptr<ASTNode> Parser::parse() {
    auto programNode = std::make_unique<ProgramNode>();
    while (!isAtEnd()) {
        if (check(TokenType::Text)) {
            programNode->children.push_back(parseText());
        } else {
            // For now, just advance to avoid infinite loops on unknown tokens
            advance();
        }
    }
    return programNode;
}

const Token& Parser::peek() { return m_tokens[m_current]; }

const Token& Parser::previous() { return m_tokens[m_current - 1]; }

bool Parser::isAtEnd() { return peek().type == TokenType::Eof; }

const Token& Parser::advance() {
    if (!isAtEnd()) m_current++;
    return previous();
}

bool Parser::check(TokenType type) {
    if (isAtEnd()) return false;
    return peek().type == type;
}

const Token& Parser::consume(TokenType type, const std::string& message) {
    if (check(type)) return advance();
    throw std::runtime_error(message);
}

std::unique_ptr<ASTNode> Parser::parseText() {
    consume(TokenType::Text, "Expect 'text' keyword.");
    const Token& leftBrace = consume(TokenType::LeftBrace, "Expect '{' after 'text' keyword.");

    int startPos = leftBrace.start_pos + leftBrace.lexeme.length();

    int lookahead = m_current;
    while (m_tokens[lookahead].type != TokenType::RightBrace && m_tokens[lookahead].type != TokenType::Eof) {
        lookahead++;
    }

    if (m_tokens[lookahead].type == TokenType::Eof) {
        throw std::runtime_error("Unterminated text block.");
    }

    const Token& rightBrace = m_tokens[lookahead];
    int endPos = rightBrace.start_pos;

    auto textNode = std::make_unique<TextNode>();
    textNode->content = m_source.substr(startPos, endPos - startPos);

    m_current = lookahead;
    consume(TokenType::RightBrace, "Expect '}' after text block.");

    return textNode;
}

} // namespace CHTL
