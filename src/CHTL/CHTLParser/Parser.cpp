#include "Parser.h"

Parser::Parser(const std::string& source, const std::vector<Token>& tokens) : source(source), tokens(tokens), current_token_index(0) {}

std::unique_ptr<ProgramNode> Parser::parse() {
    auto program = std::make_unique<ProgramNode>();
    while (current_token().type != TokenType::EndOfFile) {
        auto statement = parse_statement();
        if (statement) {
            program->children.push_back(std::move(statement));
        }
    }
    return program;
}

std::unique_ptr<BaseNode> Parser::parse_statement() {
    if (current_token().type == TokenType::TemplateKeyword) {
        return parse_template();
    } else if (current_token().type == TokenType::CustomKeyword) {
        return parse_custom();
    } else if (current_token().type == TokenType::Identifier) {
        return parse_element();
    } else if (current_token().type == TokenType::At) {
        advance(); // Consume '@'
        if (current_token().type == TokenType::Identifier && current_token().value == "Element") {
            advance(); // Consume 'Element'
            return parse_element_directive();
        }
    }
    advance();
    return nullptr;
}

std::unique_ptr<ElementNode> Parser::parse_element() {
    auto node = std::make_unique<ElementNode>(current_token().value);
    advance(); // Consume the identifier

    if (current_token().type != TokenType::OpenBrace) {
        // Handle error: expected '{'
        return nullptr;
    }
    advance(); // Consume the '{'

    while (current_token().type != TokenType::CloseBrace && current_token().type != TokenType::EndOfFile) {
        if (current_token().type == TokenType::Identifier) {
            std::string key = current_token().value;
            advance(); // Consume the identifier

            if (key == "style") {
                auto style_node = parse_style();
                if (style_node) {
                    node->children.push_back(std::move(style_node));
                }
                continue;
            }

            if (key == "script") {
                auto script_node = parse_script();
                if (script_node) {
                    node->children.push_back(std::move(script_node));
                }
                continue;
            }

            if (current_token().type != TokenType::Colon && current_token().type != TokenType::Equal) {
                // Handle error: expected ':' or '='
                return nullptr;
            }
            advance(); // Consume the ':' or '='

            if (current_token().type != TokenType::StringLiteral) {
                // Handle error: expected a string literal
                return nullptr;
            }
            std::string value = current_token().value;
            advance(); // Consume the string literal

            if (current_token().type != TokenType::Semicolon) {
                // Handle error: expected ';'
                return nullptr;
            }
            advance(); // Consume ';'

            if (key == "text") {
                node->children.push_back(std::make_unique<TextNode>(value));
            } else {
                node->attributes[key] = value;
            }
        } else {
            auto statement = parse_statement();
            if (statement) {
                node->children.push_back(std::move(statement));
            }
        }
    }

    if (current_token().type != TokenType::CloseBrace) {
        // Handle error: expected '}'
        return nullptr;
    }
    advance(); // Consume the '}'

    return node;
}

