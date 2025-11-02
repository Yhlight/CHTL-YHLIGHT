#include "Parser.h"
#include "CHTLNode/ElementNode.h"
#include "CHTLNode/TextNode.h"
#include "CHTLNode/StyleNode.h"
#include "CHTLNode/ScriptNode.h"
#include "CHTLNode/CommentNode.h"
#include "CHTLNode/TemplateNode.h"
#include "CHTLNode/CustomNode.h"
#include "CHTLNode/OriginNode.h"
#include "CHTLNode/ImportNode.h"
#include "CHTLNode/NamespaceNode.h"
#include "Util/StringUtil.h"

Parser::Parser(std::vector<Token> tokens, std::string source) : tokens(std::move(tokens)), source(std::move(source)) {}

AstNodeList Parser::parse() {
    AstNodeList nodes;
    while (!isAtEnd()) {
        nodes.push_back(parseStatement());
    }
    return nodes;
}

AstNode Parser::parseStatement() {
    switch (peek().type) {
        case TokenType::Identifier:
            return parseElement();
        case TokenType::TemplateKeyword:
            return parseTemplate();
        case TokenType::CustomKeyword:
            return parseCustom();
        case TokenType::OriginKeyword:
            return parseOrigin();
        case TokenType::ImportKeyword:
            return parseImport();
        case TokenType::NamespaceKeyword:
            return parseNamespace();
        case TokenType::GeneratorComment:
            return parseComment();
        default:
            advance();
            return nullptr;
    }
}

AstNode Parser::parseTemplate() {
    advance(); // consume [Template]
    auto node = std::make_unique<TemplateNode>();

    if (peek().type == TokenType::StyleSpecifier || peek().type == TokenType::ElementSpecifier || peek().type == TokenType::VarSpecifier) {
        node->type = advance().value;
    }
    if (peek().type == TokenType::Identifier) {
        node->name = advance().value;
    }
    if (peek().type == TokenType::OpenBrace) {
        advance(); // consume '{'
        while (peek().type != TokenType::CloseBrace && !isAtEnd()) {
            node->body.push_back(parseStatement());
        }
        if (peek().type == TokenType::CloseBrace) {
            advance(); // consume '}'
        }
    }
    return node;
}

AstNode Parser::parseCustom() {
    advance(); // consume [Custom]
    auto node = std::make_unique<CustomNode>();

    if (peek().type == TokenType::StyleSpecifier || peek().type == TokenType::ElementSpecifier || peek().type == TokenType::VarSpecifier) {
        node->type = advance().value;
    }
    if (peek().type == TokenType::Identifier) {
        node->name = advance().value;
    }
    if (peek().type == TokenType::OpenBrace) {
        advance(); // consume '{'
        while (peek().type != TokenType::CloseBrace && !isAtEnd()) {
            node->body.push_back(parseStatement());
        }
        if (peek().type == TokenType::CloseBrace) {
            advance(); // consume '}'
        }
    }
    return node;
}

AstNode Parser::parseOrigin() {
    advance(); // consume [Origin]
    auto node = std::make_unique<OriginNode>();

    if (peek().type == TokenType::HtmlSpecifier || peek().type == TokenType::StyleSpecifier || peek().type == TokenType::JavaScriptSpecifier) {
        node->type = advance().value;
    }
    if (peek().type == TokenType::Identifier) {
        node->name = advance().value;
    }
    if (peek().type == TokenType::OpenBrace) {
        advance(); // consume '{'
        size_t start = peek().pos;
        while (peek().type != TokenType::CloseBrace && !isAtEnd()) {
            advance();
        }
        size_t end = peek().pos;
        node->content = source.substr(start, end - start);

        if (peek().type == TokenType::CloseBrace) {
            advance(); // consume '}'
        }
    }
    return node;
}

AstNode Parser::parseImport() {
    advance(); // consume [Import]
    auto node = std::make_unique<ImportNode>();

    if (peek().type == TokenType::ChtlSpecifier || peek().type == TokenType::HtmlSpecifier || peek().type == TokenType::StyleSpecifier || peek().type == TokenType::JavaScriptSpecifier) {
        node->type = advance().value;
    }

    if (peek().type == TokenType::FromKeyword) {
        advance(); // consume 'from'
    }

    if (peek().type == TokenType::StringLiteral) {
        node->file_path = advance().value;
    }

    if (peek().type == TokenType::AsKeyword) {
        advance(); // consume 'as'
        if (peek().type == TokenType::Identifier) {
            node->as_name = advance().value;
        }
    }

    return node;
}

