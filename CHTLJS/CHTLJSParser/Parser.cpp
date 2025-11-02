#include "Parser.h"
#include "../CHTLJSNode/EnhancedSelectorNode.h"

CHTLJSParser::CHTLJSParser(const std::vector<CHTLJSToken>& tokens) : tokens(tokens) {}

std::unique_ptr<ProgramNode> CHTLJSParser::parse() {
    auto programNode = std::make_unique<ProgramNode>();
    while (pos < tokens.size()) {
        programNode->children.push_back(parseStatement());
    }
    return programNode;
}

std::unique_ptr<CHTLJSBaseNode> CHTLJSParser::parseStatement() {
    if (pos < tokens.size() && tokens[pos].type == CHTLJSTokenType::EnhancedSelector) {
        return std::make_unique<EnhancedSelectorNode>(tokens[pos++].value);
    }
    // In a real parser, we'd have more statement types here.
    // For now, just advance past unknown tokens.
    pos++;
    return nullptr;
}