std::unique_ptr<StyleNode> Parser::parse_style() {
    auto node = std::make_unique<StyleNode>();
    if (current_token().type != TokenType::OpenBrace) {
        // Handle error: expected '{'
        return nullptr;
    }
    advance(); // Consume the '{'

    while (current_token().type != TokenType::CloseBrace && current_token().type != TokenType::EndOfFile) {
        if (current_token().type == TokenType::Dot) {
            advance(); // Consume the '.'
            if (current_token().type != TokenType::Identifier) {
                // Handle error: expected identifier
                return nullptr;
            }
            auto rule = std::make_unique<StyleRuleNode>(current_token().value);
            advance(); // Consume the identifier

            if (current_token().type != TokenType::OpenBrace) {
                // Handle error: expected '{'
                return nullptr;
            }
            advance(); // Consume the '{'

            while (current_token().type != TokenType::CloseBrace && current_token().type != TokenType::EndOfFile) {
                if (current_token().type == TokenType::Identifier) {
                    std::string key = current_token().value;
                    advance(); // Consume the identifier

                    if (current_token().type != TokenType::Colon) {
                        // Handle error: expected ':'
                        return nullptr;
                    }
                    advance(); // Consume the ':'

                    if (current_token().type != TokenType::StringLiteral && current_token().type != TokenType::Identifier) {
                        // Handle error: expected a string literal or identifier
                        return nullptr;
                    }
                    std::string value = current_token().value;
                    advance(); // Consume the value

                    if (current_token().type != TokenType::Semicolon) {
                        // Handle error: expected ';'
                        return nullptr;
                    }
                    advance(); // Consume ';'
                    rule->children.push_back(std::make_unique<StylePropertyNode>(key, value));
                } else {
                    // Handle other style content
                    advance();
                }
            }
            if (current_token().type != TokenType::CloseBrace) {
                // Handle error: expected '}'
                return nullptr;
            }
            advance(); // Consume the '}'
            node->children.push_back(std::move(rule));
        } else if (current_token().type == TokenType::Identifier) {
            std::string key = current_token().value;
            advance(); // Consume the identifier

            if (current_token().type == TokenType::OpenParen) {
                advance(); // Consume '('
                if (current_token().type != TokenType::Identifier) {
                    // Handle error
                    return nullptr;
                }
                std::string var_name = current_token().value;
                advance();
                if (current_token().type != TokenType::CloseParen) {
                    return nullptr;
                }
                advance();
                node->children.push_back(std::make_unique<StylePropertyNode>(key, key + "(" + var_name + ")"));
            } else if (current_token().type == TokenType::Colon) {
                advance(); // Consume the ':'

                if (current_token().type != TokenType::StringLiteral && current_token().type != TokenType::Identifier) {
                    // Handle error: expected a string literal or identifier
                    return nullptr;
                }
                std::string value = current_token().value;
                advance(); // Consume the value

                if (current_token().type != TokenType::Semicolon) {
                    // Handle error: expected ';'
                    return nullptr;
                }
                advance(); // Consume ';'

                node->children.push_back(std::make_unique<StylePropertyNode>(key, value));
            }
        } else if (current_token().type == TokenType::At) {
            advance(); // Consume '@'
            if (current_token().type == TokenType::Identifier && current_token().value == "Style") {
                advance(); // Consume 'Style'
                auto directive = parse_style_directive();
                if (directive) {
                    node->children.push_back(std::move(directive));
                }
            }
        } else {
            // Handle other style content
            advance();
        }
    }

    if (current_token().type != TokenType::CloseBrace) {
        // Handle error: expected '}'
        return nullptr;
    }
    advance(); // Consume the '}'

    return node;
}

std::unique_ptr<ScriptNode> Parser::parse_script() {
    if (current_token().type != TokenType::OpenBrace) {
        // Handle error: expected '{'
        return nullptr;
    }
    int start_pos = current_token().pos + 1;
    advance(); // Consume the '{'

    int brace_level = 1;
    while (brace_level > 0 && current_token().type != TokenType::EndOfFile) {
        if (current_token().type == TokenType::OpenBrace) {
            brace_level++;
        } else if (current_token().type == TokenType::CloseBrace) {
            brace_level--;
        }
        if (brace_level > 0) {
            advance();
        }
    }
    int end_pos = current_token().pos;

    if (current_token().type != TokenType::CloseBrace) {
        // Handle error: expected '}'
        return nullptr;
    }
    advance(); // Consume the '}'

    std::string content = source.substr(start_pos, end_pos - start_pos);
    // trim leading and trailing whitespace
    content.erase(0, content.find_first_not_of(" \t\n\r\f\v"));
    content.erase(content.find_last_not_of(" \t\n\r\f\v") + 1);
    return std::make_unique<ScriptNode>(content);
}

std::unique_ptr<TemplateNode> Parser::parse_template() {
    advance(); // Consume '[Template]'
    if (current_token().type != TokenType::At) {
        // Handle error: expected '@'
        return nullptr;
    }
    advance(); // Consume '@'
    if (current_token().type != TokenType::Identifier) {
        // Handle error: expected identifier
        return nullptr;
    }
    std::string type_str = current_token().value;
    advance(); // Consume the type
    if (current_token().type != TokenType::Identifier) {
        // Handle error: expected identifier
        return nullptr;
    }
    std::string name = current_token().value;
    advance(); // Consume the name

    TemplateType type;
    if (type_str == "Element") {
        type = TemplateType::Element;
    } else if (type_str == "Style") {
        type = TemplateType::Style;
    } else if (type_str == "Var") {
        type = TemplateType::Var;
    } else {
        // Handle error: unknown template type
        return nullptr;
    }

    auto node = std::make_unique<TemplateNode>(name, type);

    if (current_token().type != TokenType::OpenBrace) {
        // Handle error: expected '{'
        return nullptr;
    }
    advance(); // Consume the '{'

    while (current_token().type != TokenType::CloseBrace && current_token().type != TokenType::EndOfFile) {
        if (type == TemplateType::Var) {
            std::string key = current_token().value;
            advance();
            if (current_token().type != TokenType::Colon) {
                // Handle error
                return nullptr;
            }
            advance();
            std::string value = current_token().value;
            advance();
            if (current_token().type != TokenType::Semicolon) {
                return nullptr;
            }
            advance();
            node->variables[key] = value;
        } else if (type == TemplateType::Style) {
            std::string key = current_token().value;
            advance();
            if (current_token().type != TokenType::Colon) {
                // Handle error
                return nullptr;
            }
            advance();
            std::string value = current_token().value;
            advance();
            if (current_token().type != TokenType::Semicolon) {
                return nullptr;
            }
            advance();
            node->children.push_back(std::make_unique<StylePropertyNode>(key, value));
        } else {
            auto statement = parse_statement();
            if (statement) {
                node->children.push_back(std::move(statement));
            }
        }
    }

    if (current_token().type != TokenType::CloseBrace) {
        // Handle error: expected '}'
        return nullptr;
    }
    advance(); // Consume the '}'

    return node;
}

