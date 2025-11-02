#include "Parser.h"
#include "../CHTLJSNode/EnhancedSelectorNode.h"
#include "../CHTLJSNode/IdentifierNode.h"
#include "../CHTLJSNode/MemberExpressionNode.h"
#include "../CHTLJSNode/CallExpressionNode.h"
#include "../CHTLJSNode/StringLiteralNode.h"

CHTLJSParser::CHTLJSParser(const std::vector<CHTLJSToken>& tokens) : tokens(tokens) {}

std::unique_ptr<ProgramNode> CHTLJSParser::parse() {
    auto programNode = std::make_unique<ProgramNode>();
    while (pos < tokens.size()) {
        programNode->children.push_back(parseExpression());
    }
    return programNode;
}

std::unique_ptr<ExpressionNode> CHTLJSParser::parseExpression() {
    auto left = parsePrimaryExpression();
    while (pos < tokens.size()) {
        if (tokens[pos].type == CHTLJSTokenType::ArrowOperator) {
            left = parseMemberExpression(std::move(left));
        } else if (tokens[pos].type == CHTLJSTokenType::OpenParen) {
            left = parseCallExpression(std::move(left));
        } else {
            break;
        }
    }
    return left;
}

std::unique_ptr<ExpressionNode> CHTLJSParser::parsePrimaryExpression() {
    if (pos < tokens.size()) {
        if (tokens[pos].type == CHTLJSTokenType::EnhancedSelector) {
            return std::make_unique<EnhancedSelectorNode>(tokens[pos++].value);
        }
        if (tokens[pos].type == CHTLJSTokenType::Identifier) {
            return std::make_unique<IdentifierNode>(tokens[pos++].value);
        }
        if (tokens[pos].type == CHTLJSTokenType::StringLiteral) {
            return std::make_unique<StringLiteralNode>(tokens[pos++].value);
        }
    }
    return nullptr;
}

std::unique_ptr<ExpressionNode> CHTLJSParser::parseMemberExpression(std::unique_ptr<ExpressionNode> object) {
    pos++; // consume '->'
    auto property = parsePrimaryExpression(); // for now, we only support identifiers as properties
    return std::make_unique<MemberExpressionNode>(std::move(object), std::move(property));
}

std::unique_ptr<ExpressionNode> CHTLJSParser::parseCallExpression(std::unique_ptr<ExpressionNode> callee) {
    pos++; // consume '('
    std::vector<std::unique_ptr<ExpressionNode>> arguments;
    if (pos < tokens.size() && tokens[pos].type != CHTLJSTokenType::CloseParen) {
        arguments.push_back(parseExpression());
        while (pos < tokens.size() && tokens[pos].type == CHTLJSTokenType::Comma) {
            pos++; // consume ','
            arguments.push_back(parseExpression());
        }
    }
    if (pos < tokens.size() && tokens[pos].type == CHTLJSTokenType::CloseParen) {
        pos++; // consume ')'
    }
    return std::make_unique<CallExpressionNode>(std::move(callee), std::move(arguments));
}
