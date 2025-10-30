#include "Parser.h"
#include "CHTL/Util/StringUtil.h"
#include <stdexcept>

namespace CHTL {

Parser::Parser(std::vector<Token> tokens, std::string_view source) : m_tokens(std::move(tokens)), m_source(source) {}

std::unique_ptr<ProgramNode> Parser::parse() {
    auto program = std::make_unique<ProgramNode>();
    while (!isAtEnd()) {
        program->children.push_back(parseStatement());
    }
    return program;
}

std::unique_ptr<ASTNode> Parser::parseStatement() {
    if (peek().type == TokenType::LEFT_BRACKET) {
        if (m_tokens.size() > m_current + 1 && m_tokens[m_current + 1].type == TokenType::TEMPLATE) {
            return parseTemplateNode();
        }
        if (m_tokens.size() > m_current + 1 && m_tokens[m_current + 1].type == TokenType::CUSTOM) {
            return parseCustomNode();
        }
        if (m_tokens.size() > m_current + 1 && m_tokens[m_current + 1].type == TokenType::IMPORT) {
            return parseImportNode();
        }
		if (m_tokens.size() > m_current + 1 && m_tokens[m_current + 1].type == TokenType::ORIGIN) {
			return parseOriginNode();
		}
        if (m_tokens.size() > m_current + 1 && m_tokens[m_current + 1].type == TokenType::NAMESPACE) {
            return parseNamespaceNode();
        }
    }
    if (peek().type == TokenType::AT) {
        return parseTemplateUsage();
    }
    if ((peek().type == TokenType::IDENTIFIER || peek().type == TokenType::STYLE || peek().type == TokenType::SCRIPT)
        && m_tokens.size() > m_current + 1 && m_tokens[m_current + 1].type == TokenType::LEFT_BRACE) {
        if (peek().lexeme == "text") {
            return parseTextNode();
        }
        return parseElementNode();
    }
    throw std::runtime_error("Unexpected token in parseStatement: " + peek().lexeme);
}

std::unique_ptr<TemplateUsageNode> Parser::parseTemplateUsage() {
    consume(TokenType::AT, "Expect '@' before template type.");
    const auto& typeToken = consume(TokenType::IDENTIFIER, "Expect template type.");
    auto node = std::make_unique<TemplateUsageNode>();
    if (typeToken.lexeme == "Style") {
        node->templateType = TemplateType::Style;
    } else if (typeToken.lexeme == "Element") {
        node->templateType = TemplateType::Element;
    } else {
        throw std::runtime_error("Invalid template type for usage: " + typeToken.lexeme);
    }
    node->name = consume(TokenType::IDENTIFIER, "Expect template name.").lexeme;

    if (match({TokenType::LEFT_BRACE})) {
        while (!check(TokenType::RIGHT_BRACE) && !isAtEnd()) {
            if (match({TokenType::DELETE})) {
                // This could be a style property deletion or an element deletion
                if (node->templateType == TemplateType::Style) {
                    do {
                        node->deleted_properties.push_back(consume(TokenType::IDENTIFIER, "Expect property name to delete.").lexeme);
                    } while (match({TokenType::COMMA}));
                } else { // Element deletion
                    ElementDeletionSpec spec;
                    spec.tagName = consume(TokenType::IDENTIFIER, "Expect tag name for deletion.").lexeme;
                    if (match({TokenType::LEFT_BRACKET})) {
                        spec.index = std::stoi(consume(TokenType::NUMBER, "Expect index for deletion.").lexeme);
                        consume(TokenType::RIGHT_BRACKET, "Expect ']' after index.");
                    }
                    node->deleted_elements.push_back(spec);
                }
                consume(TokenType::SEMICOLON, "Expect ';' after delete statement.");
            } else if (match({TokenType::INSERT})) {
                node->insertions.push_back(parseElementInsertion());
            } else if (node->templateType == TemplateType::Style) {
                std::string key = consume(TokenType::IDENTIFIER, "Expect property key.").lexeme;
                consume(TokenType::COLON, "Expect ':' after property key.");
                node->provided_values.push_back({key, parseConditionalExpression()});
                consume(TokenType::SEMICOLON, "Expect ';' after property value.");
            } else if (node->templateType == TemplateType::Element) {
                node->specializations.push_back(parseElementSpecialization());
            }
        }
        consume(TokenType::RIGHT_BRACE, "Expect '}' after provided values.");
    }

    if (match({TokenType::FROM})) {
        std::string ns = consume(TokenType::IDENTIFIER, "Expect namespace name.").lexeme;
        while (match({TokenType::DOT})) {
            ns += "." + consume(TokenType::IDENTIFIER, "Expect namespace name.").lexeme;
        }
        node->fromNamespace = ns;
    }

    if (previous().type != TokenType::RIGHT_BRACE) {
        consume(TokenType::SEMICOLON, "Expect ';' after template usage.");
    }

    return node;
}

std::unique_ptr<TemplateNode> Parser::parseTemplateNode() {
    consume(TokenType::LEFT_BRACKET, "Expect '[' before 'Template'.");
    consume(TokenType::TEMPLATE, "Expect 'Template' keyword.");
    consume(TokenType::RIGHT_BRACKET, "Expect ']' after 'Template'.");

    auto node = std::make_unique<TemplateNode>();

    consume(TokenType::AT, "Expect '@' after '[Template]'.");
    const auto& typeToken = consume(TokenType::IDENTIFIER, "Expect template type.");
    if (typeToken.lexeme == "Style") {
        node->templateType = TemplateType::Style;
    } else if (typeToken.lexeme == "Element") {
        node->templateType = TemplateType::Element;
    } else if (typeToken.lexeme == "Var") {
        node->templateType = TemplateType::Var;
    } else {
        throw std::runtime_error("Unknown template type: " + typeToken.lexeme);
    }

    node->name = consume(TokenType::IDENTIFIER, "Expect template name.").lexeme;
    consume(TokenType::LEFT_BRACE, "Expect '{' after template name.");

    while (!check(TokenType::RIGHT_BRACE) && !isAtEnd()) {
        if (match({TokenType::INHERIT}) || peek().type == TokenType::AT) {
            node->inheritances.push_back(parseTemplateUsage());
        } else if (node->templateType == TemplateType::Element) {
            node->children.push_back(parseStatement());
        } else { // Style or Var
            std::string key = consume(TokenType::IDENTIFIER, "Expect property key.").lexeme;
            consume(TokenType::COLON, "Expect ':' after property key.");
            node->properties.push_back({key, parseConditionalExpression()});
            consume(TokenType::SEMICOLON, "Expect ';' after property value.");
        }
    }

    consume(TokenType::RIGHT_BRACE, "Expect '}' after template block.");
    return node;
}

std::unique_ptr<TemplateNode> Parser::parseCustomNode() {
    consume(TokenType::LEFT_BRACKET, "Expect '[' before 'Custom'.");
    consume(TokenType::CUSTOM, "Expect 'Custom' keyword.");
    consume(TokenType::RIGHT_BRACKET, "Expect ']' after 'Custom'.");

    auto node = std::make_unique<TemplateNode>();
    node->isCustom = true;

    consume(TokenType::AT, "Expect '@' after '[Custom]'.");
    const auto& typeToken = consume(TokenType::IDENTIFIER, "Expect custom type.");
    if (typeToken.lexeme == "Style") {
        node->templateType = TemplateType::Style;
    } else if (typeToken.lexeme == "Element") {
        node->templateType = TemplateType::Element;
    } else {
        throw std::runtime_error("Unknown or unsupported custom type: " + typeToken.lexeme);
    }

    node->name = consume(TokenType::IDENTIFIER, "Expect custom name.").lexeme;
    consume(TokenType::LEFT_BRACE, "Expect '{' after custom name.");

    while (!check(TokenType::RIGHT_BRACE) && !isAtEnd()) {
        if (match({TokenType::INHERIT}) || peek().type == TokenType::AT) {
            node->inheritances.push_back(parseTemplateUsage());
        } else if (match({TokenType::DELETE})) {
            do {
                node->deleted_properties.push_back(consume(TokenType::IDENTIFIER, "Expect property name to delete.").lexeme);
            } while (match({TokenType::COMMA}));
            consume(TokenType::SEMICOLON, "Expect ';' after delete statement.");
        } else if (node->templateType == TemplateType::Element) {
            node->children.push_back(parseStatement());
        } else if (node->templateType == TemplateType::Style) {
            if (peek().type == TokenType::IDENTIFIER) {
                if (m_current + 1 < m_tokens.size() && m_tokens[m_current + 1].type == TokenType::COLON) {
                    // Full property with value
                    std::string key = consume(TokenType::IDENTIFIER, "Expect property key.").lexeme;
                    consume(TokenType::COLON, "Expect ':' after property key.");
                    node->properties.push_back({key, parseConditionalExpression()});
                    consume(TokenType::SEMICOLON, "Expect ';' after property value.");
                } else {
                    // Placeholder(s)
                    do {
                        std::string key = consume(TokenType::IDENTIFIER, "Expect placeholder key.").lexeme;
                        node->properties.push_back({key, nullptr});
                    } while (match({TokenType::COMMA}));
                    consume(TokenType::SEMICOLON, "Expect ';' after placeholder(s).");
                }
            } else {
                 throw std::runtime_error("Unexpected token in custom style template body: " + peek().lexeme);
            }
        } else {
            throw std::runtime_error("Unexpected token in custom template body: " + peek().lexeme);
        }
    }

    consume(TokenType::RIGHT_BRACE, "Expect '}' after custom block.");
    return node;
}

std::unique_ptr<ImportNode> Parser::parseImportNode() {
    consume(TokenType::LEFT_BRACKET, "Expect '[' before 'Import'.");
    consume(TokenType::IMPORT, "Expect 'Import' keyword.");
    consume(TokenType::RIGHT_BRACKET, "Expect ']' after 'Import'.");

    auto node = std::make_unique<ImportNode>();

    if (match({ TokenType::LEFT_BRACKET })) {
        if (match({ TokenType::CUSTOM })) {
            node->isCustom = true;
        }
        else if (match({ TokenType::TEMPLATE })) {
            node->isTemplate = true;
        }
        else if (match({ TokenType::IDENTIFIER }) && previous().lexeme == "Origin") {
            node->isOrigin = true;
        }
        consume(TokenType::RIGHT_BRACKET, "Expect ']' after import type.");
    }

    consume(TokenType::AT, "Expect '@' after '[Import]'.");
    const auto& typeToken = consume(TokenType::IDENTIFIER, "Expect import type.");
    if (typeToken.lexeme == "Html") {
        node->importType = ImportType::Html;
    }
    else if (typeToken.lexeme == "Style") {
        node->importType = ImportType::Style;
    }
    else if (typeToken.lexeme == "JavaScript") {
        node->importType = ImportType::JavaScript;
    }
    else if (typeToken.lexeme == "Chtl") {
        node->importType = ImportType::Chtl;
    }
    else if (typeToken.lexeme == "CJmod") {
        node->importType = ImportType::CJmod;
    }
    else {
        throw std::runtime_error("Unknown import type: " + typeToken.lexeme);
    }

    if (peek().type == TokenType::IDENTIFIER) {
        node->specificName = consume(TokenType::IDENTIFIER, "Expect specific name for import.").lexeme;
    }

    consume(TokenType::FROM, "Expect 'from' after import type.");
    node->filePath = consume(TokenType::STRING, "Expect file path.").lexeme;

    if (match({ TokenType::AS })) {
        node->alias = consume(TokenType::IDENTIFIER, "Expect alias name.").lexeme;
    }

    consume(TokenType::SEMICOLON, "Expect ';' after import statement.");

    return node;
}

std::unique_ptr<NamespaceNode> Parser::parseNamespaceNode() {
    consume(TokenType::LEFT_BRACKET, "Expect '[' before 'Namespace'.");
    consume(TokenType::NAMESPACE, "Expect 'Namespace' keyword.");
    consume(TokenType::RIGHT_BRACKET, "Expect ']' after 'Namespace'.");

    auto node = std::make_unique<NamespaceNode>();
    node->name = consume(TokenType::IDENTIFIER, "Expect namespace name.").lexeme;

    if (match({TokenType::LEFT_BRACE})) {
        while (!check(TokenType::RIGHT_BRACE) && !isAtEnd()) {
            node->children.push_back(parseStatement());
        }
        consume(TokenType::RIGHT_BRACE, "Expect '}' after namespace block.");
    }

    return node;
}

std::unique_ptr<OriginNode> Parser::parseOriginNode() {
    consume(TokenType::LEFT_BRACKET, "Expect '[' before 'Origin'.");
    consume(TokenType::ORIGIN, "Expect 'Origin' keyword.");
    consume(TokenType::RIGHT_BRACKET, "Expect ']' after 'Origin'.");

    auto node = std::make_unique<OriginNode>();

    consume(TokenType::AT, "Expect '@' after '[Origin]'.");
    node->type = consume(TokenType::IDENTIFIER, "Expect origin type.").lexeme;

    // Check for an optional name
    if (peek().type == TokenType::IDENTIFIER) {
        node->name = consume(TokenType::IDENTIFIER, "Expect origin name/alias.").lexeme;
    }

    if (match({TokenType::LEFT_BRACE})) { // It's a definition
        // Manually scan the source string for the raw content
        size_t search_start = previous().start_pos + previous().lexeme.length();
        int brace_level = 1;
        size_t search_current = search_start;
        while (brace_level > 0 && search_current < m_source.length()) {
            if (m_source[search_current] == '{') {
                brace_level++;
            } else if (m_source[search_current] == '}') {
                brace_level--;
            }
            search_current++;
        }

        if (brace_level > 0) {
            throw std::runtime_error("Unterminated origin block.");
        }

        size_t content_end = search_current - 1;
        node->content = m_source.substr(search_start, content_end - search_start);

        // Fast-forward the token stream to after the origin block
        while (!isAtEnd() && peek().start_pos < search_current) {
            advance();
        }
    } else if (!node->name.empty() && match({TokenType::SEMICOLON})) { // It's a usage
        // This is a valid usage, content remains empty
    } else {
        throw std::runtime_error("Invalid origin block: Expect '{' for definition or ';' for usage.");
    }

    return node;
}

std::unique_ptr<ElementSpecializationNode> Parser::parseElementSpecialization() {
    auto node = std::make_unique<ElementSpecializationNode>();
    node->tagName = consume(TokenType::IDENTIFIER, "Expect tag name for specialization.").lexeme;

    if (match({TokenType::LEFT_BRACKET})) {
        node->index = std::stoi(consume(TokenType::NUMBER, "Expect index in specialization.").lexeme);
        consume(TokenType::RIGHT_BRACKET, "Expect ']' after index.");
    }

    consume(TokenType::LEFT_BRACE, "Expect '{' for specialization block.");
    while (!check(TokenType::RIGHT_BRACE) && !isAtEnd()) {
        if (peek().type == TokenType::STYLE) {
            node->style = parseStyleNode(nullptr); // Pass nullptr since we're not in a real ElementNode
        } else {
            // Future: Handle other specializations like adding attributes or children
            throw std::runtime_error("Only 'style' blocks are currently supported in element specializations.");
        }
    }
    consume(TokenType::RIGHT_BRACE, "Expect '}' after specialization block.");

    return node;
}

std::unique_ptr<ElementInsertionNode> Parser::parseElementInsertion() {
    auto node = std::make_unique<ElementInsertionNode>();

    if (match({TokenType::AFTER})) {
        node->position = InsertionPosition::After;
    } else if (match({TokenType::BEFORE})) {
        node->position = InsertionPosition::Before;
    } else if (match({TokenType::REPLACE})) {
        node->position = InsertionPosition::Replace;
    } else if (peek().type == TokenType::IDENTIFIER && peek().lexeme == "at") {
        advance(); // consume 'at'
        if (match({TokenType::TOP})) {
            node->position = InsertionPosition::AtTop;
        } else if (match({TokenType::BOTTOM})) {
            node->position = InsertionPosition::AtBottom;
        } else {
            throw std::runtime_error("Expected 'top' or 'bottom' after 'at'.");
        }
    } else {
        throw std::runtime_error("Invalid insertion position.");
    }

    if (node->position == InsertionPosition::After || node->position == InsertionPosition::Before || node->position == InsertionPosition::Replace) {
        node->targetTagName = consume(TokenType::IDENTIFIER, "Expect target tag name.").lexeme;
        if (match({TokenType::LEFT_BRACKET})) {
            node->targetIndex = std::stoi(consume(TokenType::NUMBER, "Expect index.").lexeme);
            consume(TokenType::RIGHT_BRACKET, "Expect ']' after index.");
        }
    }

    consume(TokenType::LEFT_BRACE, "Expect '{' for insertion block.");
    while (!check(TokenType::RIGHT_BRACE) && !isAtEnd()) {
        node->new_elements.push_back(parseStatement());
    }
    consume(TokenType::RIGHT_BRACE, "Expect '}' after insertion block.");

    return node;
}

std::unique_ptr<ElementNode> Parser::parseElementNode() {
    auto node = std::make_unique<ElementNode>();
    if (peek().type == TokenType::IDENTIFIER || peek().type == TokenType::STYLE || peek().type == TokenType::SCRIPT) {
        node->tagName = advance().lexeme;
    } else {
        consume(TokenType::IDENTIFIER, "Expect element tag name."); // To throw the correct error
    }
    consume(TokenType::LEFT_BRACE, "Expect '{' after element tag name.");

    while (!check(TokenType::RIGHT_BRACE) && !isAtEnd()) {
        if (peek().type == TokenType::STYLE) {
            node->style = parseStyleNode(node.get());
        } else if (peek().type == TokenType::IDENTIFIER && m_tokens[m_current + 1].type == TokenType::COLON) {
            if (peek().lexeme == "text") {
                advance(); // consume 'text'
                consume(TokenType::COLON, "Expect ':' after 'text' attribute.");
                auto text_node = std::make_unique<TextNode>();
                if (peek().type == TokenType::STRING || peek().type == TokenType::UNQUOTED_LITERAL || peek().type == TokenType::IDENTIFIER) {
                    text_node->content = advance().lexeme;
                } else {
                    throw std::runtime_error("Expect string or unquoted literal for text attribute.");
                }
                node->children.push_back(std::move(text_node));
                consume(TokenType::SEMICOLON, "Expect ';' after text attribute.");
            } else {
                parseAttributes(*node);
            }
        } else {
            node->children.push_back(parseStatement());
        }
    }

    consume(TokenType::RIGHT_BRACE, "Expect '}' after element block.");
    return node;
}

std::unique_ptr<TextNode> Parser::parseTextNode() {
    consume(TokenType::IDENTIFIER, "Expect 'text' keyword.");
    consume(TokenType::LEFT_BRACE, "Expect '{' after 'text' keyword.");
    auto node = std::make_unique<TextNode>();
    if (peek().type == TokenType::STRING) {
        node->content = advance().lexeme;
    } else if (peek().type == TokenType::IDENTIFIER || peek().type == TokenType::UNQUOTED_LITERAL) {
        node->content = advance().lexeme;
    } else {
        throw std::runtime_error("Expect string or unquoted literal inside text block.");
    }
    consume(TokenType::RIGHT_BRACE, "Expect '}' after text content.");
    return node;
}

std::unique_ptr<StyleNode> Parser::parseStyleNode(ElementNode* parent) {
    consume(TokenType::STYLE, "Expect 'style' keyword.");
    consume(TokenType::LEFT_BRACE, "Expect '{' after 'style' keyword.");

    auto node = std::make_unique<StyleNode>();

    while (!check(TokenType::RIGHT_BRACE) && !isAtEnd()) {
        if (peek().type == TokenType::AT) {
            node->template_usages.push_back(parseTemplateUsage());
        } else if (peek().type == TokenType::DOT || peek().type == TokenType::HASH || peek().type == TokenType::AMPERSAND) {
            auto selector_block = std::make_unique<SelectorBlockNode>();
            std::string selector_str;

            if (match({TokenType::DOT})) {
                selector_str += "." + consume(TokenType::IDENTIFIER, "Expect class name.").lexeme;
                if(parent) parent->auto_classes.push_back(selector_str.substr(1));
            } else if (match({TokenType::HASH})) {
                selector_str += "#" + consume(TokenType::IDENTIFIER, "Expect id name.").lexeme;
                if(parent) parent->auto_ids.push_back(selector_str.substr(1));
            } else if (match({TokenType::AMPERSAND})) {
                selector_str += "&";
            }

            if (match({TokenType::COLON})) {
                selector_str += ":" + consume(TokenType::IDENTIFIER, "Expect pseudo-class name.").lexeme;
            } else if (match({TokenType::COLON_COLON})) {
                selector_str += "::";
                if (peek().type == TokenType::IDENTIFIER || peek().type == TokenType::BEFORE || peek().type == TokenType::AFTER) {
                    selector_str += advance().lexeme;
                } else {
                    consume(TokenType::IDENTIFIER, "Expect pseudo-element name.");
                }
            }

            selector_block->selector = selector_str;

            consume(TokenType::LEFT_BRACE, "Expect '{' after selector.");
            while (!check(TokenType::RIGHT_BRACE) && !isAtEnd()) {
                std::string key = consume(TokenType::IDENTIFIER, "Expect style property key.").lexeme;
                consume(TokenType::COLON, "Expect ':' after style property key.");
                selector_block->properties.push_back({key, parseConditionalExpression()});
                consume(TokenType::SEMICOLON, "Expect ';' after style property.");
            }
            consume(TokenType::RIGHT_BRACE, "Expect '}' after selector block.");
            node->selector_blocks.push_back(std::move(selector_block));
        } else {
            std::string key = consume(TokenType::IDENTIFIER, "Expect style property key.").lexeme;
            consume(TokenType::COLON, "Expect ':' after style property key.");

            node->properties.push_back({key, parseConditionalExpression()});
            consume(TokenType::SEMICOLON, "Expect ';' after style property.");
        }
    }

    consume(TokenType::RIGHT_BRACE, "Expect '}' after style block.");
    return node;
}


void Parser::parseAttributes(ElementNode& node) {
    std::string key = consume(TokenType::IDENTIFIER, "Expect attribute key.").lexeme;
    consume(TokenType::COLON, "Expect ':' after attribute key.");

    const Token& valueToken = advance();
    if (valueToken.type != TokenType::STRING && valueToken.type != TokenType::IDENTIFIER && valueToken.type != TokenType::NUMBER && valueToken.type != TokenType::UNQUOTED_LITERAL) {
        throw std::runtime_error("Expect attribute value (string, identifier, or number).");
    }

    node.attributes.push_back({key, valueToken.lexeme});
    consume(TokenType::SEMICOLON, "Expect ';' after attribute value.");
}

int Parser::getPrecedence(TokenType type) {
    switch (type) {
        case TokenType::PIPE_PIPE:
            return 1;
        case TokenType::AMPERSAND_AMPERSAND:
            return 2;
        case TokenType::EQUAL_EQUAL:
        case TokenType::BANG_EQUAL:
            return 3;
        case TokenType::GREATER:
        case TokenType::GREATER_EQUAL:
        case TokenType::LESS:
        case TokenType::LESS_EQUAL:
            return 4;
        case TokenType::PLUS:
        case TokenType::MINUS:
            return 5;
        case TokenType::STAR:
        case TokenType::SLASH:
        case TokenType::PERCENT:
            return 6;
        case TokenType::STAR_STAR:
            return 7;
        default:
            return 0;
    }
}

std::unique_ptr<ASTNode> Parser::parseConditionalExpression() {
    auto condition = parseExpression();
    if (match({TokenType::QUESTION})) {
        auto ternaryNode = std::make_unique<TernaryOpNode>();
        ternaryNode->condition = std::move(condition);
        ternaryNode->then_expr = parseConditionalExpression();
        consume(TokenType::COLON, "Expect ':' in ternary operator.");
        ternaryNode->else_expr = parseConditionalExpression();
        return ternaryNode;
    }
    return condition;
}

std::unique_ptr<ASTNode> Parser::parseExpression(int precedence) {
    auto left = parsePrefix();

    while (precedence < getPrecedence(peek().type)) {
        left = parseInfix(std::move(left));
    }

    return left;
}

std::unique_ptr<ASTNode> Parser::parsePrefix() {
    if (m_current + 1 < m_tokens.size() && m_tokens[m_current].type == TokenType::IDENTIFIER && m_tokens[m_current + 1].type == TokenType::DOT) {
        const std::string& name = peek().lexeme;
        // Heuristic: Var templates start with an uppercase letter, tags are lowercase.
        if (!name.empty() && isupper(name[0])) {
            advance(); // consume template name
            consume(TokenType::DOT, "Expect '.' after var template name.");
            std::string prop = consume(TokenType::IDENTIFIER, "Expect property name.").lexeme;
            auto node = std::make_unique<VarAccessNode>();
            node->templateName = name;
            node->property = prop;
            return node;
        }
    }


    bool isPropAccess = false;
    if (m_current + 1 < m_tokens.size()) {
        if (check(TokenType::IDENTIFIER) && m_tokens[m_current + 1].type == TokenType::DOT) {
            isPropAccess = true;
        } else if ((check(TokenType::DOT) || check(TokenType::HASH)) &&
                   m_current + 2 < m_tokens.size() &&
                   m_tokens[m_current + 1].type == TokenType::IDENTIFIER &&
                   m_tokens[m_current + 2].type == TokenType::DOT) {
            isPropAccess = true;
        }
    }

    if (isPropAccess) {
        std::string selector;
        if (check(TokenType::DOT) || check(TokenType::HASH)) {
            selector += advance().lexeme;
            selector += consume(TokenType::IDENTIFIER, "Expect selector name.").lexeme;
        } else {
            selector += consume(TokenType::IDENTIFIER, "Expect selector name.").lexeme;
        }

        consume(TokenType::DOT, "Expect '.' after selector.");
        std::string prop = consume(TokenType::IDENTIFIER, "Expect property name.").lexeme;

        auto node = std::make_unique<PropertyAccessNode>();
        node->selector = selector;
        node->property = prop;
        return node;
    }


    if (match({TokenType::NUMBER})) {
        auto node = std::make_unique<NumberLiteralNode>();
        try {
            node->value = std::stod(previous().lexeme);
        } catch (const std::invalid_argument& e) {
            throw std::runtime_error("Invalid number format: " + previous().lexeme);
        }

        if (check(TokenType::IDENTIFIER) || check(TokenType::PERCENT)) {
            node->unit = advance().lexeme;
        }

        if (check(TokenType::IDENTIFIER)) {
            std::string full_value = formatDouble(node->value) + node->unit;
            while(check(TokenType::IDENTIFIER)) {
                full_value += " " + advance().lexeme;
            }
            auto str_node = std::make_unique<IdentifierNode>();
            str_node->name = full_value;
            return str_node;
        }

        return node;
    }
    if (match({TokenType::STRING})) {
        auto node = std::make_unique<StringLiteralNode>();
        node->value = previous().lexeme;
        return node;
    }
     if (check(TokenType::IDENTIFIER)) {
        std::string value = advance().lexeme;
        while(check(TokenType::IDENTIFIER) || check(TokenType::NUMBER)) {
            value += " " + advance().lexeme;
        }
        auto node = std::make_unique<IdentifierNode>();
        node->name = value;
        return node;
    }

    throw std::runtime_error("Unexpected token in expression: " + peek().lexeme);
}

std::unique_ptr<ASTNode> Parser::parseInfix(std::unique_ptr<ASTNode> left) {
    auto node = std::make_unique<BinaryOpNode>();
    node->op = peek();
    advance();
    node->left = std::move(left);
    node->right = parseExpression(getPrecedence(node->op.type));
    return node;
}

const Token& Parser::advance() {
    if (!isAtEnd()) m_current++;
    return previous();
}

const Token& Parser::peek() const {
    return m_tokens[m_current];
}

const Token& Parser::previous() const {
    return m_tokens[m_current - 1];
}

bool Parser::isAtEnd() const {
    return peek().type == TokenType::END_OF_FILE;
}

bool Parser::check(TokenType type) const {
    if (isAtEnd()) return false;
    return peek().type == type;
}

bool Parser::match(const std::vector<TokenType>& types) {
    for (TokenType type : types) {
        if (check(type)) {
            advance();
            return true;
        }
    }
    return false;
}

const Token& Parser::consume(TokenType type, const std::string& message) {
    if (check(type)) return advance();
    throw std::runtime_error(message);
}

}
