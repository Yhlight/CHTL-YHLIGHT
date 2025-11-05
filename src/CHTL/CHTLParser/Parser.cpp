#include "Parser.h"
#include "../CHTLNode/ElementNode.h"
#include "../CHTLNode/TextNode.h"
#include "../CHTLNode/StyleNode.h"
#include "../CHTLNode/ScriptNode.h"
#include "../CHTLNode/TemplateNode.h"
#include "../CHTLNode/TemplateUsageNode.h"
#include "../CHTLNode/StylePropertyNode.h"
#include "../CHTLNode/ValueNode/LiteralValueNode.h"
#include "../CHTLNode/ValueNode/VariableUsageNode.h"
#include "../CHTLNode/ImportNode.h"
#include <stdexcept>
#include <iostream>

namespace CHTL {

Parser::Parser(std::string_view source)
    : source_(source), lexer_(source) {}

std::unique_ptr<ProgramNode> Parser::parse() {
    auto programNode = std::make_unique<ProgramNode>();
    advance();
    while (!match(TokenType::END_OF_FILE)) {
        programNode->children.push_back(parseStatement());
    }
    return programNode;
}

void Parser::advance() {
    previousToken_ = currentToken_;
    for (;;) {
        currentToken_ = lexer_.scanToken();
        if (currentToken_.type != TokenType::ERROR) break;

        throw std::runtime_error(std::string(currentToken_.lexeme));
    }
}

void Parser::consume(TokenType type, const char* message) {
    if (check(type)) {
        advance();
        return;
    }
    throw std::runtime_error(message);
}

bool Parser::match(TokenType type) {
    if (!check(type)) return false;
    advance();
    return true;
}

bool Parser::check(TokenType type) const {
    return currentToken_.type == type;
}

const Token& Parser::previous() const {
    return previousToken_;
}

const Token& Parser::peek() const {
    return currentToken_;
}

std::string Parser::parseQualifiedName() {
    consume(TokenType::IDENTIFIER, "Expect identifier.");
    std::string name = std::string(previous().lexeme);
    while (match(TokenType::COLON_COLON)) {
        consume(TokenType::IDENTIFIER, "Expect identifier after '::'.");
        name += "::" + std::string(previous().lexeme);
    }
    return name;
}

std::unique_ptr<ASTNode> Parser::parseStatement() {
    if (check(TokenType::TEXT)) {
        return parseText();
    } else if (check(TokenType::STYLE)) {
        return parseStyle();
    } else if (check(TokenType::SCRIPT)) {
        return parseScript();
    } else if (check(TokenType::IDENTIFIER)) {
        return parseElement();
    } else if (check(TokenType::BLOCK_TEMPLATE)) {
        return parseTemplate();
    } else if (check(TokenType::BLOCK_NAMESPACE)) {
        return parseNamespace();
    } else if (check(TokenType::BLOCK_IMPORT)) {
        return parseImport();
    } else if (check(TokenType::BLOCK_ORIGIN)) {
        return parseOrigin();
    } else if (check(TokenType::AT)) {
        advance(); // consume '@'
        consume(TokenType::IDENTIFIER, "Expect template type.");
        std::string type = std::string(previous().lexeme);
        std::string name = parseQualifiedName();
        consume(TokenType::SEMICOLON, "Expect ';' after template usage.");
        return std::make_unique<TemplateUsageNode>(type, name);
    }
    throw std::runtime_error("Expected a statement.");
}

std::unique_ptr<StyleNode> Parser::parseStyle() {
    consume(TokenType::STYLE, "Expect 'style' keyword.");
    consume(TokenType::LEFT_BRACE, "Expect '{' after 'style' keyword.");

    auto styleNode = std::make_unique<StyleNode>();

    while (!check(TokenType::RIGHT_BRACE) && !check(TokenType::END_OF_FILE)) {
        if (check(TokenType::AT)) {
            advance(); // consume '@'
            consume(TokenType::IDENTIFIER, "Expect template type.");
            std::string type = std::string(previous().lexeme);
            std::string name = parseQualifiedName();
            consume(TokenType::SEMICOLON, "Expect ';' after template usage.");
            styleNode->children.push_back(std::make_unique<TemplateUsageNode>(type, name));
        } else {
            consume(TokenType::IDENTIFIER, "Expect property name.");
            std::string name = std::string(previous().lexeme);
            consume(TokenType::COLON, "Expect ':' after property name.");
            auto value = parseStylePropertyValue();
            consume(TokenType::SEMICOLON, "Expect ';' after property value.");
            styleNode->children.push_back(std::make_unique<StylePropertyNode>(name, std::move(value)));
        }
    }

    consume(TokenType::RIGHT_BRACE, "Expect '}' after style block.");

    return styleNode;
}

std::unique_ptr<ValueNode> Parser::parseStylePropertyValue() {
    if (match(TokenType::AT)) {
        consume(TokenType::IDENTIFIER, "Expect variable name.");
        return std::make_unique<VariableUsageNode>(std::string(previous().lexeme));
    }

    if (match(TokenType::STRING)) {
        std::string_view lexeme = previous().lexeme;
        auto value = lexeme.substr(1, lexeme.length() - 2); // Remove quotes
        return std::make_unique<LiteralValueNode>(std::string(value));
    }

    std::string value;
    while (!check(TokenType::SEMICOLON) && !check(TokenType::END_OF_FILE)) {
        value += currentToken_.lexeme;
        advance();
    }
    return std::make_unique<LiteralValueNode>(value);
}

std::unique_ptr<TemplateNode> Parser::parseTemplate() {
    consume(TokenType::BLOCK_TEMPLATE, "Expect '[Template]' keyword.");
    consume(TokenType::AT, "Expect '@' after '[Template]'.");
    consume(TokenType::IDENTIFIER, "Expect template type.");
    std::string type = std::string(previous().lexeme);
    consume(TokenType::IDENTIFIER, "Expect template name.");
    std::string name = std::string(previous().lexeme);

    auto templateNode = std::make_unique<TemplateNode>(type, name);

    consume(TokenType::LEFT_BRACE, "Expect '{' after template name.");

    if (type == "Style" || type == "Var") {
        while (!check(TokenType::RIGHT_BRACE) && !check(TokenType::END_OF_FILE)) {
            consume(TokenType::IDENTIFIER, "Expect property name.");
            std::string propName = std::string(previous().lexeme);
            consume(TokenType::COLON, "Expect ':' after property name.");
            auto propValue = parseStylePropertyValue();
            consume(TokenType::SEMICOLON, "Expect ';' after property value.");
            templateNode->children.push_back(std::make_unique<StylePropertyNode>(propName, std::move(propValue)));
        }
    } else if (type == "Element") {
        while (!check(TokenType::RIGHT_BRACE) && !check(TokenType::END_OF_FILE)) {
            templateNode->children.push_back(parseStatement());
        }
    }

    consume(TokenType::RIGHT_BRACE, "Expect '}' after template block.");

    return templateNode;
}

std::unique_ptr<ImportNode> Parser::parseImport() {
    consume(TokenType::BLOCK_IMPORT, "Expect '[Import]' keyword.");
    consume(TokenType::AT, "Expect '@' after '[Import]'.");
    consume(TokenType::IDENTIFIER, "Expect import type.");
    std::string type = std::string(previous().lexeme);

    ImportType importType;
    if (type == "Html") {
        importType = ImportType::Html;
    } else if (type == "Style") {
        importType = ImportType::Style;
    } else if (type == "JavaScript") {
        importType = ImportType::JavaScript;
    } else if (type == "Chtl") {
        importType = ImportType::Chtl;
    } else if (type == "Config") {
        importType = ImportType::Config;
    } else {
        throw std::runtime_error("Unknown import type.");
    }

    consume(TokenType::FROM, "Expect 'from' keyword.");

    std::string path;
    if (match(TokenType::STRING)) {
        std::string_view lexeme = previous().lexeme;
        path = lexeme.substr(1, lexeme.length() - 2);
    } else if (match(TokenType::IDENTIFIER)) {
        path = std::string(previous().lexeme);
    } else {
        throw std::runtime_error("Expect import path.");
    }

    auto importNode = std::make_unique<ImportNode>(importType, path);

    if (match(TokenType::AS)) {
        consume(TokenType::IDENTIFIER, "Expect alias.");
        // For now, we're not handling aliasing in this simplified implementation.
    }

    consume(TokenType::SEMICOLON, "Expect ';' after import statement.");
    return importNode;
}

std::unique_ptr<ScriptNode> Parser::parseScript() {
    consume(TokenType::SCRIPT, "Expect 'script' keyword.");
    consume(TokenType::LEFT_BRACE, "Expect '{' after 'script' keyword.");

    size_t start = previousToken_.start_pos + previousToken_.lexeme.length();
    while (!check(TokenType::RIGHT_BRACE) && !check(TokenType::END_OF_FILE)) {
        advance();
    }
    size_t end = currentToken_.start_pos;

    consume(TokenType::RIGHT_BRACE, "Expect '}' after script block.");

    return std::make_unique<ScriptNode>(std::string(source_.substr(start, end - start)));
}

std::unique_ptr<ElementNode> Parser::parseElement() {
    consume(TokenType::IDENTIFIER, "Expect element name.");
    auto element = std::make_unique<ElementNode>(std::string(previous().lexeme));

    consume(TokenType::LEFT_BRACE, "Expect '{' after element name.");

    while (!check(TokenType::RIGHT_BRACE) && !check(TokenType::END_OF_FILE)) {
        if (check(TokenType::IDENTIFIER)) {
            Token nextToken = lexer_.peekToken();
            if (nextToken.type == TokenType::COLON || nextToken.type == TokenType::EQUAL) {
                parseAttributes(*element);
            } else {
                element->children.push_back(parseStatement());
            }
        } else {
            element->children.push_back(parseStatement());
        }
    }

    consume(TokenType::RIGHT_BRACE, "Expect '}' after element block.");

    return element;
}

void Parser::parseAttributes(ElementNode& element) {
    consume(TokenType::IDENTIFIER, "Expect attribute name.");
    std::string name = std::string(previous().lexeme);

    if (!match(TokenType::COLON) && !match(TokenType::EQUAL)) {
        throw std::runtime_error("Expect ':' or '=' after attribute name.");
    }

    if (match(TokenType::STRING)) {
        std::string_view lexeme = previous().lexeme;
        auto value = lexeme.substr(1, lexeme.length() - 2); // Remove quotes
        element.attributes[name] = std::string(value);
    } else if (match(TokenType::IDENTIFIER) || match(TokenType::NUMBER)) {
        element.attributes[name] = std::string(previous().lexeme);
    } else {
        throw std::runtime_error("Expect attribute value.");
    }

    consume(TokenType::SEMICOLON, "Expect ';' after attribute value.");
}

std::unique_ptr<TextNode> Parser::parseText() {
    consume(TokenType::TEXT, "Expect 'text' keyword.");
    consume(TokenType::LEFT_BRACE, "Expect '{' after 'text' keyword.");
    consume(TokenType::STRING, "Expect string literal in text block.");
    std::string_view lexeme = previous().lexeme;
    auto content = lexeme.substr(1, lexeme.length() - 2); // Remove quotes
    auto text = std::make_unique<TextNode>(std::string(content));
    consume(TokenType::RIGHT_BRACE, "Expect '}' after text block.");
    return text;
}

std::unique_ptr<NamespaceNode> Parser::parseNamespace() {
    consume(TokenType::BLOCK_NAMESPACE, "Expect '[Namespace]' keyword.");
    consume(TokenType::IDENTIFIER, "Expect namespace name.");
    std::string name = std::string(previous().lexeme);

    auto namespaceNode = std::make_unique<NamespaceNode>(name);

    if (match(TokenType::LEFT_BRACE)) {
        while (!check(TokenType::RIGHT_BRACE) && !check(TokenType::END_OF_FILE)) {
            namespaceNode->children.push_back(parseStatement());
        }
        consume(TokenType::RIGHT_BRACE, "Expect '}' after namespace block.");
    }

    return namespaceNode;
}

std::unique_ptr<OriginNode> Parser::parseOrigin() {
    consume(TokenType::BLOCK_ORIGIN, "Expect '[Origin]' keyword.");
    consume(TokenType::AT, "Expect '@' after '[Origin]'.");
    consume(TokenType::IDENTIFIER, "Expect origin type.");
    std::string type = std::string(previous().lexeme);

    std::optional<std::string> name;
    if (match(TokenType::IDENTIFIER)) {
        name = std::string(previous().lexeme);
    }

    consume(TokenType::LEFT_BRACE, "Expect '{' after origin type.");

    size_t start = previousToken_.start_pos + previousToken_.lexeme.length();
    int braceCount = 1;
    size_t current = start;
    while (braceCount > 0 && current < source_.length()) {
        if (source_[current] == '{') {
            braceCount++;
        } else if (source_[current] == '}') {
            braceCount--;
        }
        current++;
    }
    size_t end = current - 1;

    std::string content = std::string(source_.substr(start, end - start));

    lexer_.setPosition(end);
    advance(); // Consume the closing brace
    advance(); // Advance to the next token

    return std::make_unique<OriginNode>(type, name, content);
}

} // namespace CHTL