std::unique_ptr<CustomNode> Parser::parse_custom() {
    advance(); // Consume '[Custom]'
    if (current_token().type != TokenType::At) {
        // Handle error: expected '@'
        return nullptr;
    }
    advance(); // Consume '@'
    if (current_token().type != TokenType::Identifier) {
        // Handle error: expected identifier
        return nullptr;
    }
    std::string type_str = current_token().value;
    advance(); // Consume the type
    if (current_token().type != TokenType::Identifier) {
        // Handle error: expected identifier
        return nullptr;
    }
    std::string name = current_token().value;
    advance(); // Consume the name

    CustomType type;
    if (type_str == "Style") {
        type = CustomType::Style;
    } else {
        // Handle error: unknown custom type
        return nullptr;
    }

    auto node = std::make_unique<CustomNode>(name, type);

    if (current_token().type != TokenType::OpenBrace) {
        // Handle error: expected '{'
        return nullptr;
    }
    advance(); // Consume the '{'

    while (current_token().type != TokenType::CloseBrace && current_token().type != TokenType::EndOfFile) {
        if (current_token().type == TokenType::Identifier) {
            node->valueless_properties.push_back(current_token().value);
            advance();
            if (current_token().type == TokenType::Semicolon) {
                advance();
            }
        } else {
            advance();
        }
    }

    if (current_token().type != TokenType::CloseBrace) {
        // Handle error: expected '}'
        return nullptr;
    }
    advance(); // Consume the '}'

    return node;
}

std::unique_ptr<ElementDirectiveNode> Parser::parse_element_directive() {
    if (current_token().type != TokenType::Identifier) {
        // Handle error: expected identifier
        return nullptr;
    }
    std::string name = current_token().value;
    advance(); // Consume the name

    if (current_token().type != TokenType::Semicolon) {
        // Handle error: expected ';'
        return nullptr;
    }
    advance(); // Consume ';'

    return std::make_unique<ElementDirectiveNode>(name);
}

std::unique_ptr<StyleDirectiveNode> Parser::parse_style_directive() {
    if (current_token().type != TokenType::Identifier) {
        // Handle error: expected identifier
        return nullptr;
    }
    std::string name = current_token().value;
    advance(); // Consume the name

    if (current_token().type == TokenType::OpenBrace) {
        auto node = std::make_unique<StyleDirectiveNode>(name);
        advance(); // consume '{'
        while (current_token().type != TokenType::CloseBrace && current_token().type != TokenType::EndOfFile) {
            if (current_token().type == TokenType::Identifier) {
                std::string key = current_token().value;
                advance();
                if (current_token().type == TokenType::Colon) {
                    advance();
                    std::string value = current_token().value;
                    advance();
                    if (current_token().type == TokenType::Semicolon) {
                        advance();
                    }
                    node->properties[key] = value;
                }
            } else {
                advance();
            }
        }
        advance(); // consume '}'
        return node;
    } else if (current_token().type != TokenType::Semicolon) {
        // Handle error: expected ';'
        return nullptr;
    }
    advance(); // Consume ';'

    return std::make_unique<StyleDirectiveNode>(name);
}

Token Parser::current_token() {
    if (current_token_index < tokens.size()) {
        return tokens[current_token_index];
    }
    return {TokenType::EndOfFile, "", 0, 0, (int)source.length()};
}

void Parser::advance() {
    if (current_token_index < tokens.size()) {
        current_token_index++;
    }
}
