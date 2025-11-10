#include "Parser.h"
#include "CHTLNode/TextNode.h"
#include "CHTLNode/StyleNode.h"
#include "CHTLNode/StylePropertyNode.h"
#include "CHTLNode/StyleRuleNode.h"
#include "CHTLNode/ScriptNode.h"
#include "CHTLNode/OriginNode.h"
#include "CHTLNode/TemplateNode.h"
#include "CHTLNode/TemplateUsageNode.h"
#include "CHTLNode/ValueNode.h"
#include "CHTLNode/LiteralValueNode.h"
#include "CHTLNode/VariableUsageNode.h"
#include "CHTLNode/DeleteNode.h"
#include "CHTLNode/InsertNode.h"
#include "CHTLNode/ElementDeleteNode.h"
#include "CHTLNode/BinaryOperationNode.h"
#include "CHTLNode/PropertyReferenceNode.h"
#include "CHTLNode/IfNode.h"
#include "CHTLNode/ElseNode.h"
#include "CHTLNode/ImportNode.h"
#include "CHTLNode/NamespaceNode.h"
#include "CHTLNode/ConfigNode.h"
#include "CHTLNode/ConstraintNode.h"
#include "CHTLNode/UseNode.h"
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

    if (currentToken.type == TokenType::Except) {
        return parseConstraintNode();
    }

    if (currentToken.type == TokenType::If) {
        return parseIfNode();
    }

    if (currentToken.type == TokenType::Use) {
        return parseUseStatement();
    }

    if (currentToken.type == TokenType::OpenBracket) {
        Token peekToken = lexer->peek();
        if (peekToken.value == "Template" || peekToken.value == "Custom") {
            return parseTemplateNode();
        } else if (peekToken.value == "Import") {
            return parseImportNode();
        } else if (peekToken.value == "Namespace") {
            return parseNamespaceNode();
        } else if (peekToken.value == "Configuration") {
            return parseConfigNode();
        }
        return parseOriginNode();
    }

    if (currentToken.type == TokenType::At) {
        return parseTemplateUsageNode();
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

    std::string content;
    TokenType lastTokenType = TokenType::Unknown;
    while (currentToken.type != TokenType::CloseBrace && currentToken.type != TokenType::Eof) {
        if (lastTokenType != TokenType::Unknown &&
            ((lastTokenType == TokenType::Identifier && currentToken.type == TokenType::Identifier) ||
             (lastTokenType == TokenType::Comma))) {
            content += " ";
        }
        content += currentToken.value;
        lastTokenType = currentToken.type;
        consume(currentToken.type);
    }

    consume(TokenType::CloseBrace);

    return std::make_unique<TextNode>(content);
}

