#include "Parser.h"
#include "CHTL/CHTLNode/ProgramNode.h"
#include "CHTL/CHTLNode/TextNode.h"
#include "CHTL/CHTLNode/ElementNode.h"
#include "CHTL/CHTLNode/StyleNode.h"
#include "CHTL/CHTLNode/StylePropertyNode.h"
#include "CHTL/CHTLNode/TemplateNode.h"
#include "CHTL/CHTLNode/CustomTemplateNode.h"
#include "CHTL/CHTLNode/OriginNode.h"
#include "CHTL/CHTLNode/ImportNode.h"
#include <stdexcept>

namespace CHTL {

Parser::Parser(const std::vector<Token>& tokens, std::string_view source) : m_tokens(tokens), m_source(source) {}

std::unique_ptr<ASTNode> Parser::parse() {
    auto programNode = std::make_unique<ProgramNode>();
    while (!isAtEnd()) {
        try {
            if (auto statement = parseStatement()) {
                programNode->children.push_back(std::move(statement));
            }
        } catch (const std::runtime_error& e) {
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

std::unique_ptr<ASTNode> Parser::parseStatement() {
    if (check(TokenType::Text)) {
        return parseText();
    }
    if (check(TokenType::Identifier)) {
        return parseElement();
    }
    if (check(TokenType::Style)) {
        return parseStyle();
    }
    if (check(TokenType::BlockKeyword)) {
        if (peek().lexeme == "[Template]") {
            return parseTemplate();
        }
        if (peek().lexeme == "[Custom]") {
            return parseCustomTemplate();
        }
        if (peek().lexeme == "[Origin]") {
            return parseOrigin();
        }
        if (peek().lexeme == "[Import]") {
            return parseImport();
        }
    }
    // For now, just advance to avoid infinite loops on unknown tokens
    throw std::runtime_error("Unexpected token.");
}

std::unique_ptr<ASTNode> Parser::parseStyle() {
    consume(TokenType::Style, "Expect 'style' keyword.");
    consume(TokenType::LeftBrace, "Expect '{' after 'style' keyword.");

    auto styleNode = std::make_unique<StyleNode>();
    parseStyleProperties(*styleNode);

    consume(TokenType::RightBrace, "Expect '}' after style block.");

    return styleNode;
}

void Parser::parseStyleProperties(StyleNode& styleNode) {
    while (!check(TokenType::RightBrace) && !isAtEnd()) {
        auto propNode = std::make_unique<StylePropertyNode>();
        propNode->name = std::string(consume(TokenType::Identifier, "Expect property name.").lexeme);

        if (check(TokenType::Colon)) {
            consume(TokenType::Colon, "Expect ':' after property name.");

            std::string value = "";
            while (!check(TokenType::Semicolon) && !isAtEnd()) {
                value += advance().lexeme;
            }
            propNode->value = value;
        }

        consume(TokenType::Semicolon, "Expect ';' after property value.");
        styleNode.properties.push_back(std::move(propNode));
    }
}

std::unique_ptr<ASTNode> Parser::parseElement() {
    auto elementNode = std::make_unique<ElementNode>();
    elementNode->tagName = advance().lexeme;

    consume(TokenType::LeftBrace, "Expect '{' after element tag.");

    parseAttributes(*elementNode);

    while (!check(TokenType::RightBrace) && !isAtEnd()) {
        elementNode->children.push_back(parseStatement());
    }

    consume(TokenType::RightBrace, "Expect '}' after element block.");

    return elementNode;
}

void Parser::parseAttributes(ElementNode& element) {
    while (check(TokenType::Identifier) && (m_tokens[m_current + 1].type == TokenType::Colon || m_tokens[m_current + 1].type == TokenType::Equals)) {
        std::string name(advance().lexeme);
        advance(); // consume ':' or '='
        std::string value(advance().lexeme);
        consume(TokenType::Semicolon, "Expect ';' after attribute value.");
        element.attributes[name] = value;
    }
}

std::unique_ptr<ASTNode> Parser::parseTemplate() {
    consume(TokenType::BlockKeyword, "Expect block keyword.");
    auto templateNode = std::make_unique<TemplateNode>();

    if (check(TokenType::AtStyle)) {
        templateNode->templateType = TemplateType::Style;
    } else if (check(TokenType::AtElement)) {
        templateNode->templateType = TemplateType::Element;
    } else if (check(TokenType::AtVar)) {
        templateNode->templateType = TemplateType::Var;
    } else {
        throw std::runtime_error("Expect '@Style', '@Element', or '@Var' after '[Template]'.");
    }
    advance(); // consume @keyword

    templateNode->name = std::string(consume(TokenType::Identifier, "Expect template name.").lexeme);

    consume(TokenType::LeftBrace, "Expect '{' after template name.");

    if (templateNode->templateType == TemplateType::Style) {
        auto styleNode = std::make_unique<StyleNode>();
        parseStyleProperties(*styleNode);
        templateNode->children.push_back(std::move(styleNode));
    } else {
        while (!check(TokenType::RightBrace) && !isAtEnd()) {
            templateNode->children.push_back(parseStatement());
        }
    }

    consume(TokenType::RightBrace, "Expect '}' after template block.");

    return templateNode;
}

std::unique_ptr<ASTNode> Parser::parseCustomTemplate() {
    consume(TokenType::BlockKeyword, "Expect block keyword.");
    auto customTemplateNode = std::make_unique<CustomTemplateNode>();

    if (check(TokenType::AtStyle)) {
        customTemplateNode->templateType = TemplateType::Style;
    } else if (check(TokenType::AtElement)) {
        customTemplateNode->templateType = TemplateType::Element;
    } else if (check(TokenType::AtVar)) {
        customTemplateNode->templateType = TemplateType::Var;
    } else {
        throw std::runtime_error("Expect '@Style', '@Element', or '@Var' after '[Custom]'.");
    }
    advance(); // consume @keyword

    customTemplateNode->name = std::string(consume(TokenType::Identifier, "Expect template name.").lexeme);

    consume(TokenType::LeftBrace, "Expect '{' after template name.");

    if (customTemplateNode->templateType == TemplateType::Style) {
        auto styleNode = std::make_unique<StyleNode>();
        parseStyleProperties(*styleNode);
        customTemplateNode->children.push_back(std::move(styleNode));
    } else {
        while (!check(TokenType::RightBrace) && !isAtEnd()) {
            customTemplateNode->children.push_back(parseStatement());
        }
    }

    consume(TokenType::RightBrace, "Expect '}' after template block.");

    return customTemplateNode;
}

std::unique_ptr<ASTNode> Parser::parseOrigin() {
    consume(TokenType::BlockKeyword, "Expect block keyword.");
    auto originNode = std::make_unique<OriginNode>();

    originNode->originType = std::string(advance().lexeme);

    if (check(TokenType::Identifier)) {
        originNode->name = std::string(advance().lexeme);
    }

    const Token& leftBrace = consume(TokenType::LeftBrace, "Expect '{' after origin type.");

    int startPos = leftBrace.start_pos + leftBrace.lexeme.length();

    int lookahead = m_current;
    while (m_tokens[lookahead].type != TokenType::RightBrace && m_tokens[lookahead].type != TokenType::Eof) {
        lookahead++;
    }

    if (m_tokens[lookahead].type == TokenType::Eof) {
        throw std::runtime_error("Unterminated origin block.");
    }

    const Token& rightBrace = m_tokens[lookahead];
    int endPos = rightBrace.start_pos;

    originNode->content = m_source.substr(startPos, endPos - startPos);

    m_current = lookahead;
    consume(TokenType::RightBrace, "Expect '}' after origin block.");

    return originNode;
}

std::unique_ptr<ASTNode> Parser::parseImport() {
    consume(TokenType::BlockKeyword, "Expect block keyword.");
    auto importNode = std::make_unique<ImportNode>();

    ImportTarget target;
    if (check(TokenType::BlockKeyword)) {
        if (peek().lexeme == "[Custom]") {
            target.isCustom = true;
            advance();
        }
    }

    target.type = std::string(advance().lexeme);

    if (check(TokenType::Identifier)) {
        target.name = std::string(advance().lexeme);
    }
    importNode->targets.push_back(target);

    consume(TokenType::From, "Expect 'from' after import target.");
    importNode->path = std::string(advance().lexeme);

    if (check(TokenType::As)) {
        consume(TokenType::As, "Expect 'as' for alias.");
        importNode->alias = std::string(consume(TokenType::Identifier, "Expect alias name.").lexeme);
    }

    return importNode;
}

} // namespace CHTL