AstNode Parser::parseNamespace() {
    advance(); // consume [Namespace]
    auto node = std::make_unique<NamespaceNode>();

    if (peek().type == TokenType::Identifier) {
        node->name = advance().value;
    }

    if (peek().type == TokenType::OpenBrace) {
        advance(); // consume '{'
        while (peek().type != TokenType::CloseBrace && !isAtEnd()) {
            node->body.push_back(parseStatement());
        }
        if (peek().type == TokenType::CloseBrace) {
            advance(); // consume '}'
        }
    }
    return node;
}

AstNode Parser::parseStyleBlock() {
    advance(); // consume 'style'
    if (peek().type != TokenType::OpenBrace) return nullptr;
    advance(); // consume '{'

    auto styleNode = std::make_unique<StyleNode>();
    size_t start = peek().pos;
    int braceLevel = 1;
    while (!isAtEnd()) {
        if (peek().type == TokenType::OpenBrace) braceLevel++;
        if (peek().type == TokenType::CloseBrace) braceLevel--;
        if (braceLevel == 0) break;
        advance();
    }
    size_t end = peek().pos;
    std::string content = source.substr(start, end - start);
    StringUtil::trim(content);
    styleNode->content = content;

    if (peek().type == TokenType::CloseBrace) advance(); // consume '}'
    return styleNode;
}

AstNode Parser::parseScriptBlock() {
    advance(); // consume 'script'
    if (peek().type != TokenType::OpenBrace) return nullptr;
    advance(); // consume '{'

    auto scriptNode = std::make_unique<ScriptNode>();
    size_t start = peek().pos;
    int braceLevel = 1;
    while (!isAtEnd()) {
        if (peek().type == TokenType::OpenBrace) braceLevel++;
        if (peek().type == TokenType::CloseBrace) braceLevel--;
        if (braceLevel == 0) break;
        advance();
    }
    size_t end = peek().pos;
    std::string content = source.substr(start, end - start);
    StringUtil::trim(content);
    scriptNode->content = content;

    if (peek().type == TokenType::CloseBrace) advance(); // consume '}'
    return scriptNode;
}

AstNode Parser::parseComment() {
    auto commentNode = std::make_unique<CommentNode>();
    Token token = advance();
    commentNode->comment = token.value;
    commentNode->type = token.type;
    return commentNode;
}

AstNode Parser::parseElement() {
    if (peek().type != TokenType::Identifier) {
        return nullptr; // Expected a tag name
    }
    auto element = std::make_unique<ElementNode>();
    element->tag_name = advance().value;

    if (peek().type != TokenType::OpenBrace) {
        return element; // Self-closing or content-less element
    }
    advance(); // consume '{'

    while (peek().type != TokenType::CloseBrace && !isAtEnd()) {
        switch (peek().type) {
            case TokenType::Identifier:
                if (peekNext().type == TokenType::Colon) {
                    std::string key = advance().value;
                    advance(); // consume ':'
                    if (peek().type == TokenType::StringLiteral || peek().type == TokenType::NumericLiteral) {
                        element->attributes[key] = advance().value;
                    }
                    if (peek().type == TokenType::Semicolon) advance();
                } else {
                    element->children.push_back(parseElement());
                }
                break;
            case TokenType::TextKeyword:
                advance(); // consume 'text'
                if (peek().type == TokenType::OpenBrace) {
                    advance(); // consume '{'
                    auto textNode = std::make_unique<TextNode>();
                    if (peek().type == TokenType::StringLiteral || peek().type == TokenType::Identifier) {
                        textNode->text = advance().value;
                    }
                    element->children.push_back(std::move(textNode));
                    if (peek().type == TokenType::CloseBrace) advance();
                }
                break;
            case TokenType::StyleKeyword:
                element->children.push_back(parseStyleBlock());
                break;
            case TokenType::ScriptKeyword:
                element->children.push_back(parseScriptBlock());
                break;
            case TokenType::LineComment:
            case TokenType::BlockComment:
            case TokenType::GeneratorComment:
                element->children.push_back(parseComment());
                break;
            default:
                advance(); // Skip unexpected tokens
                break;
        }
    }

    if (peek().type == TokenType::CloseBrace) {
        advance(); // consume '}'
    }

    return element;
}

Token Parser::advance() {
    if (!isAtEnd()) current++;
    return tokens[current - 1];
}

Token Parser::peek() {
    return tokens[current];
}

Token Parser::peekNext() {
    if (current + 1 >= tokens.size()) {
        return {TokenType::EndOfFile, "", 0, 0};
    }
    return tokens[current + 1];
}

bool Parser::isAtEnd() {
    return peek().type == TokenType::EndOfFile;
}
