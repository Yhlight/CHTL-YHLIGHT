#include "Lexer.h"
#include <cctype>

CHTLJSLexer::CHTLJSLexer(const std::string& source) : source(source) {}

std::vector<CHTLJSToken> CHTLJSLexer::tokenize() {
    std::vector<CHTLJSToken> tokens;
    while (pos < source.length()) {
        if (isspace(source[pos])) {
            pos++;
            continue;
        }
        if (source.substr(pos, 2) == "{{") {
            pos += 2;
            size_t start = pos;
            while (pos < source.length() && source.substr(pos, 2) != "}}") {
                pos++;
            }
            if (pos < source.length()) {
                tokens.push_back({CHTLJSTokenType::EnhancedSelector, source.substr(start, pos - start)});
                pos += 2;
            }
        } else if (source.substr(pos, 2) == "->") {
            tokens.push_back({CHTLJSTokenType::ArrowOperator, "->"});
            pos += 2;
        } else if (source[pos] == '(') {
            tokens.push_back({CHTLJSTokenType::OpenParen, "("});
            pos++;
        } else if (source[pos] == ')') {
            tokens.push_back({CHTLJSTokenType::CloseParen, ")"});
            pos++;
        } else if (source[pos] == ',') {
            tokens.push_back({CHTLJSTokenType::Comma, ","});
            pos++;
        } else if (source[pos] == '"') {
            pos++;
            size_t start = pos;
            while (pos < source.length() && source[pos] != '"') {
                pos++;
            }
            tokens.push_back({CHTLJSTokenType::StringLiteral, source.substr(start, pos - start)});
            pos++;
        } else if (isalpha(source[pos])) {
            size_t start = pos;
            while (pos < source.length() && isalnum(source[pos])) {
                pos++;
            }
            tokens.push_back({CHTLJSTokenType::Identifier, source.substr(start, pos - start)});
        } else {
            pos++;
        }
    }
    return tokens;
}
