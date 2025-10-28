#include "Parser.h"
#include "CHTL/Util/StringUtil.h"
#include <stdexcept>

namespace CHTL {

Parser::Parser(std::vector<Token> tokens) : m_tokens(std::move(tokens)) {}

std::unique_ptr<ProgramNode> Parser::parse() {
    auto program = std::make_unique<ProgramNode>();
    while (!isAtEnd()) {
        program->children.push_back(parseStatement());
    }
    return program;
}

std::unique_ptr<ASTNode> Parser::parseStatement() {
    if (peek().type == TokenType::LEFT_BRACKET && m_tokens[m_current + 1].type == TokenType::TEMPLATE) {
        return parseTemplateNode();
    }
    if (peek().type == TokenType::AT) {
        advance(); // consume '@'
        const auto& typeToken = peek();
        if (typeToken.type == TokenType::IDENTIFIER) {
            if (typeToken.lexeme == "Style") {
                advance(); // consume 'Style'
                auto node = std::make_unique<TemplateUsageNode>();
                node->templateType = TemplateType::Style;
                node->name = consume(TokenType::IDENTIFIER, "Expect template name.").lexeme;
                consume(TokenType::SEMICOLON, "Expect ';' after template usage.");
                return node;
            }
            if (typeToken.lexeme == "Element") {
                advance(); // consume 'Element'
                auto node = std::make_unique<TemplateUsageNode>();
                node->templateType = TemplateType::Element;
                node->name = consume(TokenType::IDENTIFIER, "Expect template name.").lexeme;
                consume(TokenType::SEMICOLON, "Expect ';' after template usage.");
                return node;
            }
        }
    }
    if (peek().type == TokenType::IDENTIFIER && m_tokens[m_current + 1].type == TokenType::LEFT_BRACE) {
        return parseElementNode();
    }
    if (peek().type == TokenType::TEXT) {
         return parseTextNode();
    }
    throw std::runtime_error("Unexpected token in parseStatement: " + peek().lexeme);
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

    if (node->templateType == TemplateType::Element) {
        while (!check(TokenType::RIGHT_BRACE) && !isAtEnd()) {
            node->children.push_back(parseStatement());
        }
    } else { // Style or Var
        while (!check(TokenType::RIGHT_BRACE) && !isAtEnd()) {
            std::string key = consume(TokenType::IDENTIFIER, "Expect property key.").lexeme;
            consume(TokenType::COLON, "Expect ':' after property key.");
            node->properties.push_back({key, parseConditionalExpression()});
            consume(TokenType::SEMICOLON, "Expect ';' after property value.");
        }
    }

    consume(TokenType::RIGHT_BRACE, "Expect '}' after template block.");
    return node;
}

std::unique_ptr<ElementNode> Parser::parseElementNode() {
    auto node = std::make_unique<ElementNode>();
    node->tagName = consume(TokenType::IDENTIFIER, "Expect element tag name.").lexeme;
    consume(TokenType::LEFT_BRACE, "Expect '{' after element tag name.");

    while (!check(TokenType::RIGHT_BRACE) && !isAtEnd()) {
        if (peek().type == TokenType::STYLE) {
            node->style = parseStyleNode(node.get());
        } else if (peek().type == TokenType::IDENTIFIER && m_tokens[m_current + 1].type == TokenType::COLON) {
            parseAttributes(*node);
        } else {
            node->children.push_back(parseStatement());
        }
    }

    consume(TokenType::RIGHT_BRACE, "Expect '}' after element block.");
    return node;
}

std::unique_ptr<TextNode> Parser::parseTextNode() {
    consume(TokenType::TEXT, "Expect 'text' keyword.");
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
            advance(); // consume '@'
            const auto& typeToken = peek();
            if (typeToken.type == TokenType::IDENTIFIER && typeToken.lexeme == "Style") {
                advance(); // consume 'Style'
                auto usageNode = std::make_unique<TemplateUsageNode>();
                usageNode->templateType = TemplateType::Style;
                usageNode->name = consume(TokenType::IDENTIFIER, "Expect template name.").lexeme;
                consume(TokenType::SEMICOLON, "Expect ';' after template usage.");
                node->template_usages.push_back(std::move(usageNode));
            }
        } else if (peek().type == TokenType::DOT || peek().type == TokenType::HASH || peek().type == TokenType::AMPERSAND) {
            auto selector_block = std::make_unique<SelectorBlockNode>();
            std::string selector_str;

            if (match({TokenType::DOT})) {
                selector_str += "." + consume(TokenType::IDENTIFIER, "Expect class name.").lexeme;
                 parent->auto_classes.push_back(selector_str.substr(1));
            } else if (match({TokenType::HASH})) {
                selector_str += "#" + consume(TokenType::IDENTIFIER, "Expect id name.").lexeme;
                 parent->auto_ids.push_back(selector_str.substr(1));
            } else if (match({TokenType::AMPERSAND})) {
                selector_str += "&";
            }

            if (peek().type == TokenType::COLON) {
                selector_str += consume(TokenType::COLON, "Expect ':'.").lexeme;
                selector_str += consume(TokenType::IDENTIFIER, "Expect pseudo-class name.").lexeme;
            } else if (peek().type == TokenType::COLON_COLON) {
                selector_str += consume(TokenType::COLON_COLON, "Expect '::'.").lexeme;
                selector_str += consume(TokenType::IDENTIFIER, "Expect pseudo-element name.").lexeme;
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