std::vector<std::unique_ptr<StylePropertyNode>> Parser::parseStyleProperties() {
    auto properties = std::vector<std::unique_ptr<StylePropertyNode>>();

    while (currentToken.type == TokenType::Identifier) {
        std::string key = currentToken.value;
        consume(TokenType::Identifier);
        consume(TokenType::Colon);

        auto values = std::vector<std::unique_ptr<ValueNode>>();
        while (currentToken.type != TokenType::Semicolon && currentToken.type != TokenType::Eof) {
            values.push_back(parseExpression());

            // After parsing an expression, if the next token is not a semicolon or an operator,
            // it must be another literal value in a multi-part property.
            while (currentToken.type != TokenType::Semicolon &&
                   currentToken.type != TokenType::Plus &&
                   currentToken.type != TokenType::Minus &&
                   currentToken.type != TokenType::Asterisk &&
                   currentToken.type != TokenType::Slash &&
                   currentToken.type != TokenType::Percent &&
                   currentToken.type != TokenType::DoubleAsterisk &&
                   currentToken.type != TokenType::Eof) {
                values.push_back(parseAtom());
            }
        }

        consume(TokenType::Semicolon);
        properties.push_back(std::make_unique<StylePropertyNode>(key, std::move(values)));
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
        if (currentToken.type == TokenType::At) {
            styleNode->children.push_back(parseTemplateUsageNode());
        } else {
            Token peekToken = lexer->peek();

            if (peekToken.type == TokenType::OpenBrace || currentToken.type == TokenType::Ampersand) {
                styleNode->children.push_back(parseStyleRule());
            } else if (peekToken.type == TokenType::Colon) {
                // It's a style property
                auto properties = parseStyleProperties();
                for (auto& prop : properties) {
                    styleNode->children.push_back(std::move(prop));
                }
            } else {
                // Error or unexpected token
                consume(currentToken.type);
            }
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

std::unique_ptr<OriginNode> Parser::parseOriginNode() {
    consume(TokenType::OpenBracket);
    consume(TokenType::Identifier); // Consume "Origin"
    consume(TokenType::CloseBracket);

    consume(TokenType::At);
    std::string type = currentToken.value;
    consume(TokenType::Identifier);

    std::string name;
    if (currentToken.type == TokenType::Identifier) {
        name = currentToken.value;
        consume(TokenType::Identifier);
    }

    if (currentToken.type == TokenType::OpenBrace) {
        consume(TokenType::OpenBrace);
        std::string content = currentToken.value;
        consume(TokenType::String);
        consume(TokenType::CloseBrace);
        return std::make_unique<OriginNode>(type, content, name);
    }
    else {
        consume(TokenType::Semicolon);
        return std::make_unique<OriginNode>(type, "", name);
    }
}


std::unique_ptr<ElementNode> Parser::parseElement() {
    std::string tagName = currentToken.value;
    consume(TokenType::Identifier);

    auto elementNode = std::make_unique<ElementNode>(tagName);

    if (currentToken.type == TokenType::OpenBracket) {
        consume(TokenType::OpenBracket);
        elementNode->index = std::stoi(currentToken.value);
        consume(TokenType::Identifier); // Assuming index is a number parsed as identifier
        consume(TokenType::CloseBracket);
    }

    consume(TokenType::OpenBrace);

    while(currentToken.type != TokenType::CloseBrace && currentToken.type != TokenType::Eof) {
        // Check for attribute first
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
                continue;
            }
        }

        // If not an attribute, parse as a statement
        auto statement = parseStatement();
        if (statement) {
            if (statement->getType() == NodeType::Constraint) {
                // Constraints are not children, they are properties of the element
                auto constraintNode = static_cast<ConstraintNode*>(statement.get());
                elementNode->constraints = constraintNode->constraints;
            } else {
                elementNode->children.push_back(std::move(statement));
            }
        }
    }

    consume(TokenType::CloseBrace);

    return elementNode;
}

std::unique_ptr<TemplateNode> Parser::parseTemplateNode() {
    consume(TokenType::OpenBracket);
    bool isCustom = currentToken.value == "Custom";
    consume(TokenType::Identifier); // Consume "Template" or "Custom"
    consume(TokenType::CloseBracket);

    consume(TokenType::At);
    std::string type = currentToken.value;
    consume(TokenType::Identifier);

    std::string name = currentToken.value;
    consume(TokenType::Identifier);

    auto templateNode = std::make_unique<TemplateNode>();

    if (currentToken.type == TokenType::LessThan) {
        consume(TokenType::LessThan);
        while (currentToken.type != TokenType::OpenBrace && currentToken.type != TokenType::Eof) {
            auto usage = std::make_unique<TemplateUsageNode>(type, currentToken.value);
            templateNode->inheritances.push_back(std::move(usage));
            consume(TokenType::Identifier);
            if (currentToken.type != TokenType::Comma) break;
            consume(TokenType::Comma);
        }
    }

    consume(TokenType::OpenBrace);

    templateNode->type = type;
    templateNode->name = name;
    templateNode->isCustom = isCustom;

    if (type == "Style") {
        while (currentToken.type != TokenType::CloseBrace && currentToken.type != TokenType::Eof) {
            if (currentToken.type == TokenType::At) {
                templateNode->inheritances.push_back(parseTemplateUsageNode());
            } else if (currentToken.type == TokenType::Delete) {
                templateNode->body.push_back(parseDeleteNode());
            } else {
                // Check for valueless properties
                Token peekToken = lexer->peek();
                if (isCustom && (peekToken.type == TokenType::Comma || peekToken.type == TokenType::Semicolon)) {
                    // Valueless properties
                    while (currentToken.type == TokenType::Identifier) {
                        std::string key = currentToken.value;
                        consume(TokenType::Identifier);
                        auto values = std::vector<std::unique_ptr<ValueNode>>();
                        templateNode->body.push_back(std::make_unique<StylePropertyNode>(key, std::move(values)));

                        if (currentToken.type == TokenType::Comma) {
                            consume(TokenType::Comma);
                        } else {
                            break;
                        }
                    }
                    if (currentToken.type == TokenType::Semicolon) {
                        consume(TokenType::Semicolon);
                    }
                } else {
                    auto properties = parseStyleProperties();
                    for (auto& prop : properties) {
                        templateNode->body.push_back(std::move(prop));
                    }
                }
            }
        }
    } else if (type == "Element") {
        while (currentToken.type != TokenType::CloseBrace && currentToken.type != TokenType::Eof) {
            if (currentToken.type == TokenType::Delete) {
                templateNode->body.push_back(parseElementDeleteNode());
            } else {
                templateNode->body.push_back(parseStatement());
            }
        }
    } else if (type == "Var") {
        auto properties = parseStyleProperties();
        for (auto& prop : properties) {
            templateNode->body.push_back(std::move(prop));
        }
    }

    consume(TokenType::CloseBrace);

    return templateNode;
}

std::unique_ptr<TemplateUsageNode> Parser::parseTemplateUsageNode() {
    consume(TokenType::At);
    std::string type = currentToken.value;
    consume(TokenType::Identifier);

    std::string name = currentToken.value;
    consume(TokenType::Identifier);

    auto node = std::make_unique<TemplateUsageNode>(type, name);

    if (currentToken.type == TokenType::From) {
        consume(TokenType::From);
        node->from = currentToken.value;
        consume(TokenType::Identifier);
    }

    if (currentToken.type == TokenType::OpenBrace) {
        consume(TokenType::OpenBrace);
        if (type == "Style") {
            node->provided_properties = parseStyleProperties();
        } else if (type == "Element") {
            while (currentToken.type != TokenType::CloseBrace && currentToken.type != TokenType::Eof) {
                if (currentToken.type == TokenType::Insert) {
                    node->body.push_back(parseInsertNode());
                } else if (currentToken.type == TokenType::Delete) {
                    node->body.push_back(parseElementDeleteNode());
                } else {
                    node->body.push_back(parseStatement());
                }
            }
        }
        consume(TokenType::CloseBrace);
    } else {
        consume(TokenType::Semicolon);
    }

    return node;
}

std::unique_ptr<ConfigNode> Parser::parseConfigNode() {
    consume(TokenType::OpenBracket);
    consume(TokenType::Identifier); // Consume "Configuration"
    consume(TokenType::CloseBracket);

    auto node = std::make_unique<ConfigNode>();

    if (currentToken.type == TokenType::At) {
		consume(TokenType::At);
		if (currentToken.type == TokenType::Identifier && currentToken.value == "Config") {
			consume(TokenType::Identifier);
			node->name = currentToken.value;
			consume(TokenType::Identifier);
		}
		else {
			throw std::runtime_error("Invalid configuration name");
		}
	}

    consume(TokenType::OpenBrace);

    while (currentToken.type != TokenType::CloseBrace && currentToken.type != TokenType::Eof) {
        std::string key = currentToken.value;
        consume(TokenType::Identifier);
        consume(TokenType::Colon);
        std::string value = currentToken.value;
        if (currentToken.type == TokenType::Identifier || currentToken.type == TokenType::String) {
            consume(currentToken.type);
        } else {
            throw std::runtime_error("Invalid value in configuration");
        }
        consume(TokenType::Semicolon);

        node->settings.emplace_back(key, value);

        if (node->name.empty()) {
            if (key == "DEBUG_MODE") {
                config.debugMode = (value == "true");
            } else if (key == "DISABLE_STYLE_AUTO_ADD_CLASS") {
                config.disableStyleAutoAddClass = (value == "true");
            } else if (key == "DISABLE_STYLE_AUTO_ADD_ID") {
                config.disableStyleAutoAddId = (value == "true");
            }
        }
    }

    consume(TokenType::CloseBrace);

    return node;
}

std::unique_ptr<DeleteNode> Parser::parseDeleteNode() {
    consume(TokenType::Delete);

    auto deleteNode = std::make_unique<DeleteNode>();

    while (currentToken.type != TokenType::Semicolon && currentToken.type != TokenType::Eof) {
        if (currentToken.type == TokenType::At) {
            consume(TokenType::At);
            consume(TokenType::Identifier); // Consume "Style"
            deleteNode->inheritances.push_back(currentToken.value);
            consume(TokenType::Identifier);
        } else {
            deleteNode->properties.push_back(currentToken.value);
            consume(TokenType::Identifier);
        }
    }

    consume(TokenType::Semicolon);

    return deleteNode;
}

std::unique_ptr<InsertNode> Parser::parseInsertNode() {
    consume(TokenType::Insert);

    InsertType type;
    if (currentToken.type == TokenType::After) {
        type = InsertType::After;
        consume(TokenType::After);
    } else if (currentToken.type == TokenType::Before) {
        type = InsertType::Before;
        consume(TokenType::Before);
    } else if (currentToken.type == TokenType::Replace) {
        type = InsertType::Replace;
        consume(TokenType::Replace);
    } else if (currentToken.type == TokenType::At) {
        consume(TokenType::At);
        if (currentToken.type == TokenType::Top) {
            type = InsertType::AtTop;
            consume(TokenType::Top);
        } else {
            type = InsertType::AtBottom;
            consume(TokenType::Bottom);
        }
    } else {
        throw std::runtime_error("Invalid insert type");
    }

    std::string target;
    if (type != InsertType::AtTop && type != InsertType::AtBottom) {
        target = currentToken.value;
        consume(TokenType::Identifier);
        if (currentToken.type == TokenType::OpenBracket) {
            consume(TokenType::OpenBracket);
            target += "[" + currentToken.value + "]";
            consume(TokenType::Identifier);
            consume(TokenType::CloseBracket);
        }
    }

    consume(TokenType::OpenBrace);

    auto body = std::vector<std::unique_ptr<BaseNode>>();
    while (currentToken.type != TokenType::CloseBrace && currentToken.type != TokenType::Eof) {
        body.push_back(parseStatement());
    }

    consume(TokenType::CloseBrace);

    return std::make_unique<InsertNode>(type, target, std::move(body));
}

std::unique_ptr<ElementDeleteNode> Parser::parseElementDeleteNode() {
    consume(TokenType::Delete);
    auto deleteNode = std::make_unique<ElementDeleteNode>();

    do {
        if (currentToken.type == TokenType::At) {
            deleteNode->targets.push_back(parseTemplateUsageReference());
        } else {
            deleteNode->targets.push_back(parseElementReference());
        }

        if (currentToken.type == TokenType::Comma) {
            consume(TokenType::Comma);
        } else {
            break;
        }
    } while (currentToken.type != TokenType::Semicolon && currentToken.type != TokenType::Eof);

    consume(TokenType::Semicolon);
    return deleteNode;
}

std::unique_ptr<ElementNode> Parser::parseElementReference() {
    std::string tagName = currentToken.value;
    consume(TokenType::Identifier);

    auto elementNode = std::make_unique<ElementNode>(tagName);

    if (currentToken.type == TokenType::OpenBracket) {
        consume(TokenType::OpenBracket);
        elementNode->index = std::stoi(currentToken.value);
        consume(TokenType::Identifier);
        consume(TokenType::CloseBracket);
    }

    return elementNode;
}

std::unique_ptr<TemplateUsageNode> Parser::parseTemplateUsageReference() {
    consume(TokenType::At);
    std::string type = currentToken.value;
    consume(TokenType::Identifier);

    std::string name = currentToken.value;
    consume(TokenType::Identifier);

    return std::make_unique<TemplateUsageNode>(type, name);
}

std::unique_ptr<ValueNode> Parser::parseConditionalExpression() {
    auto expr = parseLogicalOrExpression();

    if (currentToken.type == TokenType::QuestionMark) {
        consume(TokenType::QuestionMark);
        auto true_branch = parseExpression();
        consume(TokenType::Colon);
        auto false_branch = parseExpression();

        auto node = std::make_unique<ConditionalNode>();
        node->condition = std::move(expr);
        node->true_branch = std::move(true_branch);
        node->false_branch = std::move(false_branch);
        return node;
    }

    return expr;
}

std::unique_ptr<ValueNode> Parser::parseLogicalOrExpression() {
    auto left = parseLogicalAndExpression();

    while (currentToken.type == TokenType::DoubleOr) {
        std::string op = currentToken.value;
        consume(TokenType::DoubleOr);
        auto right = parseLogicalAndExpression();
        auto node = std::make_unique<LogicalNode>();
        node->left = std::move(left);
        node->op = op;
        node->right = std::move(right);
        left = std::move(node);
    }

    return left;
}

std::unique_ptr<ValueNode> Parser::parseAdditiveExpression() {
    auto left = parseTerm();

    while (currentToken.type == TokenType::Plus || currentToken.type == TokenType::Minus) {
        std::string op = currentToken.value;
        consume(currentToken.type);
        auto right = parseTerm();
        left = std::make_unique<BinaryOperationNode>(std::move(left), op, std::move(right));
    }

    return left;
}

std::unique_ptr<ValueNode> Parser::parseLogicalAndExpression() {
    auto left = parseComparisonExpression();

    while (currentToken.type == TokenType::DoubleAnd) {
        std::string op = currentToken.value;
        consume(TokenType::DoubleAnd);
        auto right = parseComparisonExpression();
        auto node = std::make_unique<LogicalNode>();
        node->left = std::move(left);
        node->op = op;
        node->right = std::move(right);
        left = std::move(node);
    }

    return left;
}

std::unique_ptr<ValueNode> Parser::parseComparisonExpression() {
    auto left = parseAdditiveExpression();

    while (currentToken.type == TokenType::GreaterThan ||
           currentToken.type == TokenType::LessThan ||
           currentToken.type == TokenType::GreaterEqual ||
           currentToken.type == TokenType::LessEqual ||
           currentToken.type == TokenType::DoubleEqual ||
           currentToken.type == TokenType::NotEqual)
    {
        std::string op = currentToken.value;
        consume(currentToken.type);
        auto right = parseTerm();
        auto node = std::make_unique<ComparisonNode>();
        node->left = std::move(left);
        node->op = op;
        node->right = std::move(right);
        left = std::move(node);
    }

    return left;
}

std::unique_ptr<ValueNode> Parser::parseExpression() {
    return parseConditionalExpression();
}

std::unique_ptr<ValueNode> Parser::parseTerm() {
    auto left = parseFactor();

    while (currentToken.type == TokenType::Asterisk || currentToken.type == TokenType::Slash || currentToken.type == TokenType::Percent) {
        std::string op = currentToken.value;
        consume(currentToken.type);
        auto right = parseFactor();
        left = std::make_unique<BinaryOperationNode>(std::move(left), op, std::move(right));
    }

    return left;
}

std::unique_ptr<ValueNode> Parser::parseFactor() {
    auto left = parsePower();

    while (currentToken.type == TokenType::DoubleAsterisk) {
        std::string op = currentToken.value;
        consume(currentToken.type);
        auto right = parsePower();
        left = std::make_unique<BinaryOperationNode>(std::move(left), op, std::move(right));
    }

    return left;
}

std::unique_ptr<ValueNode> Parser::parsePower() {
    return parseAtom();
}

std::unique_ptr<ValueNode> Parser::parseAtom() {
    if (currentToken.type == TokenType::Identifier) {
        Token peekToken = lexer->peek();
        if (peekToken.type == TokenType::OpenParen) {
            std::string groupName = currentToken.value;
            consume(TokenType::Identifier);
            consume(TokenType::OpenParen);
            std::string variableName = currentToken.value;
            consume(TokenType::Identifier);
            consume(TokenType::CloseParen);
            auto node = std::make_unique<VariableUsageNode>(groupName, variableName);
            if (currentToken.type == TokenType::From) {
                consume(TokenType::From);
                node->from = currentToken.value;
                consume(TokenType::Identifier);
            }
            return node;
        } else {
            std::string value = currentToken.value;
            size_t dotPos = value.find('.');
            if (dotPos != std::string::npos && value[0] == '#') {
                std::string selector = value.substr(0, dotPos);
                std::string propertyName = value.substr(dotPos + 1);
                consume(TokenType::Identifier);
                return std::make_unique<PropertyReferenceNode>(selector, propertyName);
            }
            auto node = std::make_unique<LiteralValueNode>(currentToken.value);
            consume(TokenType::Identifier);
            return node;
        }
    } else if (currentToken.type == TokenType::String) {
        auto node = std::make_unique<LiteralValueNode>(currentToken.value);
        consume(TokenType::String);
        return node;
    } else if (currentToken.type == TokenType::OpenParen) {
        consume(TokenType::OpenParen);
        auto node = parseExpression();
        consume(TokenType::CloseParen);
        return node;
    } else {
        throw std::runtime_error("Unexpected token in expression");
    }
}

std::unique_ptr<IfNode> Parser::parseIfNode() {
    consume(TokenType::If);
    consume(TokenType::OpenBrace);

    auto ifNode = std::make_unique<IfNode>();

    consume(TokenType::Identifier); // consume "condition"
    consume(TokenType::Colon);
    ifNode->condition = parseExpression();

    while (currentToken.type != TokenType::CloseBrace && currentToken.type != TokenType::Eof) {
        ifNode->body.push_back(parseStatement());
    }

    consume(TokenType::CloseBrace);

    while (currentToken.type == TokenType::Else) {
        ifNode->else_branches.push_back(parseElseNode());
    }

    return ifNode;
}

std::unique_ptr<ElseNode> Parser::parseElseNode() {
    consume(TokenType::Else);
    auto elseNode = std::make_unique<ElseNode>();

    if (currentToken.type == TokenType::If) {
        consume(TokenType::If);
        consume(TokenType::OpenBrace);
        consume(TokenType::Identifier); // consume "condition"
        consume(TokenType::Colon);
        elseNode->condition = parseExpression();
    } else {
        consume(TokenType::OpenBrace);
    }

    while (currentToken.type != TokenType::CloseBrace && currentToken.type != TokenType::Eof) {
        elseNode->body.push_back(parseStatement());
    }

    consume(TokenType::CloseBrace);

    return elseNode;
}

std::unique_ptr<ImportNode> Parser::parseImportNode() {
    consume(TokenType::OpenBracket);
    consume(TokenType::Identifier); // Consume "Import"
    consume(TokenType::CloseBracket);

    std::string qualifier;
    if (currentToken.type == TokenType::OpenBracket) {
        consume(TokenType::OpenBracket);
        qualifier = currentToken.value; // "Custom", "Template", "Origin"
        consume(TokenType::Identifier);
        consume(TokenType::CloseBracket);
    }

    consume(TokenType::At);
    std::string type = currentToken.value;
    consume(TokenType::Identifier);

    std::string itemName;
    if (currentToken.type == TokenType::Identifier) {
        // Could be item name or "from"
        if (currentToken.value != "from") {
            itemName = currentToken.value;
            consume(TokenType::Identifier);
        }
    }

    consume(TokenType::From);
    std::string path = currentToken.value;
    if (currentToken.type == TokenType::String || currentToken.type == TokenType::Identifier) {
        consume(currentToken.type);
    } else {
        throw std::runtime_error("Expected string or identifier for import path");
    }

    std::string alias;
    if (currentToken.type == TokenType::As) {
        consume(TokenType::As);
        alias = currentToken.value;
        consume(TokenType::Identifier);
    }

    consume(TokenType::Semicolon);
    return std::make_unique<ImportNode>(qualifier, type, itemName, path, alias);
}

std::unique_ptr<NamespaceNode> Parser::parseNamespaceNode() {
    consume(TokenType::OpenBracket);
    consume(TokenType::Identifier); // Consume "Namespace"
    consume(TokenType::CloseBracket);

    std::string name = currentToken.value;
    consume(TokenType::Identifier);

    auto node = std::make_unique<NamespaceNode>(name);

    if (currentToken.type == TokenType::OpenBrace) {
        consume(TokenType::OpenBrace);
        while (currentToken.type != TokenType::CloseBrace && currentToken.type != TokenType::Eof) {
            node->body.push_back(parseStatement());
        }
        consume(TokenType::CloseBrace);
    }

    return node;
}

std::unique_ptr<ConstraintNode> Parser::parseConstraintNode() {
    consume(TokenType::Except);

    std::vector<std::string> constraints;
    while (currentToken.type != TokenType::Semicolon && currentToken.type != TokenType::Eof) {
        constraints.push_back(currentToken.value);
        consume(currentToken.type);
        if (currentToken.type == TokenType::Comma) {
            consume(TokenType::Comma);
        }
    }

    consume(TokenType::Semicolon);

    return std::make_unique<ConstraintNode>(constraints);
}

std::unique_ptr<UseNode> Parser::parseUseStatement() {
	consume(TokenType::Use);

	auto useNode = std::make_unique<UseNode>();

	if (currentToken.type == TokenType::Identifier && currentToken.value == "html5") {
		useNode->type = "html5";
		consume(TokenType::Identifier);
	}
	else if (currentToken.type == TokenType::At) {
		consume(TokenType::At);
		if (currentToken.type == TokenType::Identifier && currentToken.value == "Config") {
			useNode->type = "@Config";
			consume(TokenType::Identifier);
			useNode->configName = currentToken.value;
			consume(TokenType::Identifier);
		}
		else {
			throw std::runtime_error("Invalid use statement");
		}
	}
	else {
		throw std::runtime_error("Invalid use statement");
	}

	consume(TokenType::Semicolon);

	return useNode;
}

} // namespace CHTL
